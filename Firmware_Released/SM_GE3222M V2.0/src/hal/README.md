# Hardware Abstraction Layer (HAL) Subsystem

## Overview
The HAL subsystem provides hardware abstraction for the SM-GE3222M V2.0 Smart Energy Monitor firmware. It implements thread-safe access to all hardware peripherals including SPI, I2C, GPIO, and board configuration.

## Architecture

### Design Patterns
- **Singleton Pattern**: All HAL classes use singleton pattern for single point of access
- **Thread Safety**: SPI bus uses FreeRTOS mutex for thread-safe multi-device access
- **Error Tracking**: All bus managers track error counts for diagnostics
- **State Machines**: Button handling uses state machine for debouncing and gesture detection

### Dependencies
```
BoardConfig (no dependencies)
    ↓
I2CBus (Wire library)
    ↓
SPIBus (SPI library, FreeRTOS)
    ↓
GPIOManager (Adafruit_MCP23X17, I2CBus)
```

## Components

### 1. BoardConfig
**Purpose**: Board initialization, hardware identification, and validation

**Key Features**:
- ESP32 chip identification
- GPIO pin initialization
- Hardware validation
- Board information reporting

**Usage**:
```cpp
BoardConfig& board = BoardConfig::getInstance();
if (board.init()) {
    board.printInfo();
    uint64_t chipID = board.getChipID();
}
```

**Methods**:
- `init()` - Initialize board hardware
- `getName()` - Get board name ("SM-GE3222M")
- `getRevision()` - Get hardware revision ("V2.0")
- `validate()` - Validate hardware configuration
- `getChipID()` - Get ESP32 chip ID
- `getMacAddress()` - Get MAC address
- `printInfo()` - Print board information

---

### 2. SPIBus
**Purpose**: Thread-safe SPI bus management for ATM90E36 and W5500

**Key Features**:
- FreeRTOS mutex protection
- Per-device speed configuration (ATM: 1MHz, W5500: 8MHz)
- Automatic chip select management
- Error counting per device

**Usage**:
```cpp
SPIBus& spi = SPIBus::getInstance();
spi.init();

// Transaction example
if (spi.beginTransaction(SPIDevice::ATM90E36)) {
    uint16_t data = spi.transfer16(0x1234);
    spi.endTransaction();
}

// Check errors
uint32_t errors = spi.getErrorCount(SPIDevice::ATM90E36);
```

**Methods**:
- `init()` - Initialize SPI bus
- `beginTransaction(device)` - Begin SPI transaction with mutex lock
- `endTransaction()` - End transaction and release mutex
- `transfer16(data)` - Transfer 16-bit data
- `transfer8(data)` - Transfer 8-bit data
- `transfer(buffer, size)` - Transfer multiple bytes
- `getErrorCount(device)` - Get error count for device
- `recordError()` - Record error for current device

**Thread Safety**:
- Uses FreeRTOS semaphore with 100ms timeout
- Automatic mutex release on endTransaction()
- Safe for use from multiple tasks

---

### 3. I2CBus
**Purpose**: I2C bus management for LCD, MCP23017, and sensors

**Key Features**:
- Device scanning and detection
- Timeout handling (500ms default)
- Register read/write operations
- Error tracking

**Usage**:
```cpp
I2CBus& i2c = I2CBus::getInstance();
i2c.init(100000);  // 100kHz

// Scan for devices
uint8_t addresses[16];
uint8_t count = i2c.scan(addresses, 16);

// Check device presence
if (i2c.isDevicePresent(0x20)) {
    uint8_t value;
    i2c.readRegister(0x20, 0x00, &value);
}

// Multi-byte read
uint8_t buffer[4];
i2c.read(0x20, 0x00, buffer, 4);
```

**Methods**:
- `init(frequency)` - Initialize I2C bus
- `scan(addresses, maxDevices)` - Scan bus for devices
- `isDevicePresent(address)` - Check if device responds
- `readRegister(addr, reg, *value)` - Read single register
- `writeRegister(addr, reg, value)` - Write single register
- `read(addr, reg, *buffer, length)` - Read multiple bytes
- `write(addr, reg, *buffer, length)` - Write multiple bytes
- `getErrorCount()` - Get total I2C errors

---

### 4. GPIOManager
**Purpose**: High-level GPIO management using MCP23017 I/O expander

**Key Features**:
- LED pattern engine (6 patterns)
- Button debouncing with state machine
- Relay control with safety features
- MCP23017 abstraction

#### LED Control

**LED Types**:
- `LED::WIFI` - WiFi status indicator
- `LED::RUN` - System heartbeat
- `LED::FAULT` - Fault indicator
- `LED::MODBUS` - Modbus activity

**LED Patterns**:
- `LEDPattern::OFF` - LED off
- `LEDPattern::ON` - LED on
- `LEDPattern::BLINK_SLOW` - 1Hz blink (500ms on/off)
- `LEDPattern::BLINK_FAST` - 5Hz blink (100ms on/off)
- `LEDPattern::PULSE` - Breathing effect
- `LEDPattern::HEARTBEAT` - Double blink pattern

**Usage**:
```cpp
GPIOManager& gpio = GPIOManager::getInstance();
gpio.init();

// Direct LED control
gpio.setLED(LED::WIFI, true);

// LED patterns
gpio.setLEDPattern(LED::RUN, LEDPattern::HEARTBEAT);
gpio.setLEDPattern(LED::FAULT, LEDPattern::BLINK_FAST, 5000);  // 5 seconds

// Update in main loop
void loop() {
    gpio.updateLEDs();  // Call every 10-20ms
}
```

#### Button Control

**Button Types**:
- `Button::MODE` - Mode selection button
- `Button::SET` - Set/confirm button

