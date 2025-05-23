# SM Energy GE3222M Smart Energy Monitor - GitHub Repository Structure

<p align="center">
<img src="/Pictures/IMG_4798.jpg" alt="Cortex Link A8F-M ESP32 Smart Relay Board" width="500"/>

  <img src="/Pictures/Main_Board.jpg" alt="Cortex Link A8F-M ESP32 Smart Relay Board" width="600"/>
</p>

## Overview

The SM Energy GE3222M is a flexible, dual-purpose smart energy monitoring solution based on the ESP32 platform. This repository contains all the necessary documentation, code examples, and resources to get started with the GE3222M.

## Features

- **ESP32 WROOM 32** with internal antenna or UF.L connector for external antenna
- **ATM90E32/ATM90E36 Energy ICs** for accurate power monitoring
- **Multi-phase monitoring**: 3-phase line CT clamp inputs
- **Voltage inputs**: 3x AC RMS inputs
- **Onboard sensors**: DHT22 temperature and humidity sensor
- **Connectivity**: WiFi, Bluetooth, Ethernet, and MODBUS
- **Programming**: Full Arduino IDE compatibility
- **Flexible power options**: 9-12VDC input
- **User interface**: I2C 20x4 LCD module support, status LEDs
- **Data storage**: 24C64 EEPROM (I2C)
- **Safety**: All connections are low voltage for safe operation

## Repository Structure

### /Hardware
- **Datasheets/** - Component datasheets and technical documentation
- **Schematics/** - Board schematics and PCB layout files
- **Images/** - High-resolution images of the board
- **BOM/** - Bill of materials

### /Firmware
- **Libraries/** - Required Arduino libraries
  - **ATM90E32_Arduino/** - Library for the ATM90E32 energy IC
  - **ATM90E36_Arduino/** - Library for the ATM90E36 energy IC
- **Examples/** - Example sketches
  - **BasicEnergyMonitor/** - Simple energy monitoring example
  - **WiFiWebServer/** - Web server implementation for remote monitoring
  - **MODBUS_RTU/** - MODBUS communication example
  - **CloudIntegration/** - Examples for popular cloud platforms
  - **LCD_Display/** - Example for I2C LCD interface
  - **DataLogging/** - EEPROM and SD card logging examples
  - **Calibration/** - Calibration procedures and examples
  - **MultiPhase/** - Three-phase monitoring examples

### /Documentation
- **GettingStarted.md** - Initial setup instructions
- **CT_Clamp_Selection.md** - Guide for selecting appropriate CT clamps
- **Calibration.md** - Calibration procedures
- **API_Reference.md** - Software API documentation
- **Troubleshooting.md** - Common issues and solutions
- **Projects.md** - Example projects and applications

### /Applications
- **WebDashboard/** - Web-based monitoring dashboard
- **MobileApp/** - Mobile application source code (if applicable)
- **HomeAssistant/** - Home Assistant integration
- **EspHome/** - ESPHome configuration examples
- **NodeRed/** - Node-RED integration examples

### /Certification
- Compliance and certification documentation

## Compatible CT Clamps
- 20A/25mA: SCT-006
- 30A/1V: SCT-013-030 (JP1, JP2, and JP3 jumpers should be open)
- 50A/1V: SCT-013-050 (JP1, JP2, and JP3 jumpers should be open)
- 80A/26.6mA: SCT-010
- 100A/50mA: SCT-013-000
- 120A/40mA: SCT-016
- 200A/100mA: SCT-024
- 200A/50mA: SCT-024

## Getting Started

### Hardware Requirements
- SM Energy GE3222M board
- CT clamps (e.g., YHDC SCT-013-000)
- 9-12VDC power supply
- USB cable for programming

### Software Requirements
- Arduino IDE
- ESP32 board package
- Required libraries (listed in the Libraries folder)

### Installation

1. Install Arduino IDE
2. Add ESP32 board package to Arduino IDE
   - Add `https://dl.espressif.com/dl/package_esp32_index.json` to Additional Board Manager URLs
   - Install ESP32 board package from Board Manager
3. Install required libraries
4. Select "ESP32 Dev Module" or "WEMOS D1 MINI ESP32" as the board
5. Connect the board via USB and select the appropriate port
6. Upload an example sketch

## Sample Code

```cpp
#include <ATM90E32.h>

/**
 * Basic Energy Monitor Example
 * For SM Energy GE3222M with ATM90E32
 */

ATM90E32 energyMonitor;

void setup() {
  Serial.begin(115200);
  Serial.println("SM Energy GE3222M Basic Example");
  
  // Initialize the ATM90E32 energy IC
  energyMonitor.begin(CS_PIN, LineFreq_50Hz, PGAGain_2x, 0, 0);
  
  delay(1000);
}

void loop() {
  // Read voltage
  float voltageA = energyMonitor.GetLineVoltageA();
  float voltageB = energyMonitor.GetLineVoltageB();
  float voltageC = energyMonitor.GetLineVoltageC();
  
  // Read current
  float currentA = energyMonitor.GetLineCurrentA();
  float currentB = energyMonitor.GetLineCurrentB();
  float currentC = energyMonitor.GetLineCurrentC();
  
  // Read power
  float activePowerA = energyMonitor.GetActivePowerA();
  float activePowerB = energyMonitor.GetActivePowerB();
  float activePowerC = energyMonitor.GetActivePowerC();
  
  // Output readings
  Serial.println("Voltage (V):");
  Serial.print("  Phase A: "); Serial.println(voltageA);
  Serial.print("  Phase B: "); Serial.println(voltageB);
  Serial.print("  Phase C: "); Serial.println(voltageC);
  
  Serial.println("Current (A):");
  Serial.print("  Phase A: "); Serial.println(currentA);
  Serial.print("  Phase B: "); Serial.println(currentB);
  Serial.print("  Phase C: "); Serial.println(currentC);
  
  Serial.println("Active Power (W):");
  Serial.print("  Phase A: "); Serial.println(activePowerA);
  Serial.print("  Phase B: "); Serial.println(activePowerB);
  Serial.print("  Phase C: "); Serial.println(activePowerC);
  
  Serial.println("-------------------------------");
  delay(5000);
}

```
