#include "BACnetDriver.h"

#include <WiFi.h>
#include <esp_wifi.h>
#include <string.h>
#include <time.h>

#include "Logger.h"

namespace {
constexpr uint8_t BVLL_TYPE_BACNET_IP = 0x81;
constexpr uint8_t BVLL_FUNC_ORIGINAL_UNICAST_NPDU = 0x0A;
constexpr uint8_t BVLL_FUNC_ORIGINAL_BROADCAST_NPDU = 0x0B;

constexpr uint8_t PDU_TYPE_CONFIRMED_SERVICE_REQUEST = 0x00;
constexpr uint8_t PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST = 0x10;
constexpr uint8_t PDU_TYPE_SIMPLE_ACK = 0x20;
constexpr uint8_t PDU_TYPE_COMPLEX_ACK = 0x30;
constexpr uint8_t PDU_TYPE_ERROR = 0x50;

constexpr uint8_t SERVICE_UNCONFIRMED_I_AM = 0x00;
constexpr uint8_t SERVICE_UNCONFIRMED_WHO_IS = 0x08;
constexpr uint8_t SERVICE_CONFIRMED_READ_PROPERTY = 0x0C;
constexpr uint8_t SERVICE_CONFIRMED_WRITE_PROPERTY = 0x0F;

constexpr uint8_t BACNET_SEGMENTATION_NONE = 3;

constexpr uint8_t ERROR_CLASS_OBJECT = 1;
constexpr uint8_t ERROR_CLASS_PROPERTY = 2;
constexpr uint8_t ERROR_CLASS_RESOURCES = 3;
constexpr uint8_t ERROR_CODE_UNKNOWN_OBJECT = 31;
constexpr uint8_t ERROR_CODE_UNKNOWN_PROPERTY = 32;
constexpr uint8_t ERROR_CODE_INVALID_DATA_TYPE = 9;
constexpr uint8_t ERROR_CODE_WRITE_ACCESS_DENIED = 40;
constexpr uint8_t ERROR_CODE_VALUE_OUT_OF_RANGE = 37;
constexpr uint8_t ERROR_CODE_PROPERTY_IS_NOT_AN_ARRAY = 50;
constexpr uint8_t ERROR_CODE_NO_SPACE_FOR_OBJECT = 18;

static inline uint16_t u16be(const uint8_t* p) { return (uint16_t(p[0]) << 8) | p[1]; }

static String macStrFromIface(wifi_interface_t ifx) {
    uint8_t m[6] = {0};
    if (esp_wifi_get_mac(ifx, m) == ESP_OK) {
        char b[18];
        snprintf(b, sizeof(b), "%02X:%02X:%02X:%02X:%02X:%02X", m[0],m[1],m[2],m[3],m[4],m[5]);
        return String(b);
    }
    return String("00:00:00:00:00:00");
}

static String currentDateTimeString() {
    time_t now;
    time(&now);
    if (now < 1609459200) return String("UNSYNCED"); // before 2021
    struct tm t;
    localtime_r(&now, &t);
    char b[32];
    strftime(b, sizeof(b), "%Y-%m-%d %H:%M:%S", &t);
    return String(b);
}

static uint32_t fnv1a32(const uint8_t* data, uint16_t len) {
    uint32_t h = 2166136261UL;
    for (uint16_t i = 0; i < len; i++) { h ^= data[i]; h *= 16777619UL; }
    return h;
}

static bool ipNonZero(const IPAddress& ip) {
    return !(ip[0] == 0 && ip[1] == 0 && ip[2] == 0 && ip[3] == 0);
}
} // namespace

BACnetDriver::BACnetDriver() {
    memset(_lastResp, 0, sizeof(_lastResp));
    // Preload common vendor properties so .NET client can monitor comm interfaces in one place.
    setVendorPropertyString(PROP_MESA_COMM_STATUS, "INIT");
}

bool BACnetDriver::begin(uint16_t udpPort) {
    _udpPort = udpPort;
    _running = ensureTransport();
    if (_running) {
        _lastIAmMs = 0;
        sendIAm(IPAddress(255,255,255,255), _udpPort, true);
    }
    return _running;
}

void BACnetDriver::end() {
    EthernetManager::getInstance().closeBacnetUdp();
    _running = false;
}

bool BACnetDriver::ensureTransport() {
    EthernetManager& eth = EthernetManager::getInstance();
    if (!eth.isIpReady() || !eth.isLinkUp()) {
        _running = false;
        return false;
    }
    if (!eth.ensureBacnetUdpBound(_udpPort)) {
        _running = false;
        return false;
    }
    _running = true;
    return true;
}

