#pragma once

#include <Ethernet.h>
#include <IPAddress.h>
#include "../../include/PinMap.h"
#include "../hal/SPIBus.h"

/**
 * @brief EthernetManager - Singleton class for managing W5500 Ethernet connectivity
 * 
 * Features:
 * - W5500 Ethernet controller via SPI
 * - DHCP or static IP configuration
 * - Link status monitoring
 * - DHCP lease maintenance
 */
class EthernetManager {
public:
    static EthernetManager& getInstance();
    
    // Initialize W5500 controller
    bool init();
    
    // Start Ethernet with static IP
    bool begin(uint8_t* mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet);
    
    // Start Ethernet with DHCP
    bool beginDHCP(uint8_t* mac, unsigned long timeout = 60000);
    
    // Maintain DHCP lease (call periodically)
    int maintain();
    
    // Status queries
    bool isLinked() const;
    bool isConfigured() const;
    IPAddress getIP() const;
    IPAddress getSubnet() const;
    IPAddress getGateway() const;
    IPAddress getDNS() const;
    String getMACAddress() const;
    void setMACAddress(uint8_t* mac);
    
    // Main loop update
    void update();
    
    // Link change callback
    void setLinkCallback(void (*callback)(bool linked));
    
private:
    EthernetManager();
    ~EthernetManager() = default;
    EthernetManager(const EthernetManager&) = delete;
    EthernetManager& operator=(const EthernetManager&) = delete;
    
    // Link status monitoring
    void checkLinkStatus();
    
    bool initialized_;
    bool usingDHCP_;
    bool linkUp_;
    unsigned long lastLinkCheck_;
    unsigned long lastDHCPCheck_;
    
    uint8_t macAddress_[6];
    
    void (*linkCallback_)(bool linked);
    
    static constexpr unsigned long LINK_CHECK_INTERVAL = 1000;  // 1 second
    static constexpr unsigned long DHCP_CHECK_INTERVAL = 60000; // 1 minute
};
