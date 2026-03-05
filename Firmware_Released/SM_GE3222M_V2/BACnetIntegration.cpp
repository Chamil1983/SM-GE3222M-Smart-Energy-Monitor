#include "BACnetIntegration.h"

#include <WiFi.h>
#include <esp_wifi.h>

#include "BACnetDriver.h"
#include "EthernetManager.h"
#include "EnergyMeter.h"
#include "GPIOManager.h"
#include "SMNetworkManager.h"
#include "ConfigManager.h"
#include "SystemMonitor.h"
#include "NTPSync.h"
#include "PinMap.h"
#ifdef HARDWARE_VERSION
#undef HARDWARE_VERSION
#endif
#include "Version.h"
#include "Logger.h"

bool BACnetIntegration::_enabled = true;
bool BACnetIntegration::_initialized = false;
bool BACnetIntegration::_ledRemoteOverrideEnable = false;
bool BACnetIntegration::_exposeRawApPassword = false;
uint32_t BACnetIntegration::_lastSyncMs = 0;
uint32_t BACnetIntegration::_lastMetaMs = 0;
uint32_t BACnetIntegration::_lastNtpAttemptMs = 0;
bool BACnetIntegration::_ntpInit = false;
uint32_t BACnetIntegration::_lastNtpSyncUnix = 0;
uint16_t BACnetIntegration::_refreshIntervalMs = 500;
float BACnetIntegration::_dcDividerRatio = 11.0f; // default 100k/10k style divider
float BACnetIntegration::_dcGain = 1.0f;
float BACnetIntegration::_dcOffset = 0.0f;

namespace {
String formatMac(const uint8_t* m) {
    char b[18];
    snprintf(b, sizeof(b), "%02X:%02X:%02X:%02X:%02X:%02X", m[0],m[1],m[2],m[3],m[4],m[5]);
    return String(b);
}
String getIfaceMac(wifi_interface_t iface) {
    uint8_t m[6] = {0};
    if (esp_wifi_get_mac(iface, m) == ESP_OK) return formatMac(m);
    return String("00:00:00:00:00:00");
}
String boolStr(bool v) { return v ? "ON" : "OFF"; }
}

void BACnetIntegration::initialize() {
    if (_initialized) return;

    BACnetDriver::DeviceInfo di;
    di.deviceInstance = 88160;
    strncpy(di.objectName, "SM_GE3222M", sizeof(di.objectName)-1);
    strncpy(di.description, "MESA 3-Phase Smart Energy Monitor", sizeof(di.description)-1);
    strncpy(di.location, "Panel", sizeof(di.location)-1);
    strncpy(di.vendorName, "Microcode Embedded Systems and Automation", sizeof(di.vendorName)-1);
    di.vendorIdentifier = 999;
    strncpy(di.modelName, "SM_GE3222M_V2", sizeof(di.modelName)-1);
    strncpy(di.serialNumber, "AUTO", sizeof(di.serialNumber)-1);
    strncpy(di.firmwareRevision, FW_VERSION, sizeof(di.firmwareRevision)-1);
    strncpy(di.applicationSoftwareVersion, FW_VERSION, sizeof(di.applicationSoftwareVersion)-1);

    BACnetDriver& b = BACnetDriver::getInstance();
    b.setDeviceInfo(di);
    b.setRefreshIntervalMs(_refreshIntervalMs);
    b.setPeriodicIAm(true, 60);
    b.setVendorPropertyUnsigned(PROP_MESA_REFRESH_INTERVAL_MS, _refreshIntervalMs);
    b.setVendorPropertyString(PROP_MESA_HW_VERSION, "SM-GE3222M 1.1V");
    b.setVendorPropertyUnsigned(PROP_MESA_YEAR_DEVELOPED, 2026);
    b.setVendorPropertyString(PROP_MESA_COMM_STATUS, "INIT");

#if defined(ARDUINO_ARCH_ESP32)
    analogReadResolution(12);
    analogSetPinAttenuation(PIN_ADC_DCV_IN, ADC_11db);
#endif
    pinMode(PIN_ADC_DCV_IN, INPUT);

    _initialized = true;
    Logger::getInstance().info("BACnetIntegration: Initialized");
}

