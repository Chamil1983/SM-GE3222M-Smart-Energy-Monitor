#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "PinMap.h"

class EthernetManager {
public:
    struct Config {
        bool enabled = true;
        bool useDHCP = true;
        uint8_t mac[6] = {0xC8,0x2E,0xA3,0xF5,0x7D,0xDA};
        IPAddress ip = IPAddress(192,168,1,200);
        IPAddress subnet = IPAddress(255,255,255,0);
        IPAddress gateway = IPAddress(192,168,1,1);
        IPAddress dns1 = IPAddress(8,8,8,8);
        IPAddress dns2 = IPAddress(8,8,4,4);
        uint32_t linkPollMs = 500;
        uint32_t recoverRetryMs = 3000;
        uint8_t maxRecoverAttemptsBeforeBackoff = 3;
        bool skipDhcpWhileWifiApMode = false;
        bool skipDhcpWhenApClientConnected = true;  // keep AP/Web responsive while PC/phone is connected
        uint32_t apModeDhcpGraceMs = 4000;  // warmup before DHCP attempts
        uint32_t minDhcpRetryMs = 12000; // retry throttling
        uint32_t dhcpTimeoutMs = 2500;       // total DHCP timeout (ms)
        uint32_t dhcpResponseTimeoutMs = 600; // per-offer response timeout (ms)
    };

    struct Status {
        bool enabled = false;
        bool initialized = false;
        bool hardwareDetected = false;
        bool linkUp = false;
        bool ipReady = false;
        bool udpBound = false;
        bool dhcpActive = true;
        uint16_t recoveryCount = 0;
        uint16_t linkDropCount = 0;
        uint16_t udpRebindCount = 0;
        uint16_t errorCount = 0;
        uint32_t lastLinkChangeMs = 0;
        uint32_t lastIpChangeMs = 0;
        uint32_t lastRecoveryMs = 0;
        uint32_t lastDhcpMs = 0;
        uint32_t lastLoopMs = 0;
        uint16_t udpPort = 0;
        IPAddress ip;
        IPAddress subnet;
        IPAddress gateway;
        IPAddress dns1;
        uint8_t mac[6] = {0};
    };

    static EthernetManager& getInstance() {
        static EthernetManager instance;
        return instance;
    }

    bool begin(const Config& cfg);
    bool begin();
    void end();
    void update();

    bool isEnabled() const { return _cfg.enabled; }
    bool isInitialized() const { return _status.initialized; }
    bool isHardwareDetected() const { return _status.hardwareDetected; }
    bool isLinkUp() const { return _status.linkUp; }
    bool isIpReady() const { return _status.ipReady; }
    const Config& getConfig() const { return _cfg; }
    const Status& getStatus() const { return _status; }

    IPAddress localIP() const { return _status.ip; }
    IPAddress subnetMask() const { return _status.subnet; }
    IPAddress gatewayIP() const { return _status.gateway; }
    IPAddress dnsServerIP() const { return _status.dns1; }
    String localIPString() const;
    String subnetString() const;
    String gatewayString() const;
    String dnsString() const;
    String macAddressString() const;

    bool hasLinkChanged();              // edge-triggered consume
    bool hasIpChanged();                // edge-triggered consume

    // BACnet UDP wrappers (SPI-bus-safe)
    bool ensureBacnetUdpBound(uint16_t port);
    void closeBacnetUdp();
    int bacnetParsePacket();
    int bacnetRead(uint8_t* buffer, size_t size);
    IPAddress bacnetRemoteIP();
    uint16_t bacnetRemotePort();
    bool bacnetSend(IPAddress remoteIP, uint16_t remotePort, const uint8_t* data, size_t len);

    static bool parseMacString(const char* macStr, uint8_t outMac[6]);
    static String macToString(const uint8_t mac[6]);
    static String ipToString(const IPAddress& ip);

private:
    EthernetManager() = default;
    EthernetManager(const EthernetManager&) = delete;
    EthernetManager& operator=(const EthernetManager&) = delete;

    bool initHardware();
    bool configureNetwork();
    void refreshStatusFromStack(bool markChanges = true);
    void recoverIfNeeded();
    bool reconfigure();

    Config _cfg;
    Status _status;
    EthernetUDP _bacnetUdp;
    bool _linkChangedFlag = false;
    bool _ipChangedFlag = false;
    bool _begun = false;
    uint32_t _lastLinkPollMs = 0;
    uint8_t _recoverAttempts = 0;
};

