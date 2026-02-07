# HAL Subsystem - Quick Start Guide

## 1. Add Required Library

### PlatformIO (platformio.ini)
```ini
lib_deps = 
    adafruit/Adafruit MCP23017 Arduino Library @ ^2.3.0
```

### Arduino IDE
- Library Manager → Search "Adafruit MCP23017" → Install v2.3.0+

## 2. Basic Initialization

```cpp
#include "src/hal/BoardConfig.h"
#include "src/hal/I2CBus.h"
#include "src/hal/SPIBus.h"
#include "src/hal/GPIOManager.h"

void setup() {
    Serial.begin(115200);
    
    // Initialize in order
    BoardConfig::getInstance().init();
    I2CBus::getInstance().init();
    SPIBus::getInstance().init();
    GPIOManager::getInstance().init();
    
    // Set heartbeat LED
    GPIOManager::getInstance().setLEDPattern(LED::RUN, LEDPattern::HEARTBEAT);
}

void loop() {
    GPIOManager::getInstance().updateLEDs();
    GPIOManager::getInstance().updateButtons();
    delay(10);
}
```

## 3. Common Operations

### LED Control
```cpp
GPIOManager& gpio = GPIOManager::getInstance();

// Simple on/off
gpio.setLED(LED::WIFI, true);

// Patterns
gpio.setLEDPattern(LED::RUN, LEDPattern::HEARTBEAT);
gpio.setLEDPattern(LED::FAULT, LEDPattern::BLINK_FAST, 5000);  // 5 seconds
```

### Button Handling
```cpp
// Polling
ButtonState state = gpio.getButtonState(Button::MODE);

// Callback
void onButton(Button btn, ButtonState state) {
    if (state == ButtonState::PRESSED) {
        // Handle press
    }
}
gpio.attachButtonHandler(Button::MODE, onButton);
```

### Relay Control
```cpp
gpio.setRelay(Relay::RELAY_1, true);   // On
gpio.setRelay(Relay::RELAY_2, false);  // Off
```

### SPI Access
```cpp
SPIBus& spi = SPIBus::getInstance();

if (spi.beginTransaction(SPIDevice::ATM90E36)) {
    uint16_t data = spi.transfer16(0x1234);
    spi.endTransaction();
}
```

### I2C Access
```cpp
I2CBus& i2c = I2CBus::getInstance();

uint8_t value;
i2c.readRegister(0x20, 0x00, &value);
i2c.writeRegister(0x20, 0x00, 0xFF);
```

## 4. Full Example

See `src/hal/HAL_Example.ino` for complete working example.

## 5. Troubleshooting

**MCP23017 not found**
- Check I2C address (default 0x20)
- Verify I2C connections (SDA=21, SCL=22)
- Check pull-up resistors

**LEDs not working**
- Verify MCP23017 initialized
- Call updateLEDs() in main loop
- Check LED logic (active LOW)

**Buttons not responding**
- Call updateButtons() in main loop
- Check debounce time (50ms default)
- Verify button pins (MODE=32, SET=33)

## 6. API Reference

Full documentation in `src/hal/README.md`