void BACnetDriver::task(uint8_t maxPacketsPerCycle) {
    EthernetManager& eth = EthernetManager::getInstance();
    if (eth.hasLinkChanged() || eth.hasIpChanged()) {
        if (_running && !eth.isIpReady()) {
            end();
            _stats.transportRestartCount++;
        }
    }

    if (!ensureTransport()) return;

    const uint32_t now = millis();
    if (_periodicIAmEnabled && (_lastIAmMs == 0 || (uint32_t)(now - _lastIAmMs) >= (uint32_t)_periodicIAmSec * 1000UL)) {
        sendIAm(IPAddress(255,255,255,255), _udpPort, true);
    }

    for (uint8_t i = 0; i < maxPacketsPerCycle; ++i) {
        int n = eth.bacnetParsePacket();
        if (n <= 0) break;
        if (n > (int)sizeof(_rx)) n = sizeof(_rx);
        IPAddress rip = eth.bacnetRemoteIP();
        uint16_t rport = eth.bacnetRemotePort();
        int r = eth.bacnetRead(_rx, n);
        if (r <= 0) break;
        _stats.rxPackets++;
        _stats.lastPacketMs = millis();
        _stats.lastClientIP = rip;
        _stats.lastClientPort = rport;
        handleIncomingPacket(_rx, (uint16_t)r, rip, rport);
    }
}

void BACnetDriver::setDeviceInfo(const DeviceInfo& info) {
    _device = info;
}

BACnetDriver::Obj* BACnetDriver::upsertObj(Obj* arr, size_t len, uint8_t type, uint32_t instance, const char* name, const char* desc, uint16_t units) {
    Obj* freeSlot = nullptr;
    for (size_t i = 0; i < len; ++i) {
        if (arr[i].used && arr[i].type == type && arr[i].instance == instance) {
            if (name) strncpy(arr[i].name, name, sizeof(arr[i].name)-1);
            if (desc) strncpy(arr[i].description, desc, sizeof(arr[i].description)-1);
            arr[i].units = units;
            arr[i].used = true;
            return &arr[i];
        }
        if (!arr[i].used && freeSlot == nullptr) freeSlot = &arr[i];
    }
    if (!freeSlot) return nullptr;
    memset(freeSlot, 0, sizeof(Obj));
    freeSlot->used = true;
    freeSlot->type = type;
    freeSlot->instance = instance;
    freeSlot->units = units;
    if (name) strncpy(freeSlot->name, name, sizeof(freeSlot->name)-1);
    if (desc) strncpy(freeSlot->description, desc, sizeof(freeSlot->description)-1);
    return freeSlot;
}

BACnetDriver::Obj* BACnetDriver::findObj(uint8_t type, uint32_t instance) {
    return const_cast<Obj*>(findObjConst(type, instance));
}

const BACnetDriver::Obj* BACnetDriver::findObjConst(uint8_t type, uint32_t instance) const {
    const Obj* arrays[] = {_ai, _bi, _bo};
    const size_t lens[] = {MAX_AI, MAX_BI, MAX_BO};
    for (size_t a=0;a<3;++a) {
        for (size_t i=0;i<lens[a];++i) {
            const Obj& o = arrays[a][i];
            if (o.used && o.type == type && o.instance == instance) return &o;
        }
    }
    return nullptr;
}

int BACnetDriver::countAllObjects() const {
    int c = 1; // Device object
    for (const auto& o : _ai) if (o.used) c++;
    for (const auto& o : _bi) if (o.used) c++;
    for (const auto& o : _bo) if (o.used) c++;
    return c;
}

bool BACnetDriver::getObjectByFlatIndex(int idx, uint8_t& type, uint32_t& inst) const {
    if (idx == 0) { type = BACNET_OBJ_DEVICE; inst = _device.deviceInstance; return true; }
    int n = 1;
    for (const auto& o : _ai) if (o.used) { if (n++ == idx) { type=o.type; inst=o.instance; return true; } }
    for (const auto& o : _bi) if (o.used) { if (n++ == idx) { type=o.type; inst=o.instance; return true; } }
    for (const auto& o : _bo) if (o.used) { if (n++ == idx) { type=o.type; inst=o.instance; return true; } }
    return false;
}

BACnetDriver::VendorProp* BACnetDriver::upsertVendorProp(uint32_t propId) {
    VendorProp* freeSlot = nullptr;
    for (auto& vp : _vendorProps) {
        if (vp.used && vp.propId == propId) return &vp;
        if (!vp.used && freeSlot == nullptr) freeSlot = &vp;
    }
    if (!freeSlot) return nullptr;
    freeSlot->used = true;
    freeSlot->propId = propId;
    freeSlot->isString = true;
    freeSlot->s = "";
    freeSlot->u = 0;
    return freeSlot;
}

const BACnetDriver::VendorProp* BACnetDriver::findVendorProp(uint32_t propId) const {
    for (const auto& vp : _vendorProps) if (vp.used && vp.propId == propId) return &vp;
    return nullptr;
}

void BACnetDriver::setVendorPropertyString(uint32_t propId, const String& value) {
    if (VendorProp* vp = upsertVendorProp(propId)) { vp->isString = true; vp->s = value; }
}
void BACnetDriver::setVendorPropertyUnsigned(uint32_t propId, uint32_t value) {
    if (VendorProp* vp = upsertVendorProp(propId)) { vp->isString = false; vp->u = value; }
}
void BACnetDriver::setVendorPropertyBool(uint32_t propId, bool value) {
    setVendorPropertyUnsigned(propId, value ? 1U : 0U);
}

