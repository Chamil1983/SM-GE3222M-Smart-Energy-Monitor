/**
 * @file HAL_Example.ino
 * @brief Example usage of HAL subsystem
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Demonstrates initialization and usage of all HAL components
 */

#include "src/hal/BoardConfig.h"
#include "src/hal/I2CBus.h"
#include "src/hal/SPIBus.h"
#include "src/hal/GPIOManager.h"

// Button callback function
void onButtonPress(Button button, ButtonState state) {
    if (button == Button::MODE) {
        switch (state) {
            case ButtonState::PRESSED:
                Serial.println("Mode button pressed");
                GPIOManager::getInstance().setLEDPattern(LED::WIFI, LEDPattern::BLINK_FAST, 1000);
                break;
            case ButtonState::HELD:
                Serial.println("Mode button held");
                break;
            case ButtonState::LONG_PRESS:
                Serial.println("Mode button long press");
                GPIOManager::getInstance().emergencyStop();
                break;
            default:
                break;
        }
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n========================================");
    Serial.println("SM-GE3222M HAL Subsystem Example");
    Serial.println("========================================\n");
    
    // ============================================================
    // 1. Initialize Board Configuration
    // ============================================================
    Serial.println("[Setup] Initializing board configuration...");
    BoardConfig& board = BoardConfig::getInstance();
    if (!board.init()) {
        Serial.println("[Setup] ERROR: Board initialization failed!");
        while (1) delay(1000);
    }
    
    // ============================================================
    // 2. Initialize I2C Bus
    // ============================================================
    Serial.println("\n[Setup] Initializing I2C bus...");
    I2CBus& i2c = I2CBus::getInstance();
    if (!i2c.init(100000)) {  // 100kHz
        Serial.println("[Setup] ERROR: I2C initialization failed!");
        while (1) delay(1000);
    }
    
    // ============================================================
    // 3. Initialize SPI Bus
    // ============================================================
    Serial.println("\n[Setup] Initializing SPI bus...");
    SPIBus& spi = SPIBus::getInstance();
    if (!spi.init()) {
        Serial.println("[Setup] ERROR: SPI initialization failed!");
        while (1) delay(1000);
    }
    
    // ============================================================
    // 4. Initialize GPIO Manager
    // ============================================================
    Serial.println("\n[Setup] Initializing GPIO manager...");
    GPIOManager& gpio = GPIOManager::getInstance();
    if (!gpio.init()) {
        Serial.println("[Setup] ERROR: GPIO initialization failed!");
        while (1) delay(1000);
    }
    
    // ============================================================
    // 5. Configure LEDs
    // ============================================================
    Serial.println("\n[Setup] Configuring LEDs...");
    
    // Set LED patterns
    gpio.setLEDPattern(LED::RUN, LEDPattern::HEARTBEAT);     // Heartbeat on run LED
    gpio.setLEDPattern(LED::WIFI, LEDPattern::BLINK_SLOW);   // Slow blink for WiFi
    gpio.setLED(LED::FAULT, false);                          // Fault LED off
    gpio.setLED(LED::MODBUS, false);                         // Modbus LED off
    
    // ============================================================
    // 6. Configure Buttons
    // ============================================================
    Serial.println("[Setup] Configuring buttons...");
    gpio.attachButtonHandler(Button::MODE, onButtonPress);
    
    // ============================================================
    // 7. Test Relays
    // ============================================================
    Serial.println("\n[Setup] Testing relays...");
    Serial.println("  Relay 1 ON");
    gpio.setRelay(Relay::RELAY_1, true);
    delay(1000);
    Serial.println("  Relay 1 OFF");
    gpio.setRelay(Relay::RELAY_1, false);
    
    delay(500);
    
    Serial.println("  Relay 2 ON");
    gpio.setRelay(Relay::RELAY_2, true);
    delay(1000);
    Serial.println("  Relay 2 OFF");
    gpio.setRelay(Relay::RELAY_2, false);
    
    // ============================================================
    // 8. Validate Hardware
    // ============================================================
    Serial.println("\n[Setup] Validating hardware...");
    if (board.validate()) {
        Serial.println("[Setup] Hardware validation passed");
    } else {
        Serial.println("[Setup] WARNING: Hardware validation issues detected");
    }
    
    // ============================================================
    // 9. Print Diagnostics
    // ============================================================
    Serial.println("\n[Setup] System diagnostics:");
    Serial.print("  I2C errors: ");
    Serial.println(i2c.getErrorCount());
    Serial.print("  SPI ATM errors: ");
    Serial.println(spi.getErrorCount(SPIDevice::ATM90E36));
    Serial.print("  SPI W5500 errors: ");
    Serial.println(spi.getErrorCount(SPIDevice::W5500));
    
    Serial.println("\n[Setup] HAL initialization complete!");
    Serial.println("========================================\n");
    
    // Show LED test sequence
    Serial.println("Running LED test sequence...");
    testLEDPatterns();
}

void loop() {
    static unsigned long lastReport = 0;
    
    // Update GPIO manager (LEDs and buttons)
    GPIOManager& gpio = GPIOManager::getInstance();
    gpio.updateLEDs();
    gpio.updateButtons();
    
    // Report status every 5 seconds
    if (millis() - lastReport >= 5000) {
        lastReport = millis();
        printStatus();
    }
    
    delay(10);  // 10ms loop time
}

// LED pattern test sequence
void testLEDPatterns() {
    GPIOManager& gpio = GPIOManager::getInstance();
    
    Serial.println("\n=== LED Pattern Test ===");
    
    // Test OFF/ON
    Serial.println("Test: OFF/ON");
    gpio.setLED(LED::WIFI, true);
    delay(1000);
    gpio.setLED(LED::WIFI, false);
    delay(500);
    
    // Test BLINK_SLOW
    Serial.println("Test: BLINK_SLOW (3 seconds)");
    gpio.setLEDPattern(LED::WIFI, LEDPattern::BLINK_SLOW, 3000);
    delay(3500);
    
    // Test BLINK_FAST
    Serial.println("Test: BLINK_FAST (3 seconds)");
    gpio.setLEDPattern(LED::RUN, LEDPattern::BLINK_FAST, 3000);
    delay(3500);
    
    // Test PULSE
    Serial.println("Test: PULSE (3 seconds)");
    gpio.setLEDPattern(LED::FAULT, LEDPattern::PULSE, 3000);
    delay(3500);
    
    // Test HEARTBEAT
    Serial.println("Test: HEARTBEAT (3 seconds)");
    gpio.setLEDPattern(LED::MODBUS, LEDPattern::HEARTBEAT, 3000);
    delay(3500);
    
    // Restore default patterns
    gpio.setLEDPattern(LED::RUN, LEDPattern::HEARTBEAT);
    gpio.setLEDPattern(LED::WIFI, LEDPattern::BLINK_SLOW);
    gpio.setLED(LED::FAULT, false);
    gpio.setLED(LED::MODBUS, false);
    
    Serial.println("=== LED Test Complete ===\n");
}

// Print system status
void printStatus() {
    Serial.println("\n=== System Status ===");
    
    // Board info
    BoardConfig& board = BoardConfig::getInstance();
    Serial.print("Board: ");
    Serial.print(board.getName());
    Serial.print(" ");
    Serial.println(board.getRevision());
    
    // Memory
    Serial.print("Free Heap: ");
    Serial.print(ESP.getFreeHeap() / 1024);
    Serial.println(" KB");
    
    // Error counts
    I2CBus& i2c = I2CBus::getInstance();
    SPIBus& spi = SPIBus::getInstance();
    
    Serial.print("I2C Errors: ");
    Serial.println(i2c.getErrorCount());
    Serial.print("SPI ATM Errors: ");
    Serial.println(spi.getErrorCount(SPIDevice::ATM90E36));
    Serial.print("SPI W5500 Errors: ");
    Serial.println(spi.getErrorCount(SPIDevice::W5500));
    
    // Button states
    GPIOManager& gpio = GPIOManager::getInstance();
    ButtonState modeState = gpio.getButtonState(Button::MODE);
    ButtonState setState = gpio.getButtonState(Button::SET);
    
    Serial.print("Mode Button: ");
    switch (modeState) {
        case ButtonState::RELEASED: Serial.println("Released"); break;
        case ButtonState::PRESSED: Serial.println("Pressed"); break;
        case ButtonState::HELD: Serial.println("Held"); break;
        case ButtonState::LONG_PRESS: Serial.println("Long Press"); break;
    }
    
    Serial.print("Set Button: ");
    switch (setState) {
        case ButtonState::RELEASED: Serial.println("Released"); break;
        case ButtonState::PRESSED: Serial.println("Pressed"); break;
        case ButtonState::HELD: Serial.println("Held"); break;
        case ButtonState::LONG_PRESS: Serial.println("Long Press"); break;
    }
    
    // Relay states
    Serial.print("Relay 1: ");
    Serial.println(gpio.getRelayState(Relay::RELAY_1) ? "ON" : "OFF");
    Serial.print("Relay 2: ");
    Serial.println(gpio.getRelayState(Relay::RELAY_2) ? "ON" : "OFF");
    
    Serial.println("=====================\n");
}
