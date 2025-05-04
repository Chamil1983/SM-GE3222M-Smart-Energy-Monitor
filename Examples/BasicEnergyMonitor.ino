/**
 * SM Energy GE3222M - Basic Energy Monitor Example
 * 
 * This example demonstrates basic usage of the SM Energy GE3222M board
 * with ATM90E32 for single-phase energy monitoring.
 * 
 * Hardware Requirements:
 * - SM Energy GE3222M board
 * - CT clamp (e.g., YHDC SCT-013-000)
 * - 9-12VDC power supply
 * - USB cable for programming
 * 
 * Software Requirements:
 * - Arduino IDE
 * - ESP32 board package
 * - ATM90E32_Arduino library
 * 
 * Created by: Microcode Embedded Solutions
 * Date: May 4, 2025
 */

#include <ATM90E32.h>

/**
 * Pin Definitions
 * These pins are specifically for the SM Energy GE3222M board
 */
#define CS_PIN  5    // Chip Select Pin
#define IRQ_PIN 4    // IRQ Pin (optional)

// Line frequency - set based on your country (50Hz for Australia/Europe, 60Hz for USA)
#define LINE_FREQUENCY LineFreq_50Hz

// PGA gain - adjust based on your specific requirements and CT clamp type
#define PGA_GAIN PGAGain_2x

// Create instance of ATM90E32 energy monitor
ATM90E32 energyMonitor;

// Variables to store energy monitoring data
float voltageA, voltageB, voltageC;           // Line voltages
float currentA, currentB, currentC;           // Line currents
float activePowerA, activePowerB, activePowerC;   // Active power
float reactivePowerA, reactivePowerB, reactivePowerC; // Reactive power
float apparentPowerA, apparentPowerB, apparentPowerC; // Apparent power
float powerFactorA, powerFactorB, powerFactorC;   // Power factor
float frequency;                              // Line frequency
float totalActivePower, totalReactivePower, totalApparentPower; // Total power values
float totalPowerFactor;                       // Total power factor

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(500);
  
  Serial.println("-------------------------------------------");
  Serial.println("SM Energy GE3222M - Basic Energy Monitor");
  Serial.println("-------------------------------------------");
  
  // Initialize the energy monitor
  Serial.println("Initializing ATM90E32...");
  
  // Initialize with default calibration values (0, 0)
  // For accurate readings, you need to calibrate these values
  energyMonitor.begin(CS_PIN, LINE_FREQUENCY, PGA_GAIN, 0, 0);
  
  delay(1000);
  
  // Check for system status
  unsigned short systemStatus = energyMonitor.GetSysStatus();
  if (systemStatus & 0xC000) {
    // If error bits are set
    Serial.println("Error: ATM90E32 initialization failed");
    Serial.print("System Status: 0x");
    Serial.println(systemStatus, HEX);
  } else {
    Serial.println("ATM90E32 initialized successfully");
    Serial.print("System Status: 0x");
    Serial.println(systemStatus, HEX);
  }
  
  Serial.println("\nStarting energy monitoring...");
  Serial.println("-------------------------------------------");
  delay(1000);
}

