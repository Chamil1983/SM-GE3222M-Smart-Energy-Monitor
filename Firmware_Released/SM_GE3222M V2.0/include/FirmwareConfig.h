/**
 * @file FirmwareConfig.h
 * @brief Build-time feature flags and configuration
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Compile-time configuration options
 */

#pragma once

// ============================================================
// Feature Flags - Enable/Disable Components
// ============================================================

// Communication features
#define ENABLE_WIFI                 1   ///< Enable WiFi support
#define ENABLE_ETHERNET             1   ///< Enable W5500 Ethernet
#define ENABLE_MODBUS_RTU           1   ///< Enable Modbus RTU slave
#define ENABLE_MODBUS_TCP           1   ///< Enable Modbus TCP server
#define ENABLE_MQTT                 1   ///< Enable MQTT client
#define ENABLE_WEB_SERVER           1   ///< Enable web server + REST API
#define ENABLE_WEBSOCKET            1   ///< Enable WebSocket real-time push
#define ENABLE_TCP_DATA_SERVER      1   ///< Enable VB.NET TCP protocol server
#define ENABLE_OTA                  1   ///< Enable OTA firmware updates
#define ENABLE_MDNS                 1   ///< Enable mDNS responder

// Display features
#define ENABLE_LCD                  1   ///< Enable LCD display
#define ENABLE_OLED                 0   ///< Enable OLED display (alternative to LCD)
#define ENABLE_LED_INDICATORS       1   ///< Enable status LEDs
#define ENABLE_BUTTONS              1   ///< Enable physical buttons

// Energy metering features
#define ENABLE_POWER_QUALITY        1   ///< Enable power quality analysis
#define ENABLE_DEMAND_MONITOR       1   ///< Enable demand monitoring
#define ENABLE_ENERGY_ACCUMULATOR   1   ///< Enable energy accumulation
#define ENABLE_HARMONIC_ANALYSIS    1   ///< Enable THD/harmonic measurements

// Storage features
#define ENABLE_SPIFFS               1   ///< Enable SPIFFS file system
#define ENABLE_DATA_LOGGING         1   ///< Enable data logging to SPIFFS
#define ENABLE_CONFIG_MANAGER       1   ///< Enable configuration management
#define ENABLE_NVS                  1   ///< Enable NVS storage

// Security features
#define ENABLE_AUTH                 1   ///< Enable web authentication
#define ENABLE_ACCESS_CONTROL       1   ///< Enable role-based access control
#define ENABLE_CRYPTO               1   ///< Enable CRC32/hashing

// Diagnostics features
#define ENABLE_SELF_TEST            1   ///< Enable boot-time self-test
#define ENABLE_HEALTH_MONITOR       1   ///< Enable runtime health monitoring
#define ENABLE_LOGGING              1   ///< Enable serial/file logging
#define ENABLE_ERROR_HANDLER        1   ///< Enable error handler subsystem

// Watchdog features
#define ENABLE_HARDWARE_WATCHDOG    1   ///< Enable hardware watchdog
#define ENABLE_SOFTWARE_WATCHDOG    1   ///< Enable per-task software watchdog

// ============================================================
// Hardware Configuration
// ============================================================

// MCU
#define MCU_FREQ_MHZ                240     ///< CPU frequency (MHz)
#define CORE_COUNT                  2       ///< Dual-core ESP32

// Memory
#define HEAP_WARNING_THRESHOLD      20480   ///< Warn if free heap below 20KB
#define HEAP_CRITICAL_THRESHOLD     10240   ///< Critical if free heap below 10KB
#define STACK_SIZE_DEFAULT          4096    ///< Default task stack size (bytes)
#define ENABLE_PSRAM                0       ///< Enable PSRAM (if available)

// SPI Configuration
#define SPI_FREQ_ATM90E36           200000  ///< 200 kHz for ATM90E36 (conservative)
#define SPI_FREQ_W5500              8000000 ///< 8 MHz for W5500 Ethernet
#define SPI_MAX_RETRY               3       ///< Max SPI retry attempts

// I2C Configuration
#define I2C_FREQ                    100000  ///< 100 kHz I2C clock
#define I2C_TIMEOUT_MS              100     ///< I2C timeout (ms)

// ============================================================
// Task Configuration
// ============================================================

#define TASK_ENERGY_INTERVAL_MS     100     ///< Energy reading interval
#define TASK_MODBUS_INTERVAL_MS     50      ///< Modbus polling interval
#define TASK_WEBSOCKET_INTERVAL_MS  1000    ///< WebSocket push interval
#define TASK_LCD_INTERVAL_MS        500     ///< LCD update interval
#define TASK_LED_INTERVAL_MS        100     ///< LED update interval
#define TASK_MQTT_INTERVAL_MS       5000    ///< MQTT publish interval
#define TASK_WIFI_CHECK_INTERVAL_MS 5000    ///< WiFi connection check interval
#define TASK_HEALTH_INTERVAL_MS     10000   ///< Health monitoring interval
#define TASK_DATALOG_INTERVAL_MS    30000   ///< Data logging interval
#define TASK_ENERGY_PERSIST_INTERVAL_MS 60000   ///< Energy persistence interval

// ============================================================
// Network Configuration Defaults
// ============================================================

// WiFi
#define WIFI_CONNECT_TIMEOUT_MS     15000   ///< WiFi connection timeout
#define WIFI_RECONNECT_DELAY_MS     5000    ///< Reconnection delay
#define WIFI_MAX_RECONNECT_ATTEMPTS 10      ///< Max reconnect attempts
#define WIFI_AP_CHANNEL             6       ///< AP mode channel
#define WIFI_AP_MAX_CONNECTIONS     4       ///< Max AP clients

