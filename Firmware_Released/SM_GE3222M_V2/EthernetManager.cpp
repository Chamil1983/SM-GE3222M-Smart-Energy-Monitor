#include "EthernetManager.h"

#include "Logger.h"
#include "SPIBus.h"
#include <WiFi.h>

// --- DHCP begin helper: use timeout-capable Ethernet.begin() when available (varies by Ethernet library version) ---
template <typename T>
static auto ethernetBeginDhcp(T& eth, uint8_t* mac, unsigned long timeoutMs, unsigned long responseTimeoutMs, int)
    -> decltype(eth.begin(mac, timeoutMs, responseTimeoutMs), int()) {
    return eth.begin(mac, timeoutMs, responseTimeoutMs);
}
template <typename T>
static int ethernetBeginDhcp(T& eth, uint8_t* mac, unsigned long, unsigned long, long) {
    return eth.begin(mac);
}

static inline bool ipNonZero(const IPAddress& ip) {
    return !(ip[0] == 0 && ip[1] == 0 && ip[2] == 0 && ip[3] == 0);
}

static inline bool wifiApModeActive() {
#if defined(ARDUINO_ARCH_ESP32)
    wifi_mode_t m = WiFi.getMode();
    return (m == WIFI_MODE_AP || m == WIFI_MODE_APSTA);
#else
    return false;
#endif
}

static inline int wifiApStationCountSafe() {
#if defined(ARDUINO_ARCH_ESP32)
    return WiFi.softAPgetStationNum();
#else
    return 0;
#endif
}

static uint32_t s_lastDhcpDeferralLogMs = 0;

bool EthernetManager::parseMacString(const char* macStr, uint8_t outMac[6]) {
    if (!macStr || !outMac) return false;
    int v[6];
    if (sscanf(macStr, "%x:%x:%x:%x:%x:%x", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]) != 6) {
        return false;
    }
    for (int i = 0; i < 6; ++i) outMac[i] = (uint8_t)v[i];
    return true;
}

