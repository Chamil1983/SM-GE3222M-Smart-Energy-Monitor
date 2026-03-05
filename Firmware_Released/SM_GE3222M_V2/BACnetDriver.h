#pragma once

#include <Arduino.h>
#include <IPAddress.h>

#include "EthernetManager.h"

// BACnet/IP constants
static constexpr uint16_t BACNET_DEFAULT_UDP_PORT = 47808;  // 0xBAC0
static constexpr uint16_t BACNET_RX_BUFFER_SIZE = 1500;
static constexpr uint16_t BACNET_TX_BUFFER_SIZE = 1500;

// BACnet object types (subset)
static constexpr uint8_t BACNET_OBJ_ANALOG_INPUT = 0;
static constexpr uint8_t BACNET_OBJ_BINARY_INPUT = 3;
static constexpr uint8_t BACNET_OBJ_BINARY_OUTPUT = 4;
static constexpr uint8_t BACNET_OBJ_DEVICE = 8;

// BACnet property IDs (subset + requested device metadata)
static constexpr uint32_t BACNET_PROP_OBJECT_IDENTIFIER = 75;
static constexpr uint32_t BACNET_PROP_OBJECT_NAME = 77;
static constexpr uint32_t BACNET_PROP_OBJECT_TYPE = 79;
static constexpr uint32_t BACNET_PROP_PRESENT_VALUE = 85;
static constexpr uint32_t BACNET_PROP_DESCRIPTION = 28;
static constexpr uint32_t BACNET_PROP_UNITS = 117;
static constexpr uint32_t BACNET_PROP_OUT_OF_SERVICE = 81;
static constexpr uint32_t BACNET_PROP_LOCATION = 58;
static constexpr uint32_t BACNET_PROP_MODEL_NAME = 70;
static constexpr uint32_t BACNET_PROP_VENDOR_NAME = 121;
static constexpr uint32_t BACNET_PROP_VENDOR_IDENTIFIER = 120;
static constexpr uint32_t BACNET_PROP_FIRMWARE_REVISION = 44;
static constexpr uint32_t BACNET_PROP_APPLICATION_SOFTWARE_VERSION = 12;
static constexpr uint32_t BACNET_PROP_PROTOCOL_VERSION = 98;
static constexpr uint32_t BACNET_PROP_PROTOCOL_REVISION = 139;
static constexpr uint32_t BACNET_PROP_OBJECT_LIST = 76;
static constexpr uint32_t BACNET_PROP_MAX_APDU_LENGTH_ACCEPTED = 62;
static constexpr uint32_t BACNET_PROP_SEGMENTATION_SUPPORTED = 107;
static constexpr uint32_t BACNET_PROP_SERIAL_NUMBER = 372;

// Vendor (MESA) properties (512+)
static constexpr uint32_t PROP_MESA_MAC_ADDRESS = 512;
static constexpr uint32_t PROP_MESA_HW_VERSION = 513;
static constexpr uint32_t PROP_MESA_YEAR_DEVELOPED = 514;
static constexpr uint32_t PROP_MESA_SUBNET = 515;
static constexpr uint32_t PROP_MESA_GATEWAY = 516;
static constexpr uint32_t PROP_MESA_DNS1 = 517;
static constexpr uint32_t PROP_MESA_AP_SSID = 518;
static constexpr uint32_t PROP_MESA_AP_PASS = 519;
static constexpr uint32_t PROP_MESA_AP_IP = 520;
static constexpr uint32_t PROP_MESA_DEVICE_DATETIME = 521;
static constexpr uint32_t PROP_MESA_AP_MAC_ADDRESS = 522;
static constexpr uint32_t PROP_MESA_STA_IP_ADDRESS = 523;
static constexpr uint32_t PROP_MESA_STA_MAC_ADDRESS = 524;
static constexpr uint32_t PROP_MESA_ETH_IP_ADDRESS = 525;
static constexpr uint32_t PROP_MESA_ETH_MAC_ADDRESS = 526;
static constexpr uint32_t PROP_MESA_BACNET_PORT = 527;
static constexpr uint32_t PROP_MESA_ETH_SUBNET = 528;
static constexpr uint32_t PROP_MESA_ETH_GATEWAY = 529;
static constexpr uint32_t PROP_MESA_ETH_DNS = 530;
static constexpr uint32_t PROP_MESA_MODBUS_CONFIG = 531;
static constexpr uint32_t PROP_MESA_WIFI_STA_CONFIG = 532;
static constexpr uint32_t PROP_MESA_WIFI_AP_CONFIG = 533;
static constexpr uint32_t PROP_MESA_ETH_CONFIG = 534;
static constexpr uint32_t PROP_MESA_TIME_SYNC_STATUS = 535;
static constexpr uint32_t PROP_MESA_LAST_SYNC_UNIX = 536;
static constexpr uint32_t PROP_MESA_REFRESH_INTERVAL_MS = 537;
static constexpr uint32_t PROP_MESA_COMM_STATUS = 538;