// Web Server
#define WEB_SERVER_PORT             80      ///< HTTP port
#define WEB_SOCKET_PORT             81      ///< WebSocket port (or same as HTTP)
#define WEB_MAX_CLIENTS             4       ///< Max concurrent web clients
#define WEB_SESSION_TIMEOUT_SEC     1800    ///< Session timeout (30 min)
#define WEB_RATE_LIMIT_REQ_PER_MIN  100     ///< Rate limit requests/minute

// Modbus
#define MODBUS_RTU_BAUD             9600    ///< Default Modbus RTU baud rate
#define MODBUS_RTU_SLAVE_ID         1       ///< Default Modbus slave ID
#define MODBUS_TCP_PORT             502     ///< Modbus TCP port
#define MODBUS_MAX_REGISTERS        250     ///< Max register count

// MQTT
#define MQTT_PORT                   1883    ///< Default MQTT port
#define MQTT_KEEPALIVE_SEC          60      ///< MQTT keepalive interval
#define MQTT_QOS                    0       ///< Default QoS (0, 1, or 2)
#define MQTT_RECONNECT_DELAY_MS     5000    ///< MQTT reconnect delay

// TCP Data Server (VB.NET)
#define TCP_DATA_SERVER_PORT        8088    ///< VB.NET TCP protocol port
#define TCP_MAX_CLIENTS             2       ///< Max TCP clients

// Ethernet
#define ETHERNET_DHCP_TIMEOUT_MS    30000   ///< DHCP timeout

// ============================================================
// Measurement Configuration
// ============================================================

// Filtering
#define ENERGY_FILTER_SAMPLES       5       ///< Moving average filter samples
#define ENERGY_FILTER_ENABLE        1       ///< Enable measurement filtering

// Validation ranges
#define VOLTAGE_MIN                 0.0     ///< Min valid voltage (V)
#define VOLTAGE_MAX                 500.0   ///< Max valid voltage (V)
#define CURRENT_MIN                 0.0     ///< Min valid current (A)
#define CURRENT_MAX                 100.0   ///< Max valid current (A)
#define FREQUENCY_MIN               45.0    ///< Min valid frequency (Hz)
#define FREQUENCY_MAX               65.0    ///< Max valid frequency (Hz)

// Power Quality Thresholds
#define PQ_VOLTAGE_SAG_PERCENT      90      ///< Voltage sag threshold (% of nominal)
#define PQ_VOLTAGE_SWELL_PERCENT    110     ///< Voltage swell threshold (% of nominal)
#define PQ_THD_WARNING_PERCENT      5.0     ///< THD warning threshold (%)
#define PQ_THD_CRITICAL_PERCENT     10.0    ///< THD critical threshold (%)
#define PQ_IMBALANCE_WARNING_PERCENT 5.0    ///< Phase imbalance warning (%)

// Demand Monitor
#define DEMAND_INTERVAL_MIN         15      ///< Demand calculation interval (minutes)
#define DEMAND_ROLLING_SAMPLES      4       ///< Rolling demand samples

// ============================================================
// Storage Configuration
// ============================================================

// SPIFFS
#define SPIFFS_MAX_FILES            10      ///< Max open files
#define SPIFFS_MAX_PATH_LEN         64      ///< Max file path length

// Data Logging
#define DATALOG_MAX_SIZE_KB         512     ///< Max log file size (KB)
#define DATALOG_ROTATION_COUNT      10      ///< Number of rotated log files

// NVS Namespaces
#define NVS_NAMESPACE_CONFIG        "config"        ///< Configuration namespace
#define NVS_NAMESPACE_CALIBRATION   "calibration"   ///< Calibration namespace
#define NVS_NAMESPACE_ENERGY        "energy"        ///< Energy accumulators namespace
#define NVS_NAMESPACE_ERRORS        "errors"        ///< Error log namespace
#define NVS_NAMESPACE_SYSTEM        "system"        ///< System data namespace

// Ring Buffer
#define RINGBUFFER_SIZE_METER_DATA  1440    ///< 24h at 1-min intervals
#define RINGBUFFER_SIZE_ERROR_LOG   50      ///< Last 50 errors

// ============================================================
// Watchdog Configuration
// ============================================================

#define WATCHDOG_TIMEOUT_SEC        8       ///< Hardware watchdog timeout
#define WATCHDOG_FEED_INTERVAL_MS   5000    ///< Watchdog feed interval

// ============================================================
// Debug Configuration
// ============================================================

#define DEBUG_SERIAL_ENABLED        1       ///< Enable serial debug output
#define DEBUG_SERIAL_BAUD           115200  ///< Serial baud rate
#define DEBUG_LOG_LEVEL             3       ///< 0=OFF, 1=ERROR, 2=WARN, 3=INFO, 4=DEBUG

// Performance monitoring
#define ENABLE_TASK_STATS           1       ///< Enable task runtime statistics
#define ENABLE_MEMORY_STATS         1       ///< Enable memory usage tracking

// ============================================================
// OTA Configuration
// ============================================================

#define OTA_HOSTNAME_PREFIX         "SM-GE3222M"    ///< OTA hostname prefix
#define OTA_PASSWORD                "admin"         ///< OTA password - CRITICAL: MUST BE CHANGED IN PRODUCTION!
                                                    ///< This default is INSECURE and should be overridden in ConfigManager
                                                    ///< or set at compile time for production builds
#define OTA_PORT                    3232            ///< OTA port

// ============================================================
// Version Information
// ============================================================

// Defined in Version.h
// Included here for reference