void BACnetIntegration::setEnabled(bool enabled) {
    _enabled = enabled;
    if (!enabled) BACnetDriver::getInstance().end();
}

bool BACnetIntegration::isEnabled() { return _enabled; }

void BACnetIntegration::update() {
    if (!_enabled) return;
    if (!_initialized) initialize();

    EthernetManager& eth = EthernetManager::getInstance();
    const bool ethReady = eth.isLinkUp() && eth.isIpReady();

    // Time sync should never run in WiFi AP-only mode without upstream.
    updateTimeSync();

    // Keep vendor properties updated (safe even if Ethernet has no IP).
    updateDeviceMetadataAndVendorProps();

    BACnetDriver& b = BACnetDriver::getInstance();
    _refreshIntervalMs = b.getRefreshIntervalMs();

    // Safety interlock: BACnet/IP MUST only run when Ethernet has a valid IP.
    if (!ethReady) {
        if (b.isRunning()) {
            b.end();
        }
        return;
    }

    if (!b.isRunning()) {
        b.begin(BACNET_DEFAULT_UDP_PORT);
    }

    // Process a bounded number of packets each cycle to protect CPU and WDT.
    b.task(4);

    const uint32_t now = millis();
    if ((uint32_t)(now - _lastSyncMs) < _refreshIntervalMs) return;
    _lastSyncMs = now;

    applyBinaryOutputCommands();
    updateAnalogInputs();
    updateBinaryInputs();
    updateBinaryOutputs();
}


void BACnetIntegration::updateTimeSync() {
    // Only attempt NTP time sync when we have a route to the internet.
    // WiFi AP-only mode has no upstream and can trigger unstable behavior on some stacks if configTime() is called.
    const bool wifiStaReady = networkManager.isSTAMode() && networkManager.isConnected();
    const bool ethReady = EthernetManager::getInstance().isIpReady() && EthernetManager::getInstance().isLinkUp();

    if (!wifiStaReady && !ethReady) return;

    // Extra guard: if we're only in AP mode (no STA) and Ethernet isn't ready, skip NTP completely.
    if (!wifiStaReady && networkManager.isAPMode() && !ethReady) return;

    NetworkConfig netCfg;
    ConfigManager::getInstance().loadNetworkConfig(netCfg);

    if (!_ntpInit) {
        // Validate NTP server string to avoid configTime() with empty/invalid value.
        if (strlen(netCfg.ntpServer) == 0) return;
        _ntpInit = NTPSync::getInstance().init(netCfg.ntpServer, netCfg.timezoneOffset);
    }

    const uint32_t nowMs = millis();
    if (_ntpInit && ((uint32_t)(nowMs - _lastNtpAttemptMs) > 60000UL) && !NTPSync::getInstance().isTimeSynced()) {
        _lastNtpAttemptMs = nowMs;
        if (NTPSync::getInstance().sync(1500)) {
            _lastNtpSyncUnix = NTPSync::getInstance().getTimestamp();
        }
    }
}


