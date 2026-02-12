/**
 * @file DataLogger.cpp
 * @brief Ring buffer-based data logger implementation
 */

#include "DataLogger.h"
#include <new>
#include "Logger.h"
#include "SPIFFSManager.h"
#include <time.h>
#include <esp_heap_caps.h>
#include <esp_system.h>


DataLogger& DataLogger::getInstance() {
    static DataLogger instance;
    return instance;
}

DataLogger::DataLogger() 
    : _initialized(false), _maxEntries(200), _head(0), _count(0), _buffer(nullptr), _mutex(nullptr) {
}

DataLogger::~DataLogger() {
    if (_buffer) {
        delete[] _buffer;
    }
    if (_mutex) {
        vSemaphoreDelete(_mutex);
    }
}

bool DataLogger::init(size_t maxEntries) {
    if (_initialized) {
        return true;
    }
    
    Logger& logger = Logger::getInstance();
    
    _maxEntries = maxEntries;
    
    // Allocate ring buffer
    // Prefer PSRAM when available (large history buffers).
    if (psramFound()) {
        void* mem = heap_caps_malloc(sizeof(LoggedReading) * _maxEntries, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        _buffer = static_cast<LoggedReading*>(mem);
        if (_buffer) {
            // placement-new to run constructors (PhaseData/MeterData zeroing)
            for (size_t i = 0; i < _maxEntries; ++i) {
                new (&_buffer[i]) LoggedReading();
            }
            logger.info("DataLogger: Ring buffer allocated in PSRAM (%d entries)", _maxEntries);
        }
    }
    if (!_buffer) {
        _buffer = new (std::nothrow) LoggedReading[_maxEntries];
        if (!_buffer) {
            logger.error("Failed to allocate ring buffer for %d entries", _maxEntries);
            return false;
        }
        logger.info("DataLogger: Ring buffer allocated in heap (%d entries)", _maxEntries);
    }
    
    // Create mutex
    _mutex = xSemaphoreCreateMutex();
    if (!_mutex) {
        logger.error("Failed to create mutex for DataLogger");
        delete[] _buffer;
        _buffer = nullptr;
        return false;
    }
    
    _initialized = true;
    logger.info("DataLogger initialized with %d entry ring buffer", _maxEntries);
    
    return true;
}

void DataLogger::logReading(const MeterData& data) {
    if (!_initialized) {
        Logger::getInstance().warn("DataLogger not initialized");
        return;
    }
    
    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::getInstance().warn("Failed to acquire mutex for logging");
        return;
    }
    
    // Store reading in ring buffer
    _buffer[_head].data = data;
    _buffer[_head].timestamp = millis() / 1000; // Convert to seconds
    
    // Advance head pointer
    _head = (_head + 1) % _maxEntries;
    
    // Update count
    if (_count < _maxEntries) {
        _count++;
    }
    
    xSemaphoreGive(_mutex);
}

std::vector<LoggedReading> DataLogger::getRecentReadings(size_t count) {
    std::vector<LoggedReading> readings;
    
    if (!_initialized) {
        Logger::getInstance().warn("DataLogger not initialized");
        return readings;
    }
    
    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        Logger::getInstance().warn("Failed to acquire mutex for reading");
        return readings;
    }
    
    // Limit count to available entries
    if (count > _count) {
        count = _count;
    }
    
    // Reserve space
    readings.reserve(count);
    
    // Calculate starting index (working backwards from head)
    size_t startIdx;
    if (_head >= count) {
        startIdx = _head - count;
    } else {
        startIdx = _maxEntries - (count - _head);
    }
    
    // Copy readings
    for (size_t i = 0; i < count; i++) {
        size_t idx = (startIdx + i) % _maxEntries;
        readings.push_back(_buffer[idx]);
    }
    
    xSemaphoreGive(_mutex);
    
    return readings;
}