**Button States**:
- `ButtonState::RELEASED` - Button not pressed
- `ButtonState::PRESSED` - Button just pressed
- `ButtonState::HELD` - Held > 1 second
- `ButtonState::LONG_PRESS` - Held > 3 seconds

**Usage**:
```cpp
// Polling method
ButtonState state = gpio.getButtonState(Button::MODE);

// Callback method
void onButtonEvent(Button btn, ButtonState state) {
    if (btn == Button::MODE && state == ButtonState::PRESSED) {
        // Handle mode button press
    }
}

gpio.attachButtonHandler(Button::MODE, onButtonEvent);

// Update in main loop
void loop() {
    gpio.updateButtons();  // Call frequently
}
```

#### Relay Control

**Usage**:
```cpp
// Control relays
gpio.setRelay(Relay::RELAY_1, true);   // Energize
gpio.setRelay(Relay::RELAY_2, false);  // De-energize

// Check relay state
bool state = gpio.getRelayState(Relay::RELAY_1);

// Emergency shutdown
gpio.emergencyStop();  // Disables all relays
```

#### MCP23017 Direct Access

**Usage**:
```cpp
// Direct pin access (0-15)
bool value = gpio.readMCPPin(MCP_IRQ0);
gpio.writeMCPPin(MCP_SPARE_OUT_1, true);
```

**MCP23017 Pin Mapping**:
```
Port A (Outputs):
  GPA0 - Relay 1
  GPA1 - Relay 2
  GPA2 - WiFi LED
  GPA3 - Run LED
  GPA4 - Fault LED
  GPA5 - Modbus LED
  GPA6 - Spare output
  GPA7 - Spare output

Port B (Inputs):
  GPB0 - ATM90E36 IRQ0
  GPB1 - ATM90E36 IRQ1
  GPB2 - ATM90E36 Warning Output
  GPB3 - CF4 Pulse Input
  GPB4 - CF3 Pulse Input
  GPB5 - CF2 Pulse Input
  GPB6 - CF1 Pulse Input
  GPB7 - Spare input
```

---

## Initialization Sequence

```cpp
void setup() {
    Serial.begin(115200);
    
    // 1. Initialize board configuration
    BoardConfig& board = BoardConfig::getInstance();
    if (!board.init()) {
        Serial.println("Board init failed!");
        return;
    }
    
    // 2. Initialize I2C bus
    I2CBus& i2c = I2CBus::getInstance();
    if (!i2c.init()) {
        Serial.println("I2C init failed!");
        return;
    }
    
    // 3. Initialize SPI bus
    SPIBus& spi = SPIBus::getInstance();
    if (!spi.init()) {
        Serial.println("SPI init failed!");
        return;
    }
    
    // 4. Initialize GPIO manager
    GPIOManager& gpio = GPIOManager::getInstance();
    if (!gpio.init()) {
        Serial.println("GPIO init failed!");
        return;
    }
    
    // Set heartbeat LED
    gpio.setLEDPattern(LED::RUN, LEDPattern::HEARTBEAT);
}

void loop() {
    // Update GPIO patterns
    GPIOManager::getInstance().updateLEDs();
    GPIOManager::getInstance().updateButtons();
    
    delay(10);  // 10ms loop
}
```

---

## Error Handling

All HAL classes implement error tracking:

```cpp
// Check SPI errors
uint32_t atmErrors = SPIBus::getInstance().getErrorCount(SPIDevice::ATM90E36);
uint32_t w5500Errors = SPIBus::getInstance().getErrorCount(SPIDevice::W5500);

// Check I2C errors
uint32_t i2cErrors = I2CBus::getInstance().getErrorCount();

// Reset error counts
SPIBus::getInstance().resetErrorCount(SPIDevice::ATM90E36);
I2CBus::getInstance().resetErrorCount();
```

---

## Required Libraries

Add to `platformio.ini`:
```ini
lib_deps =
    Wire
    SPI
    adafruit/Adafruit MCP23017 Arduino Library @ ^2.3.0
```

Or for Arduino IDE:
- Adafruit MCP23017 Library (v2.3.0+)

---

## Performance Notes

1. **LED Updates**: Call `updateLEDs()` every 10-20ms for smooth patterns
2. **Button Updates**: Call `updateButtons()` frequently for responsive input
3. **SPI Mutex**: 100ms timeout prevents deadlocks
4. **I2C Timeout**: 500ms default, configurable with `setTimeout()`

---

## Pin Configuration

All pin definitions in `include/PinMap.h`:
- SPI: MISO=19, MOSI=23, SCK=18
- I2C: SDA=21, SCL=22
- CS: ATM=5, W5500=15
- Buttons: MODE=32, SET=33

---

## Testing

Basic test procedure:
```cpp
void testHAL() {
    // Test all LEDs
    for (int i = 0; i < 4; i++) {
        GPIOManager::getInstance().setLED((LED)i, true);
        delay(500);
        GPIOManager::getInstance().setLED((LED)i, false);
    }
    
    // Test relays
    GPIOManager::getInstance().setRelay(Relay::RELAY_1, true);
    delay(1000);
    GPIOManager::getInstance().setRelay(Relay::RELAY_1, false);
    
    // Test I2C scan
    uint8_t devices[16];
    uint8_t count = I2CBus::getInstance().scan(devices, 16);
    Serial.printf("Found %d I2C devices\n", count);
}
```

---

## License
Part of SM-GE3222M Smart Energy Monitor Firmware V2.0

## Version History
- **V2.0** - Initial HAL implementation (2024)
  - Thread-safe SPI management
  - LED pattern engine
  - Button state machine
  - Relay control with safety features