void BACnetDriver::updateAnalogInput(uint32_t instance, float value, uint16_t units, const char* name, const char* description) {
    Obj* o = upsertObj(_ai, MAX_AI, BACNET_OBJ_ANALOG_INPUT, instance, name, description, units);
    if (!o) return;
    o->presentValue = value;
    o->lastUpdateMs = millis();
}

void BACnetDriver::updateBinaryInput(uint32_t instance, bool value, const char* name, const char* description) {
    Obj* o = upsertObj(_bi, MAX_BI, BACNET_OBJ_BINARY_INPUT, instance, name, description, BACNET_UNITS_NO_UNITS);
    if (!o) return;
    o->presentValue = value ? 1.0f : 0.0f;
    o->lastUpdateMs = millis();
}

void BACnetDriver::updateBinaryOutput(uint32_t instance, bool value, const char* name, const char* description) {
    Obj* o = upsertObj(_bo, MAX_BO, BACNET_OBJ_BINARY_OUTPUT, instance, name, description, BACNET_UNITS_NO_UNITS);
    if (!o) return;
    o->presentValue = value ? 1.0f : 0.0f;
    o->lastUpdateMs = millis();
}

bool BACnetDriver::getBinaryOutputCommand(uint32_t instance, bool& value) {
    for (uint8_t i = 0; i < MAX_BO; ++i) {
        if (_bo[i].used && _bo[i].instance == instance && _boCommands[i].pending) {
            value = _boCommands[i].value;
            _boCommands[i].pending = false;
            return true;
        }
    }
    return false;
}

void BACnetDriver::handleIncomingPacket(const uint8_t* pkt, uint16_t len, IPAddress rip, uint16_t rport) {
    if (len < 6 || pkt[0] != BVLL_TYPE_BACNET_IP) { _stats.parseErrors++; return; }
    uint8_t bvllFunc = pkt[1];
    uint16_t bvllLen = u16be(pkt+2);
    if (bvllLen > len || bvllLen < 4) { _stats.parseErrors++; return; }
    if (bvllFunc != BVLL_FUNC_ORIGINAL_UNICAST_NPDU && bvllFunc != BVLL_FUNC_ORIGINAL_BROADCAST_NPDU) {
        return; // ignore unsupported BVLL functions
    }

    // NPDU (minimal parsing)
    const uint8_t* npdu = pkt + 4;
    uint16_t nlen = bvllLen - 4;
    if (nlen < 2 || npdu[0] != 0x01) { _stats.parseErrors++; return; }
    uint8_t npduCtrl = npdu[1];
    uint16_t pos = 2;
    // Skip destination/source specs if present (rare for BACnet/IP local network traffic)
    if (npduCtrl & 0x20) { // destination spec
        if (pos + 2 > nlen) return;
        pos += 2; // DNET
        if (pos + 1 > nlen) return;
        uint8_t dlen = npdu[pos++];
        pos += dlen;
        if (pos + 1 > nlen) return;
        pos += 1; // hop count
    }
    if (npduCtrl & 0x08) { // source spec
        if (pos + 2 > nlen) return;
        pos += 2;
        if (pos + 1 > nlen) return;
        uint8_t slen = npdu[pos++];
        pos += slen;
    }
    if (pos >= nlen) return;
    if (npduCtrl & 0x80) {
        // network layer message - ignore
        return;
    }

    const uint8_t* apdu = npdu + pos;
    uint16_t alen = nlen - pos;
    if (alen < 1) return;

    uint8_t pduType = apdu[0] & 0xF0;
    if (pduType == PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST) {
        if (alen < 2) return;
        uint8_t svc = apdu[1];
        if (svc == SERVICE_UNCONFIRMED_WHO_IS) {
            _stats.whoIsCount++;
            handleWhoIs(rip, rport);
        }
        return;
    }

    if (pduType == PDU_TYPE_CONFIRMED_SERVICE_REQUEST) {
        if (alen < 4) return;
        uint8_t invokeId = apdu[2];
        uint8_t svc = apdu[3];
        uint32_t reqHash = hashPacket(pkt, bvllLen);
        if (tryResendDuplicate(rip, rport, invokeId, svc, reqHash)) {
            _stats.duplicateReqHits++;
            return;
        }

        if (svc == SERVICE_CONFIRMED_READ_PROPERTY) {
            _stats.readPropertyCount++;
            handleReadProperty(invokeId, apdu, alen, rip, rport);
        } else if (svc == SERVICE_CONFIRMED_WRITE_PROPERTY) {
            _stats.writePropertyCount++;
            handleWriteProperty(invokeId, apdu, alen, rip, rport);
        }
    }
}

void BACnetDriver::handleWhoIs(IPAddress rip, uint16_t rport) {
    sendIAm(rip, rport, false);
}