// Units subset
static constexpr uint16_t BACNET_UNITS_NO_UNITS = 95;
static constexpr uint16_t BACNET_UNITS_VOLTS = 5;
static constexpr uint16_t BACNET_UNITS_AMPERES = 3;
static constexpr uint16_t BACNET_UNITS_WATTS = 47;
static constexpr uint16_t BACNET_UNITS_VOLT_AMPERES = 50;
static constexpr uint16_t BACNET_UNITS_VOLT_AMPERES_REACTIVE = 51;
static constexpr uint16_t BACNET_UNITS_HERTZ = 27;
static constexpr uint16_t BACNET_UNITS_PERCENT = 98;
static constexpr uint16_t BACNET_UNITS_DEGREES_CELSIUS = 62;
static constexpr uint16_t BACNET_UNITS_KILOWATT_HOURS = 19;
static constexpr uint16_t BACNET_UNITS_KILOVOLT_AMPERE_HOURS = 146; // commonly used enum (tool-dependent)
static constexpr uint16_t BACNET_UNITS_KILOVAR_HOURS = 18;

class BACnetDriver {
public:
    struct DeviceInfo {
        uint32_t deviceInstance = 88160;
        char objectName[48] = "SM_GE3222M";
        char description[96] = "SM-GE3222M Smart Energy Meter";
        char location[64] = "Panel";
        char vendorName[64] = "Microcode Embedded Systems and Automation";
        uint16_t vendorIdentifier = 999;
        char modelName[32] = "SM_GE3222M_V2";
        char serialNumber[32] = "AUTO";
        char firmwareRevision[24] = "2.0.1";
        char applicationSoftwareVersion[24] = "2.0.1";
    };

    struct RuntimeStats {
        uint32_t rxPackets = 0;
        uint32_t txPackets = 0;
        uint32_t parseErrors = 0;
        uint32_t readPropertyCount = 0;
        uint32_t writePropertyCount = 0;
        uint32_t whoIsCount = 0;
        uint32_t iAmCount = 0;
        uint32_t duplicateReqHits = 0;
        uint32_t transportRestartCount = 0;
        uint32_t lastPacketMs = 0;
        uint32_t lastObjectRefreshMs = 0;
        IPAddress lastClientIP = IPAddress(0,0,0,0);
        uint16_t lastClientPort = 0;
    };

    static BACnetDriver& getInstance() {
        static BACnetDriver instance;
        return instance;
    }

    bool begin(uint16_t udpPort = BACNET_DEFAULT_UDP_PORT);
    void end();
    void task(uint8_t maxPacketsPerCycle = 4);
    bool isRunning() const { return _running; }
    uint16_t udpPort() const { return _udpPort; }

    void setDeviceInfo(const DeviceInfo& info);
    const DeviceInfo& getDeviceInfo() const { return _device; }

    void setRefreshIntervalMs(uint16_t ms) { _refreshIntervalMs = ms; }
    uint16_t getRefreshIntervalMs() const { return _refreshIntervalMs; }
    void setPeriodicIAm(bool enabled, uint16_t sec) { _periodicIAmEnabled = enabled; _periodicIAmSec = sec; }

    // Generic object registration/update (instances are BACnet instances, not array indexes)
    void updateAnalogInput(uint32_t instance, float value, uint16_t units, const char* name, const char* description);
    void updateBinaryInput(uint32_t instance, bool value, const char* name, const char* description);
    void updateBinaryOutput(uint32_t instance, bool value, const char* name, const char* description);

    // Pending BO commands from BACnet WriteProperty
    bool getBinaryOutputCommand(uint32_t instance, bool& value);

    // Device vendor properties (string/uint/bool represented as strings or integers)
    void setVendorPropertyString(uint32_t propId, const String& value);
    void setVendorPropertyUnsigned(uint32_t propId, uint32_t value);
    void setVendorPropertyBool(uint32_t propId, bool value);

    const RuntimeStats& getStats() const { return _stats; }

private:
    BACnetDriver();
    BACnetDriver(const BACnetDriver&) = delete;
    BACnetDriver& operator=(const BACnetDriver&) = delete;

    struct Obj {
        uint8_t type = 0;
        uint32_t instance = 0;
        char name[40] = {0};
        char description[96] = {0};
        float presentValue = 0.0f;
        uint16_t units = BACNET_UNITS_NO_UNITS;
        bool outOfService = false;
        bool used = false;
        uint32_t lastUpdateMs = 0;
    };

    struct BOCommand {
        bool pending = false;
        bool value = false;
    };

    struct VendorProp {
        uint32_t propId = 0;
        bool used = false;
        bool isString = true;
        String s;
        uint32_t u = 0;
    };

    static constexpr uint8_t MAX_AI = 96;
    static constexpr uint8_t MAX_BI = 64;
    static constexpr uint8_t MAX_BO = 64;
    static constexpr uint8_t MAX_VENDOR_PROPS = 48;