void BACnetIntegration::updateDeviceMetadataAndVendorProps() {
    const uint32_t now = millis();
    if ((uint32_t)(now - _lastMetaMs) < 1000UL) return;
    _lastMetaMs = now;

    BACnetDriver& b = BACnetDriver::getInstance();
    EthernetManager& eth = EthernetManager::getInstance();
    const auto& es = eth.getStatus();

    // Dynamic vendor properties (identity/network/RTC/comm interface monitoring)
    b.setVendorPropertyString(PROP_MESA_MAC_ADDRESS, networkManager.getMacAddress());
    b.setVendorPropertyString(PROP_MESA_ETH_MAC_ADDRESS, eth.macAddressString());
    b.setVendorPropertyString(PROP_MESA_ETH_IP_ADDRESS, eth.localIPString());
    b.setVendorPropertyString(PROP_MESA_ETH_SUBNET, eth.subnetString());
    b.setVendorPropertyString(PROP_MESA_ETH_GATEWAY, eth.gatewayString());
    b.setVendorPropertyString(PROP_MESA_ETH_DNS, eth.dnsString());
    b.setVendorPropertyString(PROP_MESA_SUBNET, eth.subnetString());
    b.setVendorPropertyString(PROP_MESA_GATEWAY, eth.gatewayString());
    b.setVendorPropertyString(PROP_MESA_DNS1, eth.dnsString());
    b.setVendorPropertyString(PROP_MESA_AP_SSID, AP_SSID);
    b.setVendorPropertyString(PROP_MESA_AP_PASS, _exposeRawApPassword ? String(AP_PASS) : maskedApPassword());
    b.setVendorPropertyString(PROP_MESA_AP_IP, String(AP_IP_ADDR));
    b.setVendorPropertyString(PROP_MESA_STA_IP_ADDRESS, WiFi.localIP().toString());
    b.setVendorPropertyString(PROP_MESA_STA_MAC_ADDRESS, getIfaceMac(WIFI_IF_STA));
    b.setVendorPropertyString(PROP_MESA_AP_MAC_ADDRESS, getIfaceMac(WIFI_IF_AP));
    b.setVendorPropertyUnsigned(PROP_MESA_BACNET_PORT, BACNET_DEFAULT_UDP_PORT);
    b.setVendorPropertyString(PROP_MESA_DEVICE_DATETIME, NTPSync::getInstance().isTimeSynced() ? NTPSync::getInstance().getTimeString() : String("UNSYNCED"));
    b.setVendorPropertyBool(PROP_MESA_TIME_SYNC_STATUS, NTPSync::getInstance().isTimeSynced());
    b.setVendorPropertyUnsigned(PROP_MESA_LAST_SYNC_UNIX, _lastNtpSyncUnix);
    b.setVendorPropertyUnsigned(PROP_MESA_REFRESH_INTERVAL_MS, b.getRefreshIntervalMs());

    // Communication interface monitoring/config summary strings (requested "section" for user visibility)
    ModbusConfig mb; ConfigManager::getInstance().loadModbusConfig(mb);
    String modbusSummary = String("RTU=") + boolStr(mb.rtuEnabled)
        + ",TCP=" + boolStr(mb.tcpEnabled)
        + ",ID=" + String(mb.slaveID)
        + ",Baud=" + String(mb.baudrate)
        + ",DB=" + String(mb.dataBits)
        + ",SB=" + String(mb.stopBits)
        + ",Parity=" + String(mb.parity)
        + ",TCPPort=" + String(mb.tcpPort);
    b.setVendorPropertyString(PROP_MESA_MODBUS_CONFIG, modbusSummary);

    WiFiConfig wc = networkManager.getConfig();
    String staSummary = String("SSID=") + (wc.ssid.length() ? wc.ssid : "<empty>")
        + ",DHCP=" + boolStr(wc.useDHCP)
        + ",IP=" + wc.ip + ",GW=" + wc.gw + ",SN=" + wc.sn + ",DNS1=" + wc.dns1 + ",DNS2=" + wc.dns2
        + ",STA_MAC=" + getIfaceMac(WIFI_IF_STA);
    b.setVendorPropertyString(PROP_MESA_WIFI_STA_CONFIG, staSummary);

    String apSummary = String("SSID=") + AP_SSID
        + ",PASS=" + (_exposeRawApPassword ? String(AP_PASS) : maskedApPassword())
        + ",IP=" + String(AP_IP_ADDR)
        + ",GW=" + String(AP_GATEWAY)
        + ",SN=" + String(AP_SUBNET)
        + ",AP_MAC=" + getIfaceMac(WIFI_IF_AP);
    b.setVendorPropertyString(PROP_MESA_WIFI_AP_CONFIG, apSummary);

    String ethSummary = String("EN=") + boolStr(eth.isEnabled())
        + ",HW=" + boolStr(es.hardwareDetected)
        + ",LINK=" + boolStr(es.linkUp)
        + ",DHCP=" + boolStr(es.dhcpActive)
        + ",IP=" + eth.localIPString()
        + ",SN=" + eth.subnetString()
        + ",GW=" + eth.gatewayString()
        + ",DNS=" + eth.dnsString()
        + ",MAC=" + eth.macAddressString();
    b.setVendorPropertyString(PROP_MESA_ETH_CONFIG, ethSummary);

    String commStatus = String("WiFi=") + (networkManager.isConnected() ? (networkManager.isAPMode() ? "AP" : "STA") : "OFF")
        + ",Eth=" + (eth.isLinkUp() ? "UP" : "DOWN")
        + ",EthIP=" + eth.localIPString()
        + ",BACnet=" + (b.isRunning() ? "RUN" : "STOP");
    b.setVendorPropertyString(PROP_MESA_COMM_STATUS, commStatus);
}