void BACnetDriver::handleReadProperty(uint8_t invokeId, const uint8_t* apdu, uint16_t apduLen, IPAddress rip, uint16_t rport) {
    if (apduLen < 8) {
        sendError(invokeId, SERVICE_CONFIRMED_READ_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_UNKNOWN_PROPERTY, rip, rport);
        return;
    }
    // APDU layout for confirmed req: [0]=PDU, [1]=maxseg/maxresp, [2]=invoke, [3]=svc, payload...
    const uint8_t* p = apdu + 4;
    uint16_t plen = apduLen - 4;

    uint8_t objType = 0; uint32_t objInst = 0; uint16_t used = 0;
    if (!decodeObjectId(p, plen, objType, objInst, used)) {
        sendError(invokeId, SERVICE_CONFIRMED_READ_PROPERTY, ERROR_CLASS_OBJECT, ERROR_CODE_UNKNOWN_OBJECT, rip, rport);
        return;
    }
    p += used; plen -= used;

    uint32_t propId = 0; used = 0;
    if (!decodeContextUnsigned(1, p, plen, propId, used)) {
        sendError(invokeId, SERVICE_CONFIRMED_READ_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_UNKNOWN_PROPERTY, rip, rport);
        return;
    }
    p += used; plen -= used;

    bool hasArrayIndex = false; uint32_t arrayIndex = 0;
    if (plen > 0) {
        if (decodeContextUnsigned(2, p, plen, arrayIndex, used)) {
            hasArrayIndex = true;
        }
    }

    uint8_t app[900];
    uint16_t appLen = 0;

    if (objType == BACNET_OBJ_DEVICE && objInst == _device.deviceInstance) {
        switch (propId) {
            case BACNET_PROP_OBJECT_IDENTIFIER: appLen = encodeAppObjectId(app, BACNET_OBJ_DEVICE, _device.deviceInstance); break;
            case BACNET_PROP_OBJECT_NAME: appLen = encodeAppCharacterString(app, _device.objectName); break;
            case BACNET_PROP_DESCRIPTION: appLen = encodeAppCharacterString(app, _device.description); break;
            case BACNET_PROP_LOCATION: appLen = encodeAppCharacterString(app, _device.location); break;
            case BACNET_PROP_VENDOR_NAME: appLen = encodeAppCharacterString(app, _device.vendorName); break;
            case BACNET_PROP_VENDOR_IDENTIFIER: appLen = encodeAppUnsigned(app, _device.vendorIdentifier); break;
            case BACNET_PROP_MODEL_NAME: appLen = encodeAppCharacterString(app, _device.modelName); break;
            case BACNET_PROP_SERIAL_NUMBER: appLen = encodeAppCharacterString(app, _device.serialNumber); break;
            case BACNET_PROP_FIRMWARE_REVISION: appLen = encodeAppCharacterString(app, _device.firmwareRevision); break;
            case BACNET_PROP_APPLICATION_SOFTWARE_VERSION: appLen = encodeAppCharacterString(app, _device.applicationSoftwareVersion); break;
            case BACNET_PROP_PROTOCOL_VERSION: appLen = encodeAppUnsigned(app, 1); break;
            case BACNET_PROP_PROTOCOL_REVISION: appLen = encodeAppUnsigned(app, 14); break;
            case BACNET_PROP_MAX_APDU_LENGTH_ACCEPTED: appLen = encodeAppUnsigned(app, 480); break;
            case BACNET_PROP_SEGMENTATION_SUPPORTED: appLen = encodeAppEnumerated(app, BACNET_SEGMENTATION_NONE); break;
            case BACNET_PROP_OBJECT_LIST: {
                int total = countAllObjects();
                if (hasArrayIndex) {
                    if (arrayIndex == 0) {
                        appLen = encodeAppUnsigned(app, (uint32_t)total);
                    } else {
                        uint8_t ot; uint32_t oi;
                        if (!getObjectByFlatIndex((int)arrayIndex - 1, ot, oi)) {
                            sendError(invokeId, SERVICE_CONFIRMED_READ_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_VALUE_OUT_OF_RANGE, rip, rport);
                            return;
                        }
                        appLen = encodeAppObjectId(app, ot, oi);
                    }
                } else {
                    // Entire array
                    uint16_t pos2 = 0;
                    pos2 += encodeOpeningTag(app + pos2, 0);
                    for (int i = 0; i < total; ++i) {
                        uint8_t ot; uint32_t oi;
                        if (!getObjectByFlatIndex(i, ot, oi)) continue;
                        pos2 += encodeAppObjectId(app + pos2, ot, oi);
                        if (pos2 > sizeof(app) - 8) break;
                    }
                    pos2 += encodeClosingTag(app + pos2, 0);
                    appLen = pos2;
                }
                break;
            }
            default: {
                if (const VendorProp* vp = findVendorProp(propId)) {
                    if (vp->isString) appLen = encodeAppCharacterString(app, vp->s.c_str());
                    else appLen = encodeAppUnsigned(app, vp->u);
                } else {
                    // Some dynamic comm/RTC properties are injected on demand if not pre-set.
                    if (propId == PROP_MESA_DEVICE_DATETIME) {
                        appLen = encodeAppCharacterString(app, currentDateTimeString().c_str());
                    } else if (propId == PROP_MESA_STA_MAC_ADDRESS) {
                        appLen = encodeAppCharacterString(app, macStrFromIface(WIFI_IF_STA).c_str());
                    } else if (propId == PROP_MESA_AP_MAC_ADDRESS) {
                        appLen = encodeAppCharacterString(app, macStrFromIface(WIFI_IF_AP).c_str());
                    } else {
                        sendError(invokeId, SERVICE_CONFIRMED_READ_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_UNKNOWN_PROPERTY, rip, rport);
                        return;
                    }
                }
                break;
            }
        }
        sendReadPropertyAck(invokeId, objType, objInst, propId, hasArrayIndex, arrayIndex, app, appLen, rip, rport);
        return;
    }

    const Obj* o = findObjConst(objType, objInst);
    if (!o) {
        sendError(invokeId, SERVICE_CONFIRMED_READ_PROPERTY, ERROR_CLASS_OBJECT, ERROR_CODE_UNKNOWN_OBJECT, rip, rport);
        return;
    }

    switch (propId) {
        case BACNET_PROP_OBJECT_IDENTIFIER: appLen = encodeAppObjectId(app, objType, objInst); break;
        case BACNET_PROP_OBJECT_NAME: appLen = encodeAppCharacterString(app, o->name); break;
        case BACNET_PROP_OBJECT_TYPE: appLen = encodeAppEnumerated(app, objType); break;
        case BACNET_PROP_DESCRIPTION: appLen = encodeAppCharacterString(app, o->description); break;
        case BACNET_PROP_PRESENT_VALUE:
            if (objType == BACNET_OBJ_BINARY_INPUT || objType == BACNET_OBJ_BINARY_OUTPUT) {
                appLen = encodeAppEnumerated(app, (o->presentValue > 0.5f) ? 1U : 0U); // active/inactive
            } else {
                appLen = encodeAppReal(app, o->presentValue);
            }
            break;
        case BACNET_PROP_UNITS:
            if (objType == BACNET_OBJ_ANALOG_INPUT) appLen = encodeAppEnumerated(app, o->units);
            else { sendError(invokeId, SERVICE_CONFIRMED_READ_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_UNKNOWN_PROPERTY, rip, rport); return; }
            break;
        case BACNET_PROP_OUT_OF_SERVICE: appLen = encodeAppBoolean(app, o->outOfService); break;
        default:
            sendError(invokeId, SERVICE_CONFIRMED_READ_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_UNKNOWN_PROPERTY, rip, rport);
            return;
    }

    sendReadPropertyAck(invokeId, objType, objInst, propId, hasArrayIndex, arrayIndex, app, appLen, rip, rport);
}