bool DataLogger::exportToCSV() {
    if (!_initialized) {
        Logger::getInstance().error("DataLogger not initialized");
        return false;
    }
    
    Logger& logger = Logger::getInstance();
    SPIFFSManager& spiffs = SPIFFSManager::getInstance();
    
    logger.info("Exporting %d readings to CSV...", _count);
    
    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(1000)) != pdTRUE) {
        logger.error("Failed to acquire mutex for CSV export");
        return false;
    }
    
    // Build CSV content
    String csv = generateCSVHeader();
    csv += "\n";
    
    // Calculate starting index
    size_t startIdx = (_count < _maxEntries) ? 0 : _head;
    
    // Add all readings
    for (size_t i = 0; i < _count; i++) {
        size_t idx = (startIdx + i) % _maxEntries;
        csv += generateCSVRow(_buffer[idx]);
        csv += "\n";
    }
    
    xSemaphoreGive(_mutex);
    
    // Generate filename with timestamp
    char filename[32];
    snprintf(filename, sizeof(filename), "/data_%lu.csv", millis() / 1000);
    
    // Write to SPIFFS
    if (!spiffs.writeFile(filename, csv)) {
        logger.error("Failed to write CSV to SPIFFS");
        return false;
    }
    
    logger.info("Exported to %s (%d bytes)", filename, csv.length());
    return true;
}

void DataLogger::clearBuffer() {
    if (!_initialized) {
        return;
    }
    
    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        _head = 0;
        _count = 0;
        Logger::getInstance().info("DataLogger buffer cleared");
        xSemaphoreGive(_mutex);
    }
}

size_t DataLogger::getBufferSize() const {
    return _maxEntries;
}

size_t DataLogger::getBufferCount() const {
    return _count;
}

String DataLogger::generateCSVHeader() {
    return "Timestamp,Seq,"
           "V_A,I_A,P_A,Q_A,S_A,PF_A,E_A_Fwd,"
           "V_B,I_B,P_B,Q_B,S_B,PF_B,E_B_Fwd,"
           "V_C,I_C,P_C,Q_C,S_C,PF_C,E_C_Fwd,"
           "P_Total,Q_Total,S_Total,PF_Total,E_Total_Fwd,"
           "I_N,Freq,Temp_Board,Temp_Ambient,Humidity";
}

String DataLogger::generateCSVRow(const LoggedReading& reading) {
    char row[512];
    snprintf(row, sizeof(row),
        "%lu,%lu,"
        "%.2f,%.2f,%.2f,%.2f,%.2f,%.3f,%.3f,"
        "%.2f,%.2f,%.2f,%.2f,%.2f,%.3f,%.3f,"
        "%.2f,%.2f,%.2f,%.2f,%.2f,%.3f,%.3f,"
        "%.2f,%.2f,%.2f,%.3f,%.3f,"
        "%.2f,%.2f,%.1f,%.1f,%.1f",
        reading.timestamp,
        reading.data.sequenceNumber,
        // Phase A
        reading.data.phaseA.voltageRMS,
        reading.data.phaseA.currentRMS,
        reading.data.phaseA.activePower,
        reading.data.phaseA.reactivePower,
        reading.data.phaseA.apparentPower,
        reading.data.phaseA.powerFactor,
        reading.data.phaseA.fwdActiveEnergy,
        // Phase B
        reading.data.phaseB.voltageRMS,
        reading.data.phaseB.currentRMS,
        reading.data.phaseB.activePower,
        reading.data.phaseB.reactivePower,
        reading.data.phaseB.apparentPower,
        reading.data.phaseB.powerFactor,
        reading.data.phaseB.fwdActiveEnergy,
        // Phase C
        reading.data.phaseC.voltageRMS,
        reading.data.phaseC.currentRMS,
        reading.data.phaseC.activePower,
        reading.data.phaseC.reactivePower,
        reading.data.phaseC.apparentPower,
        reading.data.phaseC.powerFactor,
        reading.data.phaseC.fwdActiveEnergy,
        // Totals
        reading.data.totalActivePower,
        reading.data.totalReactivePower,
        reading.data.totalApparentPower,
        reading.data.totalPowerFactor,
        reading.data.totalFwdActiveEnergy,
        // Other
        reading.data.neutralCurrent,
        reading.data.frequency,
        reading.data.boardTemperature,
        reading.data.ambientTemperature,
        reading.data.ambientHumidity
    );
    
    return String(row);
}