float BACnetIntegration::readScaledDCVoltage() {
    uint32_t acc = 0;
    constexpr uint8_t samples = 8;
    for (uint8_t i = 0; i < samples; ++i) {
        acc += analogRead(PIN_ADC_DCV_IN);
        delayMicroseconds(200);
    }
    float raw = (float)acc / (float)samples;
    float pinV = (raw / (float)ADC_MAX_VALUE) * ADC_VREF;
    return ((pinV * _dcDividerRatio) * _dcGain) + _dcOffset;
}

void BACnetIntegration::updateAnalogInputs() {
    BACnetDriver& b = BACnetDriver::getInstance();
    MeterData m = EnergyMeter::getInstance().getSnapshot();

    // Phase voltages/currents/power/frequency/energy (subset + requested mapping)
    b.updateAnalogInput(1,  m.phaseA.voltageRMS, BACNET_UNITS_VOLTS,   "PhaseA_Voltage", "Phase A Voltage RMS");
    b.updateAnalogInput(2,  m.phaseB.voltageRMS, BACNET_UNITS_VOLTS,   "PhaseB_Voltage", "Phase B Voltage RMS");
    b.updateAnalogInput(3,  m.phaseC.voltageRMS, BACNET_UNITS_VOLTS,   "PhaseC_Voltage", "Phase C Voltage RMS");

    b.updateAnalogInput(11, m.phaseA.currentRMS, BACNET_UNITS_AMPERES, "PhaseA_Current", "Phase A Current RMS");
    b.updateAnalogInput(12, m.phaseB.currentRMS, BACNET_UNITS_AMPERES, "PhaseB_Current", "Phase B Current RMS");
    b.updateAnalogInput(13, m.phaseC.currentRMS, BACNET_UNITS_AMPERES, "PhaseC_Current", "Phase C Current RMS");

    b.updateAnalogInput(21, m.phaseA.activePower, BACNET_UNITS_WATTS, "PhaseA_ActivePower", "Phase A Active Power");
    b.updateAnalogInput(22, m.phaseB.activePower, BACNET_UNITS_WATTS, "PhaseB_ActivePower", "Phase B Active Power");
    b.updateAnalogInput(23, m.phaseC.activePower, BACNET_UNITS_WATTS, "PhaseC_ActivePower", "Phase C Active Power");

    b.updateAnalogInput(31, m.phaseA.powerFactor, BACNET_UNITS_NO_UNITS, "PhaseA_PF", "Phase A Power Factor");
    b.updateAnalogInput(32, m.phaseB.powerFactor, BACNET_UNITS_NO_UNITS, "PhaseB_PF", "Phase B Power Factor");
    b.updateAnalogInput(33, m.phaseC.powerFactor, BACNET_UNITS_NO_UNITS, "PhaseC_PF", "Phase C Power Factor");

    b.updateAnalogInput(40, m.totalActivePower, BACNET_UNITS_WATTS, "Total_ActivePower", "Total Active Power");
    b.updateAnalogInput(41, m.totalReactivePower, BACNET_UNITS_VOLT_AMPERES_REACTIVE, "Total_ReactivePower", "Total Reactive Power");
    b.updateAnalogInput(42, m.totalApparentPower, BACNET_UNITS_VOLT_AMPERES, "Total_ApparentPower", "Total Apparent Power");
    b.updateAnalogInput(43, m.totalPowerFactor, BACNET_UNITS_NO_UNITS, "Total_PF", "Total Power Factor");
    b.updateAnalogInput(44, m.frequency, BACNET_UNITS_HERTZ, "Frequency", "Line Frequency");
    b.updateAnalogInput(45, m.neutralCurrent, BACNET_UNITS_AMPERES, "Neutral_Current", "Neutral Current");

    b.updateAnalogInput(50, m.boardTemperature, BACNET_UNITS_DEGREES_CELSIUS, "Board_Temperature", "Board Temperature");
    b.updateAnalogInput(51, m.ambientTemperature, BACNET_UNITS_DEGREES_CELSIUS, "Ambient_Temperature", "Ambient Temperature");
    b.updateAnalogInput(52, m.ambientHumidity, BACNET_UNITS_PERCENT, "Ambient_Humidity", "Ambient Humidity");

    b.updateAnalogInput(60, m.totalFwdActiveEnergy, BACNET_UNITS_KILOWATT_HOURS, "Energy_kWh_Fwd", "Total Forward Active Energy");
    b.updateAnalogInput(61, m.totalRevActiveEnergy, BACNET_UNITS_KILOWATT_HOURS, "Energy_kWh_Rev", "Total Reverse Active Energy");
    b.updateAnalogInput(62, m.totalFwdReactiveEnergy, BACNET_UNITS_KILOVAR_HOURS, "Energy_kVARh_Fwd", "Total Forward Reactive Energy");
    b.updateAnalogInput(63, m.totalRevReactiveEnergy, BACNET_UNITS_KILOVAR_HOURS, "Energy_kVARh_Rev", "Total Reverse Reactive Energy");

    // Requested GPIO36 scaled DC voltage input
    b.updateAnalogInput(70, readScaledDCVoltage(), BACNET_UNITS_VOLTS, "DC_Input_Voltage", "Scaled DC voltage @ GPIO36 ADC1_CH0");

    BACnetDriver::getInstance().setVendorPropertyUnsigned(PROP_MESA_LAST_SYNC_UNIX, _lastNtpSyncUnix);
}

