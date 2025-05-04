/**
 * SM Energy GE3222M - WiFi Web Server Example
 * 
 * This example demonstrates how to create a web server on the SM Energy GE3222M
 * that displays energy monitoring data over WiFi.
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
 * - WiFi.h, WebServer.h (included with ESP32 board package)
 * 
 * Created by: Microcode Embedded Solutions
 * Date: May 4, 2025
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ATM90E32.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "YourWiFiSSID";     // Replace with your WiFi SSID
const char* password = "YourWiFiPassword"; // Replace with your WiFi password

// ATM90E32 pin definitions
#define CS_PIN  5    // Chip Select Pin
#define LINE_FREQUENCY LineFreq_50Hz
#define PGA_GAIN PGAGain_2x

// DHT22 sensor pin
#define DHTPIN 21
#define DHTTYPE DHT22

// Create instances
ATM90E32 energyMonitor;
WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

// Variables to store energy monitoring data
float voltageA, voltageB, voltageC;
float currentA, currentB, currentC;
float activePowerA, activePowerB, activePowerC;
float totalActivePower, temperature, humidity;
float energyA, energyB, energyC, totalEnergy;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(500);
  
  Serial.println("-------------------------------------------");
  Serial.println("SM Energy GE3222M - WiFi Web Server Example");
  Serial.println("-------------------------------------------");
  
  // Initialize the energy monitor
  Serial.println("Initializing ATM90E32...");
  energyMonitor.begin(CS_PIN, LINE_FREQUENCY, PGA_GAIN, 0, 0);
  
  delay(1000);
  
  // Check for system status
  unsigned short systemStatus = energyMonitor.GetSysStatus();
  if (systemStatus & 0xC000) {
    Serial.println("Error: ATM90E32 initialization failed");
    Serial.print("System Status: 0x");
    Serial.println(systemStatus, HEX);
  } else {
    Serial.println("ATM90E32 initialized successfully");
  }
  
  // Initialize DHT sensor
  dht.begin();
  Serial.println("DHT22 sensor initialized");
  
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/metrics", handleMetrics);
  server.onNotFound(handleNotFound);
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("-------------------------------------------");
}

void loop() {
  // Handle client requests
  server.handleClient();
  
  // Read energy monitoring data periodically
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 5000) {
    updateEnergyData();
    lastUpdate = millis();
  }
}

void updateEnergyData() {
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
  
  // Read total active power
  totalActivePower = energyMonitor.GetTotalActivePower();
  
  // Read energy values (in Watt-hours)
  energyA = energyMonitor.GetImportEnergy();  // Import energy register
  totalEnergy = energyA;  // For simplicity, using only import energy
  
  // Read temperature and humidity
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  // Print readings to serial
  Serial.println("Energy Monitor Update:");
  Serial.print("Total Active Power: ");
  Serial.print(totalActivePower, 2);
  Serial.println(" W");
  Serial.print("Temperature: ");
  Serial.print(temperature, 1);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity, 1);
  Serial.println(" %");
}

void handleData() {
  // Create JSON response
  String json = "{\n";
  json += "  \"timestamp\": " + String(millis()) + ",\n";
  json += "  \"power\": {\n";
  json += "    \"total\": " + String(totalActivePower) + ",\n";
  json += "    \"phaseA\": " + String(activePowerA) + ",\n";
  json += "    \"phaseB\": " + String(activePowerB) + ",\n";
  json += "    \"phaseC\": " + String(activePowerC) + "\n";
  json += "  },\n";
  json += "  \"voltage\": {\n";
  json += "    \"phaseA\": " + String(voltageA) + ",\n";
  json += "    \"phaseB\": " + String(voltageB) + ",\n";
  json += "    \"phaseC\": " + String(voltageC) + "\n";
  json += "  },\n";
  json += "  \"current\": {\n";
  json += "    \"phaseA\": " + String(currentA) + ",\n";
  json += "    \"phaseB\": " + String(currentB) + ",\n";
  json += "    \"phaseC\": " + String(currentC) + "\n";
  json += "  },\n";
  json += "  \"energy\": {\n";
  json += "    \"total\": " + String(totalEnergy) + "\n";
  json += "  },\n";
  json += "  \"environment\": {\n";
  json += "    \"temperature\": " + String(temperature) + ",\n";
  json += "    \"humidity\": " + String(humidity) + "\n";
  json += "  }\n";
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleMetrics() {
  // Create Prometheus metrics format
  String metrics = "";
  
  // Add power metrics
  metrics += "# HELP energy_monitor_power_watts Current power consumption in watts\n";
  metrics += "# TYPE energy_monitor_power_watts gauge\n";
  metrics += "energy_monitor_power_watts{phase=\"A\"} " + String(activePowerA) + "\n";
  metrics += "energy_monitor_power_watts{phase=\"B\"} " + String(activePowerB) + "\n";
  metrics += "energy_monitor_power_watts{phase=\"C\"} " + String(activePowerC) + "\n";
  metrics += "energy_monitor_power_watts{phase=\"total\"} " + String(totalActivePower) + "\n";
  
  // Add voltage metrics
  metrics += "# HELP energy_monitor_voltage_volts Current voltage in volts\n";
  metrics += "# TYPE energy_monitor_voltage_volts gauge\n";
  metrics += "energy_monitor_voltage_volts{phase=\"A\"} " + String(voltageA) + "\n";
  metrics += "energy_monitor_voltage_volts{phase=\"B\"} " + String(voltageB) + "\n";
  metrics += "energy_monitor_voltage_volts{phase=\"C\"} " + String(voltageC) + "\n";
  
  // Add current metrics
  metrics += "# HELP energy_monitor_current_amps Current in amps\n";
  metrics += "# TYPE energy_monitor_current_amps gauge\n";
  metrics += "energy_monitor_current_amps{phase=\"A\"} " + String(currentA) + "\n";
  metrics += "energy_monitor_current_amps{phase=\"B\"} " + String(currentB) + "\n";
  metrics += "energy_monitor_current_amps{phase=\"C\"} " + String(currentC) + "\n";
  
  // Add energy metrics
  metrics += "# HELP energy_monitor_energy_wh Total energy in watt-hours\n";
  metrics += "# TYPE energy_monitor_energy_wh counter\n";
  metrics += "energy_monitor_energy_wh{phase=\"total\"} " + String(totalEnergy) + "\n";
  
  // Add environmental metrics
  metrics += "# HELP energy_monitor_temperature_celsius Temperature in degrees Celsius\n";
  metrics += "# TYPE energy_monitor_temperature_celsius gauge\n";
  metrics += "energy_monitor_temperature_celsius " + String(temperature) + "\n";
  
  metrics += "# HELP energy_monitor_humidity_percent Relative humidity in percent\n";
  metrics += "# TYPE energy_monitor_humidity_percent gauge\n";
  metrics += "energy_monitor_humidity_percent " + String(humidity) + "\n";
  
  server.send(200, "text/plain", metrics);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
}

void handleRoot() {
  String html = "<!DOCTYPE html>\n";
  html += "<html>\n";
  html += "<head>\n";
  html += "  <title>SM Energy GE3222M Monitor</title>\n";
  html += "  <meta name='viewport' content='width=device-width, initial-scale=1'>\n";
  html += "  <meta http-equiv='refresh' content='5'>\n";
  html += "  <style>\n";
  html += "    body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: #f5f5f5; }\n";
  html += "    .container { max-width: 800px; margin: 0 auto; background: white; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n";
  html += "    h1 { color: #333; text-align: center; margin-bottom: 30px; }\n";
  html += "    .card { background: #f9f9f9; border-radius: 5px; padding: 15px; margin-bottom: 20px; }\n";
  html += "    .card h2 { margin-top: 0; color: #555; font-size: 18px; }\n";
  html += "    .data-row { display: flex; justify-content: space-between; margin-bottom: 8px; }\n";
  html += "    .label { font-weight: bold; color: #666; }\n";
  html += "    .value { color: #333; }\n";
  html += "    .phase-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 15px; margin-top: 20px; }\n";
  html += "    .phase-card { background: #eaffff; padding: 15px; border-radius: 5px; }\n";
  html += "    .footer { text-align: center; margin-top: 30px; font-size: 14px; color: #999; }\n";
  html += "  </style>\n";
  html += "</head>\n";
  html += "<body>\n";
  html += "  <div class='container'>\n";
  html += "    <h1>SM Energy GE3222M Monitor</h1>\n";
  
  html += "    <div class='card'>\n";
  html += "      <h2>Power Summary</h2>\n";
  html += "      <div class='data-row'><span class='label'>Total Active Power:</span><span class='value'>" + String(totalActivePower, 2) + " W</span></div>\n";
  html += "      <div class='data-row'><span class='label'>Total Energy:</span><span class='value'>" + String(totalEnergy, 2) + " Wh</span></div>\n";
  html += "    </div>\n";
  
  html += "    <div class='card'>\n";
  html += "      <h2>Environment</h2>\n";
  html += "      <div class='data-row'><span class='label'>Temperature:</span><span class='value'>" + String(temperature, 1) + " °C</span></div>\n";
  html += "      <div class='data-row'><span class='label'>Humidity:</span><span class='value'>" + String(humidity, 1) + " %</span></div>\n";
  html += "    </div>\n";
  
  html += "    <div class='card'>\n";
  html += "      <h2>Phase Measurements</h2>\n";
  html += "      <div class='phase-grid'>\n";
  
  html += "        <div class='phase-card'>\n";
  html += "          <h3>Phase A</h3>\n";
  html += "          <div class='data-row'><span class='label'>Voltage:</span><span class='value'>" + String(voltageA, 1) + " V</span></div>\n";
  html += "          <div class='data-row'><span class='label'>Current:</span><span class='value'>" + String(currentA, 3) + " A</span></div>\n";
  html += "          <div class='data-row'><span class='label'>Power:</span><span class='value'>" + String(activePowerA, 1) + " W</span></div>\n";
  html += "        </div>\n";
  
  html += "        <div class='phase-card'>\n";
  html += "          <h3>Phase B</h3>\n";
  html += "          <div class='data-row'><span class='label'>Voltage:</span><span class='value'>" + String(voltageB, 1) + " V</span></div>\n";
  html += "          <div class='data-row'><span class='label'>Current:</span><span class='value'>" + String(currentB, 3) + " A</span></div>\n";
  html += "          <div class='data-row'><span class='label'>Power:</span><span class='value'>" + String(activePowerB, 1) + " W</span></div>\n";
  html += "        </div>\n";
  
  html += "        <div class='phase-card'>\n";
  html += "          <h3>Phase C</h3>\n";
  html += "          <div class='data-row'><span class='label'>Voltage:</span><span class='value'>" + String(voltageC, 1) + " V</span></div>\n";
  html += "          <div class='data-row'><span class='label'>Current:</span><span class='value'>" + String(currentC, 3) + " A</span></div>\n";
  html += "          <div class='data-row'><span class='label'>Power:</span><span class='value'>" + String(activePowerC, 1) + " W</span></div>\n";
  html += "        </div>\n";
  
  html += "      </div>\n";
  html += "    </div>\n";
  
  html += "    <div class='footer'>\n";
  html += "      <p>SM Energy GE3222M Smart Energy Monitor | <a href='/data'>JSON Data</a> | <a href='/metrics'>Prometheus Metrics</a></p>\n";
  html += "    </div>\n";
  html += "  </div>\n";
  html += "  <script>\n";
  html += "    // Auto-refresh the page every 5 seconds\n";
  html += "    setTimeout(function() { location.reload(); }, 5000);\n";
  html += "  </script>\n";
  html += "</body>\n";
  html += "</html>\n";
  
  server.send(200, "text/html", html);
}