void BACnetDriver::handleWriteProperty(uint8_t invokeId, const uint8_t* apdu, uint16_t apduLen, IPAddress rip, uint16_t rport) {
    const uint8_t* p = apdu + 4;
    uint16_t plen = (apduLen >= 4) ? (apduLen - 4) : 0;
    if (plen < 6) {
        sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_UNKNOWN_PROPERTY, rip, rport);
        return;
    }

    uint8_t objType = 0; uint32_t objInst = 0; uint16_t used = 0;
    if (!decodeObjectId(p, plen, objType, objInst, used)) {
        sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_OBJECT, ERROR_CODE_UNKNOWN_OBJECT, rip, rport);
        return;
    }
    p += used; plen -= used;

    uint32_t propId = 0;
    if (!decodeContextUnsigned(1, p, plen, propId, used)) {
        sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_UNKNOWN_PROPERTY, rip, rport);
        return;
    }
    p += used; plen -= used;

    // Optional array index (tag 2) before value (tag 3)
    uint32_t arrayIndex = 0;
    if (decodeContextUnsigned(2, p, plen, arrayIndex, used)) {
        p += used; plen -= used;
    }

    // Expect opening tag [3]
    if (plen < 2 || p[0] != ((3u << 4) | 0x0Eu)) {
        sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_INVALID_DATA_TYPE, rip, rport);
        return;
    }
    p += 1; plen -= 1;

    // Find matching closing tag [3]
    uint16_t valLen = 0;
    while (valLen < plen) {
        if (p[valLen] == ((3u << 4) | 0x0Fu)) break;
        ++valLen;
    }
    if (valLen >= plen) {
        sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_INVALID_DATA_TYPE, rip, rport);
        return;
    }

    if (objType == BACNET_OBJ_BINARY_OUTPUT && propId == BACNET_PROP_PRESENT_VALUE) {
        bool v = false; uint16_t consumed = 0;
        if (!decodeAnyValueToBool(p, valLen, v, consumed)) {
            sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_INVALID_DATA_TYPE, rip, rport);
            return;
        }
        Obj* o = findObj(objType, objInst);
        if (!o) {
            sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_OBJECT, ERROR_CODE_UNKNOWN_OBJECT, rip, rport);
            return;
        }
        o->presentValue = v ? 1.0f : 0.0f;
        o->lastUpdateMs = millis();
        // Queue command by BO slot index
        for (uint8_t i = 0; i < MAX_BO; ++i) {
            if (_bo[i].used && _bo[i].instance == objInst) {
                _boCommands[i].pending = true;
                _boCommands[i].value = v;
                break;
            }
        }
        sendSimpleAck(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, rip, rport);
        return;
    }

    if (objType == BACNET_OBJ_DEVICE && propId == PROP_MESA_REFRESH_INTERVAL_MS) {
        // accept unsigned only
        if (valLen < 1 || (p[0] >> 4) != 2) {
            sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_INVALID_DATA_TYPE, rip, rport);
            return;
        }
        uint8_t lvt = p[0] & 0x07;
        uint8_t l = (lvt <= 4) ? lvt : 0;
        if (l == 0 || 1 + l > valLen) {
            sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_INVALID_DATA_TYPE, rip, rport);
            return;
        }
        uint32_t v=0; for (uint8_t i=0;i<l;i++) v = (v<<8) | p[1+i];
        if (v < 50 || v > 60000) {
            sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_VALUE_OUT_OF_RANGE, rip, rport);
            return;
        }
        _refreshIntervalMs = (uint16_t)v;
        setVendorPropertyUnsigned(PROP_MESA_REFRESH_INTERVAL_MS, _refreshIntervalMs);
        sendSimpleAck(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, rip, rport);
        return;
    }

    sendError(invokeId, SERVICE_CONFIRMED_WRITE_PROPERTY, ERROR_CLASS_PROPERTY, ERROR_CODE_WRITE_ACCESS_DENIED, rip, rport);
}