void BACnetIntegration::updateBinaryInputs() {
    BACnetDriver& b = BACnetDriver::getInstance();
    MeterData m = EnergyMeter::getInstance().getSnapshot();
    SystemStatus s = SystemMonitor::getInstance().getSystemStatus();
    EthernetManager& eth = EthernetManager::getInstance();

    b.updateBinaryInput(1, m.valid, "Meter_Data_Valid", "Energy meter snapshot valid");
    b.updateBinaryInput(2, !GPIOManager::getInstance().readPortB(MCP_PORTB_ATM_WARN), "ATM_Warn", "ATM90E36 warning input (active low)");
    b.updateBinaryInput(3, s.wifiConnected, "WiFi_Connected", "WiFi connected");
    b.updateBinaryInput(4, s.wifiAPMode, "WiFi_AP_Mode", "Soft AP active");
    b.updateBinaryInput(5, eth.isLinkUp(), "Ethernet_Link", "W5500 Ethernet link up");
    b.updateBinaryInput(6, BACnetDriver::getInstance().isRunning(), "BACnet_Running", "BACnet/IP service running");
    b.updateBinaryInput(7, s.modbusActive, "Modbus_Active", "Modbus service active");
    b.updateBinaryInput(8, NTPSync::getInstance().isTimeSynced(), "Time_Synced", "RTC/NTP time synced");

    // Requested MCP Port B diagnostic/status objects (read-only)
    b.updateBinaryInput(20, !GPIOManager::getInstance().readPortB(MCP_PORTB_IRQ_1), "IRQ_1", "MCP PortB IRQ_1 (active low if wired)");
    b.updateBinaryInput(21, !GPIOManager::getInstance().readPortB(MCP_PORTB_IRQ_2), "IRQ_2", "MCP PortB IRQ_2 (active low if wired)");
    b.updateBinaryInput(22, !GPIOManager::getInstance().readPortB(MCP_PORTB_ATM_WARN), "ATM_WARN", "ATM90E36 warning input");
    b.updateBinaryInput(23, !GPIOManager::getInstance().readPortB(MCP_PORTB_CF1), "CF1", "CF1 pulse input state");
    b.updateBinaryInput(24, !GPIOManager::getInstance().readPortB(MCP_PORTB_CF2), "CF2", "CF2 pulse input state");
    b.updateBinaryInput(25, !GPIOManager::getInstance().readPortB(MCP_PORTB_CF3), "CF3", "CF3 pulse input state");
    b.updateBinaryInput(26, !GPIOManager::getInstance().readPortB(MCP_PORTB_CF4), "CF4", "CF4 pulse input state");
}

