/**
 * @file LCDManager.cpp
 * @brief 20x4 LCD display management implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "LCDManager.h"
#include "../../include/Version.h"
#include "../core/ErrorHandler.h"

// Custom character definitions
static const uint8_t degreeChar[8] = {0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00};
static const uint8_t arrowChar[8] = {0x00, 0x04, 0x02, 0x1F, 0x02, 0x04, 0x00, 0x00};
static const uint8_t wifiChar[8] = {0x00, 0x0E, 0x11, 0x04, 0x0A, 0x00, 0x04, 0x00};
static const uint8_t checkChar[8] = {0x00, 0x01, 0x03, 0x16, 0x1C, 0x08, 0x00, 0x00};

LCDManager::LCDManager() 
    : lcd(I2C_ADDR_LCD, 20, 4),
      currentPage(DisplayPage::SPLASH),
      initialized(false),
      autoScrollEnabled(false),
      autoScrollInterval(5000),
      lastScrollTime(0),
      networkRSSI(0),
      networkMQTT(false),
      systemUptime(0),
      systemTemp(0),
      systemFreeHeap(0),
      systemErrorCount(0),
      lastUpdateTime(0),
      needsRedraw(true) {
    strcpy(networkIP, "0.0.0.0");
}

LCDManager& LCDManager::getInstance() {
    static LCDManager instance;
    return instance;
}

bool LCDManager::init() {
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    
    // Create custom characters
    initCustomCharacters();
    
    initialized = true;
    needsRedraw = true;
    currentPage = DisplayPage::SPLASH;
    
    return true;
}

void LCDManager::initCustomCharacters() {
    lcd.createChar(CHAR_DEGREE, (uint8_t*)degreeChar);
    lcd.createChar(CHAR_ARROW, (uint8_t*)arrowChar);
    lcd.createChar(CHAR_WIFI, (uint8_t*)wifiChar);
    lcd.createChar(CHAR_CHECK, (uint8_t*)checkChar);
}

void LCDManager::clear() {
    if (!initialized) return;
    lcd.clear();
    needsRedraw = true;
}

void LCDManager::setPage(DisplayPage page) {
    if (page >= DisplayPage::MAX_PAGES) return;
    
    if (currentPage != page) {
        currentPage = page;
        needsRedraw = true;
    }
}

void LCDManager::nextPage() {
    uint8_t nextPageNum = static_cast<uint8_t>(currentPage) + 1;
    if (nextPageNum >= static_cast<uint8_t>(DisplayPage::MAX_PAGES)) {
        nextPageNum = 1; // Skip splash after boot
    }
    setPage(static_cast<DisplayPage>(nextPageNum));
}

void LCDManager::prevPage() {
    uint8_t prevPageNum = static_cast<uint8_t>(currentPage);
    if (prevPageNum <= 1) {
        prevPageNum = static_cast<uint8_t>(DisplayPage::MAX_PAGES);
    }
    prevPageNum--;
    setPage(static_cast<DisplayPage>(prevPageNum));
}

void LCDManager::update() {
    if (!initialized) return;
    
    unsigned long now = millis();
    
    // Handle auto-scroll
    if (autoScrollEnabled && (now - lastScrollTime >= autoScrollInterval)) {
        nextPage();
        lastScrollTime = now;
    }
    
    // Throttle updates to 2Hz
    if (!needsRedraw && (now - lastUpdateTime < 500)) {
        return;
    }
    
    // Render current page
    if (needsRedraw) {
        lcd.clear();
    }
    
    switch (currentPage) {
        case DisplayPage::SPLASH:
            renderSplash();
            break;
        case DisplayPage::PHASE_A:
            renderPhase('A', cachedData.phaseA);
            break;
        case DisplayPage::PHASE_B:
            renderPhase('B', cachedData.phaseB);
            break;
        case DisplayPage::PHASE_C:
            renderPhase('C', cachedData.phaseC);
            break;
        case DisplayPage::TOTALS:
            renderTotals();
            break;
        case DisplayPage::ENERGY:
            renderEnergy();
            break;
        case DisplayPage::NETWORK:
            renderNetwork();
            break;
        case DisplayPage::SYSTEM:
            renderSystem();
            break;
        default:
            break;
    }
    
    needsRedraw = false;
    lastUpdateTime = now;
}

void LCDManager::displayMeterData(const MeterData& data) {
    cachedData = data;
    needsRedraw = true;
}

void LCDManager::setAutoScroll(bool enabled, uint32_t intervalMs) {
    autoScrollEnabled = enabled;
    autoScrollInterval = intervalMs;
    if (enabled) {
        lastScrollTime = millis();
    }
}

void LCDManager::displayMessage(uint8_t line, const char* message) {
    if (!initialized || line > 3) return;
    lcd.setCursor(0, line);
    lcd.print(message);
}

void LCDManager::setNetworkInfo(const char* ip, int rssi, bool mqttConnected) {
    strncpy(networkIP, ip, sizeof(networkIP) - 1);
    networkIP[sizeof(networkIP) - 1] = '\0';
    networkRSSI = rssi;
    networkMQTT = mqttConnected;
}

void LCDManager::setSystemInfo(uint32_t uptime, float temp, uint32_t freeHeap, uint32_t errorCount) {
    systemUptime = uptime;
    systemTemp = temp;
    systemFreeHeap = freeHeap;
    systemErrorCount = errorCount;
}

void LCDManager::renderSplash() {
    lcd.setCursor(0, 0);
    lcd.print("   SM-GE3222M V2.0   ");
    lcd.setCursor(0, 1);
    lcd.print("  Energy Monitor    ");
    lcd.setCursor(0, 2);
    lcd.print("   FW: ");
    lcd.print(FW_VERSION_STRING);
    lcd.print("       ");
    lcd.setCursor(0, 3);
    lcd.print(" Microcode Eng.     ");
}

void LCDManager::renderPhase(char phase, const PhaseData& data) {
    char buffer[21];
    
    // Line 0: Phase header
    lcd.setCursor(0, 0);
    snprintf(buffer, sizeof(buffer), "==== PHASE %c ====", phase);
    lcd.print(buffer);
    
    // Line 1: Voltage and Current
    lcd.setCursor(0, 1);
    snprintf(buffer, sizeof(buffer), "V:%5.1fV  I:%5.2fA", data.voltage, data.current);
    lcd.print(buffer);
    
    // Line 2: Power
    lcd.setCursor(0, 2);
    snprintf(buffer, sizeof(buffer), "P:%6.0fW  PF:%4.2f", data.activePower, data.powerFactor);
    lcd.print(buffer);
    
    // Line 3: Reactive and Apparent
    lcd.setCursor(0, 3);
    snprintf(buffer, sizeof(buffer), "Q:%5.0fVAR S:%5.0fVA", data.reactivePower, data.apparentPower);
    lcd.print(buffer);
}

void LCDManager::renderTotals() {
    char buffer[21];
    
    // Line 0: Header
    lcd.setCursor(0, 0);
    lcd.print("====  TOTALS  ====  ");
    
    // Line 1: Total Active Power
    lcd.setCursor(0, 1);
    snprintf(buffer, sizeof(buffer), "P: %7.0f W", cachedData.totalActivePower);
    lcd.print(buffer);
    
    // Line 2: Total Reactive Power
    lcd.setCursor(0, 2);
    snprintf(buffer, sizeof(buffer), "Q: %7.0f VAR", cachedData.totalReactivePower);
    lcd.print(buffer);
    
    // Line 3: Total Apparent and PF
    lcd.setCursor(0, 3);
    snprintf(buffer, sizeof(buffer), "S:%5.0fVA PF:%5.2f", 
             cachedData.totalApparentPower, cachedData.totalPowerFactor);
    lcd.print(buffer);
}

void LCDManager::renderEnergy() {
    char buffer[21];
    
    // Line 0: Header
    lcd.setCursor(0, 0);
    lcd.print("====  ENERGY  ====  ");
    
    // Line 1: Import Active Energy
    lcd.setCursor(0, 1);
    snprintf(buffer, sizeof(buffer), "Import:%9.2fkWh", cachedData.importActiveEnergy[0]);
    lcd.print(buffer);
    
    // Line 2: Export Active Energy
    lcd.setCursor(0, 2);
    snprintf(buffer, sizeof(buffer), "Export:%9.2fkWh", cachedData.exportActiveEnergy[0]);
    lcd.print(buffer);
    
    // Line 3: Frequency
    lcd.setCursor(0, 3);
    snprintf(buffer, sizeof(buffer), "Freq: %5.2f Hz", cachedData.frequency);
    lcd.print(buffer);
}

void LCDManager::renderNetwork() {
    char buffer[21];
    
    // Line 0: Header
    lcd.setCursor(0, 0);
    lcd.print("====  NETWORK  ==== ");
    
    // Line 1: IP Address
    lcd.setCursor(0, 1);
    snprintf(buffer, sizeof(buffer), "IP: %-15s", networkIP);
    lcd.print(buffer);
    
    // Line 2: WiFi/Ethernet status
    lcd.setCursor(0, 2);
    if (networkRSSI == -1) {
        lcd.print("ETH: Connected      ");
    } else {
        snprintf(buffer, sizeof(buffer), "WiFi: %d dBm", networkRSSI);
        lcd.print(buffer);
        // Clear rest of line
        for (int i = strlen(buffer); i < 20; i++) lcd.print(" ");
    }
    
    // Line 3: MQTT status
    lcd.setCursor(0, 3);
    lcd.print("MQTT: ");
    lcd.print(networkMQTT ? "Connected" : "Disconn.");
    lcd.print("    ");
}

void LCDManager::renderSystem() {
    char buffer[21];
    
    // Line 0: Header
    lcd.setCursor(0, 0);
    lcd.print("====  SYSTEM  ====  ");
    
    // Line 1: Uptime
    lcd.setCursor(0, 1);
    uint32_t days = systemUptime / 86400;
    uint32_t hours = (systemUptime % 86400) / 3600;
    uint32_t mins = (systemUptime % 3600) / 60;
    snprintf(buffer, sizeof(buffer), "Up:%3u %02uh %02um", days, hours, mins);
    lcd.print(buffer);
    
    // Line 2: Temperature and Heap
    lcd.setCursor(0, 2);
    snprintf(buffer, sizeof(buffer), "T:%4.1f", systemTemp);
    lcd.print(buffer);
    lcd.write(CHAR_DEGREE);
    snprintf(buffer, sizeof(buffer), "C H:%5luKB", systemFreeHeap / 1024);
    lcd.print(buffer);
    
    // Line 3: Error count
    lcd.setCursor(0, 3);
    snprintf(buffer, sizeof(buffer), "Errors: %lu", systemErrorCount);
    lcd.print(buffer);
    // Clear rest of line
    for (int i = strlen(buffer); i < 20; i++) lcd.print(" ");
}

void LCDManager::formatValue(char* buffer, float value, uint8_t decimals, const char* unit) {
    char fmt[16];
    snprintf(fmt, sizeof(fmt), "%%.%df%%s", decimals);
    snprintf(buffer, 20, fmt, value, unit);
}