// ---------------- Encoding helpers ----------------
uint16_t BACnetDriver::encodeObjectId(uint8_t* out, uint8_t objectType, uint32_t instance) {
    uint32_t v = ((uint32_t)objectType << 22) | (instance & 0x3FFFFF);
    out[0] = (v >> 24) & 0xFF; out[1] = (v >> 16) & 0xFF; out[2] = (v >> 8) & 0xFF; out[3] = v & 0xFF;
    return 4;
}

uint16_t BACnetDriver::encodeAppNull(uint8_t* out) { out[0] = 0x00; return 1; }
uint16_t BACnetDriver::encodeAppBoolean(uint8_t* out, bool value) { out[0] = uint8_t((1 << 4) | (value ? 1 : 0)); return 1; }

uint16_t BACnetDriver::encodeAppUnsigned(uint8_t* out, uint32_t value) {
    uint8_t tmp[4]; uint8_t len = 0;
    if (value <= 0xFF) { tmp[0] = (uint8_t)value; len = 1; }
    else if (value <= 0xFFFF) { tmp[0] = (value>>8)&0xFF; tmp[1] = value&0xFF; len = 2; }
    else if (value <= 0xFFFFFF) { tmp[0]=(value>>16)&0xFF; tmp[1]=(value>>8)&0xFF; tmp[2]=value&0xFF; len=3; }
    else { tmp[0]=(value>>24)&0xFF; tmp[1]=(value>>16)&0xFF; tmp[2]=(value>>8)&0xFF; tmp[3]=value&0xFF; len=4; }
    out[0] = (2 << 4) | len; memcpy(out+1, tmp, len); return 1 + len;
}

uint16_t BACnetDriver::encodeAppEnumerated(uint8_t* out, uint32_t value) {
    uint8_t tmp[4]; uint8_t len = 0;
    if (value <= 0xFF) { tmp[0] = (uint8_t)value; len = 1; }
    else if (value <= 0xFFFF) { tmp[0] = (value>>8)&0xFF; tmp[1] = value&0xFF; len = 2; }
    else if (value <= 0xFFFFFF) { tmp[0]=(value>>16)&0xFF; tmp[1]=(value>>8)&0xFF; tmp[2]=value&0xFF; len=3; }
    else { tmp[0]=(value>>24)&0xFF; tmp[1]=(value>>16)&0xFF; tmp[2]=(value>>8)&0xFF; tmp[3]=value&0xFF; len=4; }
    out[0] = (9 << 4) | len; memcpy(out+1, tmp, len); return 1 + len;
}

uint16_t BACnetDriver::encodeAppReal(uint8_t* out, float value) {
    out[0] = (4 << 4) | 4;
    union { float f; uint8_t b[4]; } u; u.f = value;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    out[1]=u.b[3]; out[2]=u.b[2]; out[3]=u.b[1]; out[4]=u.b[0];
#else
    memcpy(out+1, u.b, 4);
#endif
    return 5;
}

uint16_t BACnetDriver::encodeAppCharacterString(uint8_t* out, const char* s) {
    if (!s) s = "";
    size_t n = strlen(s);
    if (n > 220) n = 220;
    // BACnet CharacterString application tag with extended length if needed
    if (n + 1 <= 4) {
        out[0] = (7 << 4) | uint8_t(n + 1);
        out[1] = 0; // ANSI X3.4 / UTF-8 clients often accept ANSI tag for ASCII text
        memcpy(out + 2, s, n);
        return uint16_t(2 + n);
    }
    out[0] = (7 << 4) | 5; // extended length follows
    out[1] = uint8_t(n + 1);
    out[2] = 0;
    memcpy(out + 3, s, n);
    return uint16_t(3 + n);
}

uint16_t BACnetDriver::encodeAppObjectId(uint8_t* out, uint8_t objectType, uint32_t instance) {
    out[0] = (12 << 4) | 4;
    return 1 + encodeObjectId(out + 1, objectType, instance);
}