String EthernetManager::macToString(const uint8_t mac[6]) {
    char b[18];
    snprintf(b, sizeof(b), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(b);
}

String EthernetManager::ipToString(const IPAddress& ip) {
    char b[16];
    snprintf(b, sizeof(b), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    return String(b);
}

String EthernetManager::localIPString() const { return ipToString(_status.ip); }
String EthernetManager::subnetString() const { return ipToString(_status.subnet); }
String EthernetManager::gatewayString() const { return ipToString(_status.gateway); }
String EthernetManager::dnsString() const { return ipToString(_status.dns1); }
String EthernetManager::macAddressString() const { return macToString(_status.mac); }

bool EthernetManager::begin() {
    Config c;
    return begin(c);
}

bool EthernetManager::begin(const Config& cfg) {
    _cfg = cfg;
    _status = Status();
    _status.enabled = cfg.enabled;
    _status.dhcpActive = cfg.useDHCP;
    memcpy(_status.mac, cfg.mac, 6);
    _status.ip = IPAddress(0,0,0,0);
    _status.subnet = IPAddress(0,0,0,0);
    _status.gateway = IPAddress(0,0,0,0);
    _status.dns1 = IPAddress(0,0,0,0);
    _linkChangedFlag = false;
    _ipChangedFlag = false;
    _begun = false;
    _recoverAttempts = 0;

    if (!_cfg.enabled) {
        Logger::getInstance().info("EthernetManager: Disabled by config");
        return false;
    }

    if (!SPIBus::getInstance().isInitialized()) {
        SPIBus::getInstance().init();
    }

    // Mark as begun even if first probe fails so update() can retry hardware detection later.
    _begun = true;

    if (!initHardware()) {
        _status.errorCount++;
        Logger::getInstance().warn("EthernetManager: Hardware probe failed on first attempt (will retry in update)");
        return false;
    }

    // Do NOT run DHCP/static configuration in begin(); it can block boot for a long time
    // if Ethernet link is down or DHCP is unavailable. Only probe hardware here and
    // defer IP configuration to update()/recoverIfNeeded() after boot progresses.
    refreshStatusFromStack(true);
    if (_status.linkUp) {
        Logger::getInstance().info("EthernetManager: Link is UP; scheduling background IP config (DHCP/static retry)");
    } else {
        Logger::getInstance().info("EthernetManager: Link is DOWN; DHCP/static config deferred");
    }
    return _status.initialized;
}

void EthernetManager::end() {
    closeBacnetUdp();
    _status.initialized = false;
    _status.ipReady = false;
    _status.linkUp = false;
}

bool EthernetManager::initHardware() {
    Logger::getInstance().info("EthernetManager: Init W5500 SPI pins MISO=%u MOSI=%u SCK=%u CS=%u",
                               PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SCK, PIN_ETH_CS);

    SPIBus::getInstance().reinit();

    // Ensure *all* SPI chip-selects on the shared bus are deasserted before probing W5500.
    pinMode(PIN_SPI_CS_ATM90E36, OUTPUT);
    digitalWrite(PIN_SPI_CS_ATM90E36, HIGH);
    pinMode(PIN_ETH_CS, OUTPUT);
    digitalWrite(PIN_ETH_CS, HIGH);
    delay(20);

    Ethernet.init(PIN_ETH_CS);

    auto probeOnce = [this]() -> bool {
        bool locked = SPIBus::getInstance().lock(300);
        if (!locked) {
            Logger::getInstance().error("EthernetManager: SPI lock timeout during initHardware probe");
            return false;
        }
        // Prime the driver a little; some builds return NoHardware on the first immediate query.
        (void)Ethernet.linkStatus();
        auto hw = Ethernet.hardwareStatus();
        SPIBus::getInstance().unlock();
        _status.hardwareDetected = (hw != EthernetNoHardware);
        return _status.hardwareDetected;
    };

    bool detected = false;
    for (uint8_t attempt = 1; attempt <= 3; ++attempt) {
        detected = probeOnce();
        if (detected) break;
        Logger::getInstance().warn("EthernetManager: W5500 probe attempt %u failed", (unsigned)attempt);
        delay(25);
        SPIBus::getInstance().reinit();
        digitalWrite(PIN_SPI_CS_ATM90E36, HIGH);
        digitalWrite(PIN_ETH_CS, HIGH);
        Ethernet.init(PIN_ETH_CS);
    }

    if (!detected) {
        Logger::getInstance().warn("EthernetManager: No Ethernet hardware detected (check power/SPI/CS wiring and shared SPI contention)");
        _status.initialized = false;
        return false;
    }

    _status.initialized = true;
    Logger::getInstance().info("EthernetManager: W5500 hardware detected");
    return true;
}


bool EthernetManager::configureNetwork() {
    if (!_status.hardwareDetected) return false;

    // Avoid blocking DHCP/static configuration when link is down.
    refreshStatusFromStack(false);
    if (!_status.linkUp) {
        Logger::getInstance().warn("EthernetManager: Link down - skipping IP configuration retry");
        _status.ipReady = false;
        return false;
    }

    const uint32_t nowMs = millis();
    if (_cfg.useDHCP) {
        const bool apMode = wifiApModeActive();
        if (apMode) {
            // Grace period after AP bring-up keeps setup SSID stable/visible before any potentially blocking W5500 DHCP attempt.
            if (nowMs < _cfg.apModeDhcpGraceMs) {
                if ((uint32_t)(nowMs - s_lastDhcpDeferralLogMs) >= 5000UL || s_lastDhcpDeferralLogMs == 0) {
                    Logger::getInstance().info("EthernetManager: AP-mode DHCP waiting for grace period (%lu/%lu ms)",
                                               (unsigned long)nowMs, (unsigned long)_cfg.apModeDhcpGraceMs);
                    s_lastDhcpDeferralLogMs = nowMs;
                }
                return false;
            }
            if (_cfg.skipDhcpWhileWifiApMode) {
                if ((uint32_t)(nowMs - s_lastDhcpDeferralLogMs) >= 10000UL || s_lastDhcpDeferralLogMs == 0) {
                    Logger::getInstance().warn("EthernetManager: DHCP deferred while WiFi AP mode is active");
                    s_lastDhcpDeferralLogMs = nowMs;
                }
                return false;
            }
            if (_cfg.skipDhcpWhenApClientConnected) {
                int staCount = wifiApStationCountSafe();
                if (staCount > 0) {
                    if ((uint32_t)(nowMs - s_lastDhcpDeferralLogMs) >= 10000UL || s_lastDhcpDeferralLogMs == 0) {
                        Logger::getInstance().warn("EthernetManager: DHCP deferred while AP client connected (stations=%d)", staCount);
                        s_lastDhcpDeferralLogMs = nowMs;
                    }
                    return false;
                }
            }
        }
        if (_status.lastDhcpMs != 0 && (uint32_t)(nowMs - _status.lastDhcpMs) < _cfg.minDhcpRetryMs) {
            return false;
        }
        // Allow PHY to settle briefly after link-up to reduce false DHCP failures.
        if (_status.lastLinkChangeMs != 0 && (uint32_t)(nowMs - _status.lastLinkChangeMs) < 1200UL) {
            return false;
        }
    }

    SPIBus::getInstance().reinit();
    bool locked = SPIBus::getInstance().lock(1500);
    if (!locked) {
        Logger::getInstance().error("EthernetManager: SPI lock timeout during configureNetwork");
        return false;
    }

    // Re-prime shared SPI before long W5500 DHCP/static operation.
    pinMode(PIN_SPI_CS_ATM90E36, OUTPUT);
    digitalWrite(PIN_SPI_CS_ATM90E36, HIGH);
    pinMode(PIN_ETH_CS, OUTPUT);
    digitalWrite(PIN_ETH_CS, HIGH);
    delay(2);

    int rc = 0;
    if (_cfg.useDHCP) {
        _status.lastDhcpMs = millis();
        Logger::getInstance().info("EthernetManager: Starting DHCP request...");
        Logger::getInstance().info("EthernetManager: DHCP timeouts: total=%lu ms, resp=%lu ms", (unsigned long)_cfg.dhcpTimeoutMs, (unsigned long)_cfg.dhcpResponseTimeoutMs);
        rc = ethernetBeginDhcp(Ethernet, _cfg.mac, _cfg.dhcpTimeoutMs, _cfg.dhcpResponseTimeoutMs, 0);
    } else {
        Logger::getInstance().info("EthernetManager: Applying static IP config...");
        Ethernet.begin(_cfg.mac, _cfg.ip, _cfg.dns1, _cfg.gateway, _cfg.subnet);
        rc = 1;
    }
    SPIBus::getInstance().unlock();

    if (_cfg.useDHCP && rc == 0) {
        Logger::getInstance().warn("EthernetManager: DHCP failed");
        refreshStatusFromStack(true);
        return false;
    }

    refreshStatusFromStack(true);
    _recoverAttempts = 0;
    s_lastDhcpDeferralLogMs = 0;
    Logger::getInstance().info("EthernetManager: %s IP=%s",
        _cfg.useDHCP ? "DHCP configured" : "Static configured", localIPString().c_str());
    return true;
}

bool EthernetManager::reconfigure() {
    closeBacnetUdp();
    _status.lastRecoveryMs = millis();
    _status.recoveryCount++;
    Logger::getInstance().warn("EthernetManager: Reconfiguring Ethernet (attempt %u)", (unsigned)_status.recoveryCount);
    return configureNetwork();
}

void EthernetManager::refreshStatusFromStack(bool markChanges) {
    if (!_status.hardwareDetected) return;

    IPAddress oldIp = _status.ip;
    bool oldLink = _status.linkUp;

    bool locked = SPIBus::getInstance().lock(100);
    if (!locked) return;

    auto link = Ethernet.linkStatus();
    _status.linkUp = (link == LinkON);

    _status.ip = Ethernet.localIP();
    _status.subnet = Ethernet.subnetMask();
    _status.gateway = Ethernet.gatewayIP();
    _status.dns1 = Ethernet.dnsServerIP();
    _status.ipReady = ipNonZero(_status.ip);
    _status.lastLoopMs = millis();
    SPIBus::getInstance().unlock();

    if (markChanges) {
        if (oldLink != _status.linkUp) {
            _status.lastLinkChangeMs = millis();
            _linkChangedFlag = true;
            if (!_status.linkUp && oldLink) _status.linkDropCount++;
            Logger::getInstance().info("EthernetManager: Link %s", _status.linkUp ? "UP" : "DOWN");
        }
        if (oldIp != _status.ip) {
            _status.lastIpChangeMs = millis();
            _ipChangedFlag = true;
            Logger::getInstance().info("EthernetManager: IP %s", localIPString().c_str());
        }
    }
}

void EthernetManager::recoverIfNeeded() {
    if (!_cfg.enabled) return;

    const uint32_t now = millis();
    const bool timeForRetry = (_status.lastRecoveryMs == 0) || ((uint32_t)(now - _status.lastRecoveryMs) >= _cfg.recoverRetryMs);

    // If hardware was not detected during boot, keep probing in the background.
    if (!_status.hardwareDetected) {
        if (timeForRetry) {
            _status.lastRecoveryMs = now;
            if (initHardware()) {
                refreshStatusFromStack(true);
            }
        }
        return;
    }

    // Always refresh link state before deciding on DHCP/static retry.
    refreshStatusFromStack(false);

    // If link is down, never attempt DHCP/static config (prevents long blocking calls during boot/runtime).
    if (!_status.linkUp) {
        if (_status.udpBound) {
            closeBacnetUdp();
        }
        return;
    }

    // Reconfigure if link up but no valid IP.
    if (!_status.ipReady && timeForRetry) {
        if (_recoverAttempts < _cfg.maxRecoverAttemptsBeforeBackoff) {
            _status.lastRecoveryMs = now;
            _recoverAttempts++;
            reconfigure();
        } else {
            // Backoff window: retry less frequently after repeated failures.
            if ((uint32_t)(now - _status.lastRecoveryMs) >= (_cfg.recoverRetryMs * 3UL)) {
                _recoverAttempts = 0;
            }
        }
    }

    // If BACnet UDP bound but link/IP lost, mark it closed.
    if (_status.udpBound && (!_status.linkUp || !_status.ipReady)) {
        closeBacnetUdp();
    }
}

void EthernetManager::update() {
    if (!_cfg.enabled || !_begun) return;
    const uint32_t now = millis();
    if (_lastLinkPollMs == 0 || (uint32_t)(now - _lastLinkPollMs) >= _cfg.linkPollMs) {
        _lastLinkPollMs = now;
        refreshStatusFromStack(true);
        recoverIfNeeded();
    }
}

bool EthernetManager::hasLinkChanged() {
    bool v = _linkChangedFlag;
    _linkChangedFlag = false;
    return v;
}

bool EthernetManager::hasIpChanged() {
    bool v = _ipChangedFlag;
    _ipChangedFlag = false;
    return v;
}

bool EthernetManager::ensureBacnetUdpBound(uint16_t port) {
    if (!_cfg.enabled || !_status.hardwareDetected || !_status.ipReady) return false;
    if (_status.udpBound && _status.udpPort == port) return true;

    closeBacnetUdp();

    bool locked = SPIBus::getInstance().lock(300);
    if (!locked) return false;
    bool ok = _bacnetUdp.begin(port);
    SPIBus::getInstance().unlock();

    if (ok) {
        _status.udpBound = true;
        _status.udpPort = port;
        _status.udpRebindCount++;
        Logger::getInstance().info("EthernetManager: BACnet UDP bound port %u", (unsigned)port);
    } else {
        _status.errorCount++;
        Logger::getInstance().warn("EthernetManager: BACnet UDP bind failed port %u", (unsigned)port);
    }
    return ok;
}

void EthernetManager::closeBacnetUdp() {
    if (!_status.udpBound) return;
    bool locked = SPIBus::getInstance().lock(200);
    if (locked) {
        _bacnetUdp.stop();
        SPIBus::getInstance().unlock();
    }
    _status.udpBound = false;
    Logger::getInstance().info("EthernetManager: BACnet UDP closed");
}

int EthernetManager::bacnetParsePacket() {
    if (!_status.udpBound) return 0;
    bool locked = SPIBus::getInstance().lock(50);
    if (!locked) return 0;
    int n = _bacnetUdp.parsePacket();
    SPIBus::getInstance().unlock();
    return n;
}

int EthernetManager::bacnetRead(uint8_t* buffer, size_t size) {
    if (!_status.udpBound) return 0;
    bool locked = SPIBus::getInstance().lock(50);
    if (!locked) return 0;
    int n = _bacnetUdp.read(buffer, size);
    SPIBus::getInstance().unlock();
    return n;
}

IPAddress EthernetManager::bacnetRemoteIP() {
    return _bacnetUdp.remoteIP();
}

uint16_t EthernetManager::bacnetRemotePort() {
    return _bacnetUdp.remotePort();
}

bool EthernetManager::bacnetSend(IPAddress remoteIP, uint16_t remotePort, const uint8_t* data, size_t len) {
    if (!_status.udpBound || data == nullptr || len == 0) return false;
    bool locked = SPIBus::getInstance().lock(100);
    if (!locked) return false;
    bool ok = (_bacnetUdp.beginPacket(remoteIP, remotePort) == 1);
    if (ok) {
        ok = (_bacnetUdp.write(data, len) == len) && (_bacnetUdp.endPacket() == 1);
    } else {
        _bacnetUdp.stop();
    }
    SPIBus::getInstance().unlock();
    if (!ok) _status.errorCount++;
    return ok;
}

