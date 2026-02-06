#include "EthernetManager.h"
#include "../core/ErrorHandler.h"

// Singleton instance
EthernetManager& EthernetManager::getInstance() {
    static EthernetManager instance;
    return instance;
}

EthernetManager::EthernetManager()
    : initialized_(false),
      usingDHCP_(false),
      linkUp_(false),
      lastLinkCheck_(0),
      lastDHCPCheck_(0),
      linkCallback_(nullptr) {
    // Initialize MAC address with default
    macAddress_[0] = 0xDE;
    macAddress_[1] = 0xAD;
    macAddress_[2] = 0xBE;
    macAddress_[3] = 0xEF;
    macAddress_[4] = 0xFE;
    macAddress_[5] = 0xED;
}

bool EthernetManager::init() {
    if (initialized_) {
        return true;
    }
    
    // Get SPI bus instance
    SPIBus& spiBus = SPIBus::getInstance();
    
    // W5500 requires SPI to be initialized
    if (!spiBus.isInitialized()) {
        ErrorHandler::getInstance().logError(ErrorCode::SPI_INIT_FAILED, "SPI bus not initialized");
        return false;
    }
    
    // Set Ethernet CS pin
    Ethernet.init(PIN_SPI_CS_W5500);
    
    initialized_ = true;
    ErrorHandler::getInstance().logInfo("EthernetManager initialized");
    return true;
}

bool EthernetManager::begin(uint8_t* mac, IPAddress ip, IPAddress dns, 
                            IPAddress gateway, IPAddress subnet) {
    if (!initialized_) {
        ErrorHandler::getInstance().logError(ErrorCode::ETHERNET_INIT_FAILED, "Ethernet not initialized");
        return false;
    }
    
    memcpy(macAddress_, mac, 6);
    
    Ethernet.begin(mac, ip, dns, gateway, subnet);
    usingDHCP_ = false;
    
    // Wait for link
    delay(1000);
    
    if (Ethernet.linkStatus() == LinkON) {
        linkUp_ = true;
        ErrorHandler::getInstance().logInfo("Ethernet connected (Static IP): " + 
                                          Ethernet.localIP().toString());
        return true;
    }
    
    ErrorHandler::getInstance().logWarning("Ethernet link not detected");
    return false;
}

bool EthernetManager::beginDHCP(uint8_t* mac, unsigned long timeout) {
    if (!initialized_) {
        ErrorHandler::getInstance().logError(ErrorCode::ETHERNET_INIT_FAILED, "Ethernet not initialized");
        return false;
    }
    
    memcpy(macAddress_, mac, 6);
    
    ErrorHandler::getInstance().logInfo("Starting Ethernet with DHCP...");
    
    if (Ethernet.begin(mac, timeout) == 0) {
        ErrorHandler::getInstance().logError(ErrorCode::ETHERNET_DHCP_FAILED, "DHCP failed");
        return false;
    }
    
    usingDHCP_ = true;
    
    // Check link status
    if (Ethernet.linkStatus() == LinkON) {
        linkUp_ = true;
        ErrorHandler::getInstance().logInfo("Ethernet connected (DHCP): " + 
                                          Ethernet.localIP().toString());
        return true;
    }
    
    ErrorHandler::getInstance().logWarning("Ethernet link not detected");
    return false;
}

int EthernetManager::maintain() {
    if (!usingDHCP_) {
        return 0; // Not using DHCP
    }
    
    int result = Ethernet.maintain();
    
    switch (result) {
        case 0:
            // Nothing happened
            break;
            
        case 1:
            // Renew failed
            ErrorHandler::getInstance().logError(ErrorCode::ETHERNET_DHCP_FAILED, "DHCP renew failed");
            break;
            
        case 2:
            // Renew success
            ErrorHandler::getInstance().logInfo("DHCP lease renewed: " + Ethernet.localIP().toString());
            break;
            
        case 3:
            // Rebind failed
            ErrorHandler::getInstance().logError(ErrorCode::ETHERNET_DHCP_FAILED, "DHCP rebind failed");
            break;
            
        case 4:
            // Rebind success
            ErrorHandler::getInstance().logInfo("DHCP lease rebound: " + Ethernet.localIP().toString());
            break;
    }
    
    return result;
}

bool EthernetManager::isLinked() const {
    return linkUp_ && Ethernet.linkStatus() == LinkON;
}

bool EthernetManager::isConfigured() const {
    return initialized_ && (Ethernet.localIP() != IPAddress(0, 0, 0, 0));
}

IPAddress EthernetManager::getIP() const {
    return Ethernet.localIP();
}

IPAddress EthernetManager::getSubnet() const {
    return Ethernet.subnetMask();
}

IPAddress EthernetManager::getGateway() const {
    return Ethernet.gatewayIP();
}

IPAddress EthernetManager::getDNS() const {
    return Ethernet.dnsServerIP();
}

String EthernetManager::getMACAddress() const {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             macAddress_[0], macAddress_[1], macAddress_[2],
             macAddress_[3], macAddress_[4], macAddress_[5]);
    return String(macStr);
}

void EthernetManager::setMACAddress(uint8_t* mac) {
    memcpy(macAddress_, mac, 6);
}

void EthernetManager::update() {
    unsigned long now = millis();
    
    // Check link status periodically
    if (now - lastLinkCheck_ >= LINK_CHECK_INTERVAL) {
        lastLinkCheck_ = now;
        checkLinkStatus();
    }
    
    // Maintain DHCP lease if using DHCP
    if (usingDHCP_ && now - lastDHCPCheck_ >= DHCP_CHECK_INTERVAL) {
        lastDHCPCheck_ = now;
        maintain();
    }
}

void EthernetManager::setLinkCallback(void (*callback)(bool linked)) {
    linkCallback_ = callback;
}

// Private methods

void EthernetManager::checkLinkStatus() {
    bool newLinkStatus = (Ethernet.linkStatus() == LinkON);
    
    if (newLinkStatus != linkUp_) {
        linkUp_ = newLinkStatus;
        
        if (linkUp_) {
            ErrorHandler::getInstance().logInfo("Ethernet link up");
        } else {
            ErrorHandler::getInstance().logWarning("Ethernet link down");
        }
        
        // Call callback if registered
        if (linkCallback_ != nullptr) {
            linkCallback_(linkUp_);
        }
    }
}