uint16_t BACnetDriver::encodeContextTagHeader(uint8_t* out, uint8_t tagNo, uint16_t len) {
    if (len <= 4) { out[0] = uint8_t((tagNo << 4) | 0x08 | len); return 1; }
    out[0] = uint8_t((tagNo << 4) | 0x08 | 5);
    out[1] = (uint8_t)len;
    return 2;
}

uint16_t BACnetDriver::encodeContextUnsigned(uint8_t* out, uint8_t tagNo, uint32_t value) {
    uint8_t tmp[4]; uint8_t len = 0;
    if (value <= 0xFF) { tmp[0] = (uint8_t)value; len = 1; }
    else if (value <= 0xFFFF) { tmp[0] = (value>>8)&0xFF; tmp[1]=value&0xFF; len=2; }
    else if (value <= 0xFFFFFF) { tmp[0]=(value>>16)&0xFF; tmp[1]=(value>>8)&0xFF; tmp[2]=value&0xFF; len=3; }
    else { tmp[0]=(value>>24)&0xFF; tmp[1]=(value>>16)&0xFF; tmp[2]=(value>>8)&0xFF; tmp[3]=value&0xFF; len=4; }
    uint16_t p = 0; p += encodeContextTagHeader(out+p, tagNo, len); memcpy(out+p, tmp, len); p += len; return p;
}

uint16_t BACnetDriver::encodeContextObjectId(uint8_t* out, uint8_t tagNo, uint8_t objType, uint32_t inst) {
    out[0] = uint8_t((tagNo << 4) | 0x08 | 4);
    encodeObjectId(out+1, objType, inst);
    return 5;
}

uint16_t BACnetDriver::encodeOpeningTag(uint8_t* out, uint8_t tagNo) { out[0] = uint8_t((tagNo << 4) | 0x0E); return 1; }
uint16_t BACnetDriver::encodeClosingTag(uint8_t* out, uint8_t tagNo) { out[0] = uint8_t((tagNo << 4) | 0x0F); return 1; }

bool BACnetDriver::decodeObjectId(const uint8_t* buf, uint16_t len, uint8_t& objectType, uint32_t& instance, uint16_t& consumed) {
    consumed = 0;
    if (len < 5) return false;
    uint8_t h = buf[0];
    if (((h >> 4) != 0) || ((h & 0x08) == 0) || ((h & 0x07) != 4)) return false; // context tag 0 len4
    uint32_t v = (uint32_t(buf[1])<<24) | (uint32_t(buf[2])<<16) | (uint32_t(buf[3])<<8) | uint32_t(buf[4]);
    objectType = (uint8_t)((v >> 22) & 0x03FF);
    instance = v & 0x3FFFFF;
    consumed = 5;
    return true;
}

bool BACnetDriver::decodeContextUnsigned(uint8_t expectedTag, const uint8_t* buf, uint16_t len, uint32_t& value, uint16_t& consumed) {
    consumed = 0; value = 0;
    if (len < 2) return false;
    uint8_t h = buf[0];
    if ((h >> 4) != expectedTag) return false;
    if ((h & 0x08) == 0) return false;
    uint8_t l = h & 0x07;
    uint16_t hdr = 1;
    if (l == 5) {
        if (len < 3) return false;
        l = buf[1];
        hdr = 2;
    }
    if (l == 0 || hdr + l > len || l > 4) return false;
    for (uint8_t i = 0; i < l; i++) value = (value << 8) | buf[hdr + i];
    consumed = hdr + l;
    return true;
}

bool BACnetDriver::decodeAnyValueToBool(const uint8_t* buf, uint16_t len, bool& value, uint16_t& consumed) {
    consumed = 0;
    if (len < 1) return false;
    uint8_t tag = buf[0] >> 4;
    uint8_t lvt = buf[0] & 0x07;
    // BOOLEAN app tag (1) uses lvt = value
    if (tag == 1) { value = (lvt != 0); consumed = 1; return true; }
    // ENUMERATED app tag (9) inactive/active = 0/1
    if (tag == 9) {
        uint8_t l = lvt;
        if (l == 0 || l > 4 || len < 1 + l) return false;
        uint32_t e = 0; for (uint8_t i=0;i<l;i++) e = (e<<8) | buf[1+i];
        value = (e == 1); consumed = 1 + l; return true;
    }
    // Unsigned 0/1 accepted
    if (tag == 2) {
        uint8_t l = lvt;
        if (l == 0 || l > 4 || len < 1 + l) return false;
        uint32_t u = 0; for (uint8_t i=0;i<l;i++) u = (u<<8) | buf[1+i];
        value = (u != 0); consumed = 1 + l; return true;
    }
    return false;
}

// ---------------- Response builders ----------------
bool BACnetDriver::sendPacket(const uint8_t* npduApdu, uint16_t len, IPAddress rip, uint16_t rport, bool broadcast) {
    if (len + 4 > sizeof(_tx)) return false;
    _tx[0] = BVLL_TYPE_BACNET_IP;
    _tx[1] = broadcast ? BVLL_FUNC_ORIGINAL_BROADCAST_NPDU : BVLL_FUNC_ORIGINAL_UNICAST_NPDU;
    uint16_t total = len + 4;
    _tx[2] = (total >> 8) & 0xFF;
    _tx[3] = total & 0xFF;
    memcpy(_tx + 4, npduApdu, len);
    bool ok = EthernetManager::getInstance().bacnetSend(broadcast ? IPAddress(255,255,255,255) : rip, rport, _tx, total);
    if (ok) _stats.txPackets++;
    return ok;
}

