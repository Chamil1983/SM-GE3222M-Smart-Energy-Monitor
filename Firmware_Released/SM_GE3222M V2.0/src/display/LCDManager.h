/**
 * @file LCDManager.h
 * @brief 20x4 LCD display management system
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Multi-page display with auto-scroll and button navigation
 */

#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "../../include/GlobalTypes.h"
#include "../../include/PinMap.h"

/**
 * @brief Display page identifiers
 */
enum class DisplayPage {
    SPLASH = 0,     // Splash screen with firmware version
    PHASE_A,        // Phase A: V, I, P, PF
    PHASE_B,        // Phase B: V, I, P, PF
    PHASE_C,        // Phase C: V, I, P, PF
    TOTALS,         // Totals: P, Q, S, PF
    ENERGY,         // Energy: Import/Export kWh
    NETWORK,        // Network: IP, RSSI, MQTT status
    SYSTEM,         // System: Uptime, Temp, Heap, Errors
    MAX_PAGES       // Total number of pages
};

/**
 * @class LCDManager
 * @brief Manages 20x4 I2C LCD display with multi-page system
 * 
 * Singleton class providing comprehensive LCD management with:
 * - 8 display pages (splash, 3 phases, totals, energy, network, system)
 * - Auto-scroll mode with configurable interval
 * - Manual navigation via buttons
 * - Non-blocking updates
 * - Custom characters for symbols
 */
class LCDManager {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to LCDManager instance
     */
    static LCDManager& getInstance();
    
    /**
     * @brief Initialize LCD display
     * @return true if successful, false on failure
     */
    bool init();
    
    /**
     * @brief Clear display
     */
    void clear();
    
    /**
     * @brief Set display page
     * @param page Page number to display
     */
    void setPage(DisplayPage page);
    
    /**
     * @brief Go to next page
     */
    void nextPage();
    
    /**
     * @brief Go to previous page
     */
    void prevPage();
    
    /**
     * @brief Update current page display (call from main loop)
     * 
     * Non-blocking update that refreshes display based on current page
     * and respects auto-scroll timing
     */
    void update();
    
    /**
     * @brief Update meter data for display
     * @param data Reference to MeterData structure
     * 
     * Updates internal data cache for display rendering
     */
    void displayMeterData(const MeterData& data);
    
    /**
     * @brief Enable/disable auto-scroll mode
     * @param enabled True to enable auto-scroll
     * @param intervalMs Scroll interval in milliseconds (default 5000)
     */
    void setAutoScroll(bool enabled, uint32_t intervalMs = 5000);
    
    /**
     * @brief Get auto-scroll status
     * @return true if auto-scroll is enabled
     */
    bool isAutoScrollEnabled() const { return autoScrollEnabled; }
    
    /**
     * @brief Display custom message on specific line
     * @param line Line number (0-3)
     * @param message Message to display (max 20 chars)
     */
    void displayMessage(uint8_t line, const char* message);
    
    /**
     * @brief Get current page
     * @return Current DisplayPage
     */
    DisplayPage getCurrentPage() const { return currentPage; }
    
    /**
     * @brief Set network info for display
     * @param ip IP address string
     * @param rssi WiFi RSSI or -1 if using Ethernet
     * @param mqttConnected MQTT connection status
     */
    void setNetworkInfo(const char* ip, int rssi, bool mqttConnected);
    
    /**
     * @brief Set system info for display
     * @param uptime System uptime in seconds
     * @param temp Temperature in Celsius
     * @param freeHeap Free heap in bytes
     * @param errorCount Total error count
     */
    void setSystemInfo(uint32_t uptime, float temp, uint32_t freeHeap, uint32_t errorCount);

private:
    // Private constructor for singleton
    LCDManager();
    
    // Delete copy constructor and assignment operator
    LCDManager(const LCDManager&) = delete;
    LCDManager& operator=(const LCDManager&) = delete;
    
    /**
     * @brief Initialize custom characters
     */
    void initCustomCharacters();
    
    /**
     * @brief Render splash screen
     */
    void renderSplash();
    
    /**
     * @brief Render phase data page
     * @param phase Phase identifier ('A', 'B', or 'C')
     * @param data Phase data structure
     */
    void renderPhase(char phase, const PhaseData& data);
    
    /**
     * @brief Render totals page
     */
    void renderTotals();
    
    /**
     * @brief Render energy page
     */
    void renderEnergy();
    
    /**
     * @brief Render network page
     */
    void renderNetwork();
    
    /**
     * @brief Render system page
     */
    void renderSystem();
    
    /**
     * @brief Format float value with units
     * @param buffer Output buffer
     * @param value Value to format
     * @param decimals Number of decimal places
     * @param unit Unit string
     */
    void formatValue(char* buffer, float value, uint8_t decimals, const char* unit);
    
    // LCD hardware
    LiquidCrystal_I2C lcd;
    
    // Display state
    DisplayPage currentPage;
    bool initialized;
    
    // Auto-scroll
    bool autoScrollEnabled;
    uint32_t autoScrollInterval;
    unsigned long lastScrollTime;
    
    // Data cache
    MeterData cachedData;
    
    // Network info
    char networkIP[16];
    int networkRSSI;
    bool networkMQTT;
    
    // System info
    uint32_t systemUptime;
    float systemTemp;
    uint32_t systemFreeHeap;
    uint32_t systemErrorCount;
    
    // Update tracking
    unsigned long lastUpdateTime;
    bool needsRedraw;
    
    // Custom character indices
    static const uint8_t CHAR_DEGREE = 0;
    static const uint8_t CHAR_ARROW = 1;
    static const uint8_t CHAR_WIFI = 2;
    static const uint8_t CHAR_CHECK = 3;
};

#endif // LCDMANAGER_H