void BACnetIntegration::updateBinaryOutputs() {
    BACnetDriver& b = BACnetDriver::getInstance();
    // Relays and LEDs are active-low outputs on Port A
    auto readLogical = [](uint8_t portaPin) {
        return GPIOManager::getInstance().readPortA(portaPin) == LOW;
    };

    b.updateBinaryOutput(1, readLogical(MCP_PORTA_RELAY_1), "Relay_1", "Relay 1 output");
    b.updateBinaryOutput(2, readLogical(MCP_PORTA_RELAY_2), "Relay_2", "Relay 2 output");
    b.updateBinaryOutput(3, readLogical(MCP_PORTA_WIFI_LED), "WiFi_LED", "WiFi Status LED (GPA2)");
    b.updateBinaryOutput(4, readLogical(MCP_PORTA_RUN_LED), "Run_LED", "Run LED Green (GPA3)");
    b.updateBinaryOutput(5, readLogical(MCP_PORTA_FLT_LED), "Fault_LED", "Fault LED Red (GPA4)");
    b.updateBinaryOutput(6, readLogical(MCP_PORTA_MODBUS_LED), "Modbus_LED", "Modbus Activity LED (GPA5)");
}

void BACnetIntegration::applyBinaryOutputCommands() {
    BACnetDriver& b = BACnetDriver::getInstance();
    bool v = false;

    if (b.getBinaryOutputCommand(1, v)) GPIOManager::getInstance().setRelay(Relay::RELAY_1, v);
    if (b.getBinaryOutputCommand(2, v)) GPIOManager::getInstance().setRelay(Relay::RELAY_2, v);

    // LED BOs (requested). Gate writes behind override enable for safety.
    if (_ledRemoteOverrideEnable) {
        if (b.getBinaryOutputCommand(3, v)) GPIOManager::getInstance().setLED(LED::WIFI, v);
        if (b.getBinaryOutputCommand(4, v)) GPIOManager::getInstance().setLED(LED::RUN, v);
        if (b.getBinaryOutputCommand(5, v)) GPIOManager::getInstance().setLED(LED::FAULT, v);
        if (b.getBinaryOutputCommand(6, v)) GPIOManager::getInstance().setLED(LED::MODBUS, v);
    } else {
        // Drain pending commands so client doesn't see stale queue; reflect current state on next updateBinaryOutputs().
        if (b.getBinaryOutputCommand(3, v)) {}
        if (b.getBinaryOutputCommand(4, v)) {}
        if (b.getBinaryOutputCommand(5, v)) {}
        if (b.getBinaryOutputCommand(6, v)) {}
    }
}

String BACnetIntegration::maskedApPassword() {
    size_t n = strlen(AP_PASS);
    if (n == 0) return String("");
    String s; s.reserve(n);
    for (size_t i = 0; i < n; ++i) s += '*';
    return s;
}