void BACnetDriver::sendIAm(IPAddress rip, uint16_t rport, bool broadcast) {
    uint8_t p[256]; uint16_t pos = 0;
    // NPDU
    p[pos++] = 0x01; // version
    p[pos++] = 0x00; // control (no routing, expecting reply false)
    // APDU Unconfirmed Service Request
    p[pos++] = PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST;
    p[pos++] = SERVICE_UNCONFIRMED_I_AM;
    pos += encodeAppObjectId(p + pos, BACNET_OBJ_DEVICE, _device.deviceInstance);
    pos += encodeAppUnsigned(p + pos, _device.vendorIdentifier);
    pos += encodeAppUnsigned(p + pos, 480);
    pos += encodeAppEnumerated(p + pos, BACNET_SEGMENTATION_NONE);
    bool ok = sendPacket(p, pos, rip, rport, broadcast);
    if (ok) {
        _stats.iAmCount++;
        _lastIAmMs = millis();
    }
}

void BACnetDriver::sendSimpleAck(uint8_t invokeId, uint8_t service, IPAddress rip, uint16_t rport) {
    uint8_t p[64]; uint16_t pos = 0;
    p[pos++] = 0x01; p[pos++] = 0x00; // NPDU
    p[pos++] = PDU_TYPE_SIMPLE_ACK;
    p[pos++] = invokeId;
    p[pos++] = service;
    sendPacket(p, pos, rip, rport, false);
}

void BACnetDriver::sendError(uint8_t invokeId, uint8_t service, uint8_t errorClass, uint8_t errorCode, IPAddress rip, uint16_t rport) {
    uint8_t p[128]; uint16_t pos = 0;
    p[pos++] = 0x01; p[pos++] = 0x00;
    p[pos++] = PDU_TYPE_ERROR;
    p[pos++] = invokeId;
    p[pos++] = service;
    // Error class + code as application ENUMERATEDs
    pos += encodeAppEnumerated(p + pos, errorClass);
    pos += encodeAppEnumerated(p + pos, errorCode);
    sendPacket(p, pos, rip, rport, false);
}

void BACnetDriver::sendReadPropertyAck(uint8_t invokeId, uint8_t objType, uint32_t objInstance, uint32_t propId,
                                       bool hasArrayIndex, uint32_t arrayIndex,
                                       const uint8_t* appData, uint16_t appLen,
                                       IPAddress rip, uint16_t rport) {
    uint8_t p[1200]; uint16_t pos = 0;
    p[pos++] = 0x01; p[pos++] = 0x00; // NPDU
    p[pos++] = PDU_TYPE_COMPLEX_ACK;
    p[pos++] = invokeId;
    p[pos++] = SERVICE_CONFIRMED_READ_PROPERTY;
    pos += encodeContextObjectId(p + pos, 0, objType, objInstance);
    pos += encodeContextUnsigned(p + pos, 1, propId);
    if (hasArrayIndex) pos += encodeContextUnsigned(p + pos, 2, arrayIndex);
    pos += encodeOpeningTag(p + pos, 3);
    if (appLen > 0 && appData) { memcpy(p + pos, appData, appLen); pos += appLen; }
    pos += encodeClosingTag(p + pos, 3);
    sendPacket(p, pos, rip, rport, false);

    // Build duplicate-cache payload (actual on-wire packet bytes including BVLL) in _lastResp via sendPacket wrapper source _tx
    // _tx currently contains on-wire packet after sendPacket. Cache from there.
    uint16_t wireLen = uint16_t((_tx[2] << 8) | _tx[3]);
    cacheLastResponse(rip, rport, invokeId, SERVICE_CONFIRMED_READ_PROPERTY, _lastReqHash, _tx, wireLen);
}

// ---------------- Duplicate cache ----------------
uint32_t BACnetDriver::hashPacket(const uint8_t* data, uint16_t len) const { return fnv1a32(data, len); }

bool BACnetDriver::tryResendDuplicate(IPAddress rip, uint16_t rport, uint8_t invokeId, uint8_t serviceChoice, uint32_t reqHash) {
    (void)rip; (void)rport; (void)invokeId; (void)serviceChoice; (void)reqHash;
    // Phase-1 port keeps duplicate cache storage but does not actively resend duplicates yet.
    return false;
}

void BACnetDriver::cacheLastResponse(IPAddress rip, uint16_t rport, uint8_t invokeId, uint8_t serviceChoice, uint32_t reqHash, const uint8_t* data, uint16_t len) {
    (void)reqHash; // simplified cache key uses invoke/service/ip/port only in this phase
    if (!data || len == 0 || len > sizeof(_lastResp)) return;
    memcpy(_lastResp, data, len);
    _lastRespLen = len;
    _lastReqIP = rip;
    _lastReqPort = rport;
    _lastReqInvokeId = invokeId;
    _lastReqService = serviceChoice;
}