void loop() {
  // Read line voltages
  voltageA = energyMonitor.GetLineVoltageA();
  voltageB = energyMonitor.GetLineVoltageB();
  voltageC = energyMonitor.GetLineVoltageC();
  
  // Read line currents
  currentA = energyMonitor.GetLineCurrentA();
  currentB = energyMonitor.GetLineCurrentB();
  currentC = energyMonitor.GetLineCurrentC();
  
  // Read active power
  activePowerA = energyMonitor.GetActivePowerA();
  activePowerB = energyMonitor.GetActivePowerB();
  activePowerC = energyMonitor.GetActivePowerC();
  
  // Read reactive power
  reactivePowerA = energyMonitor.GetReactivePowerA();
  reactivePowerB = energyMonitor.GetReactivePowerB();
  reactivePowerC = energyMonitor.GetReactivePowerC();
  
  // Read apparent power
  apparentPowerA = energyMonitor.GetApparentPowerA();
  apparentPowerB = energyMonitor.GetApparentPowerB();
  apparentPowerC = energyMonitor.GetApparentPowerC();
  
  // Read power factor
  powerFactorA = energyMonitor.GetPowerFactorA();
  powerFactorB = energyMonitor.GetPowerFactorB();
  powerFactorC = energyMonitor.GetPowerFactorC();
  
  // Read frequency
  frequency = energyMonitor.GetFrequency();
  
  // Read total values
  totalActivePower = energyMonitor.GetTotalActivePower();
  totalReactivePower = energyMonitor.GetTotalReactivePower();
  totalApparentPower = energyMonitor.GetTotalApparentPower();
  totalPowerFactor = energyMonitor.GetTotalPowerFactor();
  
  // Print readings
  Serial.println("--- ENERGY MONITORING READINGS ---");
  
  // Print frequency
  Serial.print("Line Frequency: ");
  Serial.print(frequency, 2);
  Serial.println(" Hz");
  
  // Print phase A readings
  Serial.println("\nPHASE A:");
  Serial.print("  Voltage: ");
  Serial.print(voltageA, 2);
  Serial.println(" V");
  Serial.print("  Current: ");
  Serial.print(currentA, 3);
  Serial.println(" A");
  Serial.print("  Active Power: ");
  Serial.print(activePowerA, 2);
  Serial.println(" W");
  Serial.print("  Reactive Power: ");
  Serial.print(reactivePowerA, 2);
  Serial.println(" VAR");
  Serial.print("  Apparent Power: ");
  Serial.print(apparentPowerA, 2);
  Serial.println(" VA");
  Serial.print("  Power Factor: ");
  Serial.println(powerFactorA, 3);
  
  // Print phase B readings
  Serial.println("\nPHASE B:");
  Serial.print("  Voltage: ");
  Serial.print(voltageB, 2);
  Serial.println(" V");
  Serial.print("  Current: ");
  Serial.print(currentB, 3);
  Serial.println(" A");
  Serial.print("  Active Power: ");
  Serial.print(activePowerB, 2);
  Serial.println(" W");
  Serial.print("  Reactive Power: ");
  Serial.print(reactivePowerB, 2);
  Serial.println(" VAR");
  Serial.print("  Apparent Power: ");
  Serial.print(apparentPowerB, 2);
  Serial.println(" VA");
  Serial.print("  Power Factor: ");
  Serial.println(powerFactorB, 3);
  
  // Print phase C readings
  Serial.println("\nPHASE C:");
  Serial.print("  Voltage: ");
  Serial.print(voltageC, 2);
  Serial.println(" V");
  Serial.print("  Current: ");
  Serial.print(currentC, 3);
  Serial.println(" A");
  Serial.print("  Active Power: ");
  Serial.print(activePowerC, 2);
  Serial.println(" W");
  Serial.print("  Reactive Power: ");
  Serial.print(reactivePowerC, 2);
  Serial.println(" VAR");
  Serial.print("  Apparent Power: ");
  Serial.print(apparentPowerC, 2);
  Serial.println(" VA");
  Serial.print("  Power Factor: ");
  Serial.println(powerFactorC, 3);
  
  // Print total readings
  Serial.println("\nTOTAL READINGS:");
  Serial.print("  Total Active Power: ");
  Serial.print(totalActivePower, 2);
  Serial.println(" W");
  Serial.print("  Total Reactive Power: ");
  Serial.print(totalReactivePower, 2);
  Serial.println(" VAR");
  Serial.print("  Total Apparent Power: ");
  Serial.print(totalApparentPower, 2);
  Serial.println(" VA");
  Serial.print("  Total Power Factor: ");
  Serial.println(totalPowerFactor, 3);
  
  Serial.println("\n-------------------------------------------");
  
  // Wait before next reading
  delay(5000);
}