    Obj* upsertObj(Obj* arr, size_t len, uint8_t type, uint32_t instance, const char* name, const char* desc, uint16_t units);
    Obj* findObj(uint8_t type, uint32_t instance);
    const Obj* findObjConst(uint8_t type, uint32_t instance) const;
    int countAllObjects() const;
    bool getObjectByFlatIndex(int idx, uint8_t& type, uint32_t& inst) const;

    VendorProp* upsertVendorProp(uint32_t propId);
    const VendorProp* findVendorProp(uint32_t propId) const;

    void handleIncomingPacket(const uint8_t* pkt, uint16_t len, IPAddress rip, uint16_t rport);
    void handleWhoIs(IPAddress rip, uint16_t rport);
    void handleReadProperty(uint8_t invokeId, const uint8_t* apdu, uint16_t apduLen, IPAddress rip, uint16_t rport);
    void handleWriteProperty(uint8_t invokeId, const uint8_t* apdu, uint16_t apduLen, IPAddress rip, uint16_t rport);

    // BACnet encoding helpers
    uint16_t encodeObjectId(uint8_t* out, uint8_t objectType, uint32_t instance);
    uint16_t encodeAppNull(uint8_t* out);
    uint16_t encodeAppBoolean(uint8_t* out, bool value);
    uint16_t encodeAppUnsigned(uint8_t* out, uint32_t value);
    uint16_t encodeAppEnumerated(uint8_t* out, uint32_t value);
    uint16_t encodeAppReal(uint8_t* out, float value);
    uint16_t encodeAppCharacterString(uint8_t* out, const char* s);
    uint16_t encodeAppObjectId(uint8_t* out, uint8_t objectType, uint32_t instance);
    uint16_t encodeContextTagHeader(uint8_t* out, uint8_t tagNo, uint16_t len);
    uint16_t encodeContextUnsigned(uint8_t* out, uint8_t tagNo, uint32_t value);
    uint16_t encodeContextObjectId(uint8_t* out, uint8_t tagNo, uint8_t objType, uint32_t inst);
    uint16_t encodeOpeningTag(uint8_t* out, uint8_t tagNo);
    uint16_t encodeClosingTag(uint8_t* out, uint8_t tagNo);

    bool decodeObjectId(const uint8_t* buf, uint16_t len, uint8_t& objectType, uint32_t& instance, uint16_t& consumed);
    bool decodeContextUnsigned(uint8_t expectedTag, const uint8_t* buf, uint16_t len, uint32_t& value, uint16_t& consumed);
    bool decodeAnyValueToBool(const uint8_t* buf, uint16_t len, bool& value, uint16_t& consumed);

    void sendIAm(IPAddress rip, uint16_t rport, bool broadcast);
    void sendSimpleAck(uint8_t invokeId, uint8_t service, IPAddress rip, uint16_t rport);
    void sendError(uint8_t invokeId, uint8_t service, uint8_t errorClass, uint8_t errorCode, IPAddress rip, uint16_t rport);
    void sendReadPropertyAck(uint8_t invokeId, uint8_t objType, uint32_t objInstance, uint32_t propId,
                             bool hasArrayIndex, uint32_t arrayIndex,
                             const uint8_t* appData, uint16_t appLen, IPAddress rip, uint16_t rport);

    bool sendPacket(const uint8_t* npduApdu, uint16_t len, IPAddress rip, uint16_t rport, bool broadcast);
    bool ensureTransport();

    // duplicate request cache (single-entry)
    uint32_t hashPacket(const uint8_t* data, uint16_t len) const;
    bool tryResendDuplicate(IPAddress rip, uint16_t rport, uint8_t invokeId, uint8_t serviceChoice, uint32_t reqHash);
    void cacheLastResponse(IPAddress rip, uint16_t rport, uint8_t invokeId, uint8_t serviceChoice, uint32_t reqHash, const uint8_t* data, uint16_t len);

    Obj _ai[MAX_AI];
    Obj _bi[MAX_BI];
    Obj _bo[MAX_BO];
    BOCommand _boCommands[MAX_BO];
    VendorProp _vendorProps[MAX_VENDOR_PROPS];

    DeviceInfo _device;
    RuntimeStats _stats;

    bool _running = false;
    uint16_t _udpPort = BACNET_DEFAULT_UDP_PORT;
    uint8_t _rx[BACNET_RX_BUFFER_SIZE];
    uint8_t _tx[BACNET_TX_BUFFER_SIZE];
    uint32_t _lastIAmMs = 0;
    bool _periodicIAmEnabled = true;
    uint16_t _periodicIAmSec = 60;
    uint16_t _refreshIntervalMs = 500;

    // duplicate cache
    uint32_t _lastReqHash = 0;
    uint8_t _lastReqInvokeId = 0;
    uint8_t _lastReqService = 0;
    IPAddress _lastReqIP = IPAddress(0,0,0,0);
    uint16_t _lastReqPort = 0;
    uint16_t _lastRespLen = 0;
    uint8_t _lastResp[BACNET_TX_BUFFER_SIZE];
};

