/**
 * @file ExportManager.cpp
 * @brief Data export functionality implementation
 */

#include "ExportManager.h"
#include "DataLogger.h"
#include "SPIFFSManager.h"
#include "../core/Logger.h"
#include <SPIFFS.h>

ExportManager& ExportManager::getInstance() {
    static ExportManager instance;
    return instance;
}

ExportManager::ExportManager()
    : _initialized(false) {
    strcpy(_sourceFile, "/datalog.csv");
}

bool ExportManager::init() {
    if (_initialized) {
        return true;
    }

    // Verify SPIFFS is mounted
    if (!SPIFFS.begin(true)) {
        Logger::error("ExportManager", "SPIFFS not available");
        return false;
    }

    _initialized = true;
    Logger::info("ExportManager", "Initialized successfully");
    return true;
}

ExportManager::ExportResult ExportManager::exportToFile(const char* outputPath, 
                                                         const ExportOptions& options) {
    ExportResult result;

    if (!_initialized) {
        strcpy(result.errorMessage, "Not initialized");
        return result;
    }

    if (outputPath == nullptr) {
        strcpy(result.errorMessage, "Invalid output path");
        return result;
    }

    // Open output file
    File outFile = SPIFFS.open(outputPath, FILE_WRITE);
    if (!outFile) {
        strcpy(result.errorMessage, "Failed to open output file");
        Logger::error("ExportManager", result.errorMessage);
        return result;
    }

    // Export to file stream
    result = exportToStream(outFile, options);
    outFile.close();

    return result;
}

ExportManager::ExportResult ExportManager::exportToString(String& output, 
                                                           const ExportOptions& options) {
    ExportResult result;

    if (!_initialized) {
        strcpy(result.errorMessage, "Not initialized");
        return result;
    }

    // Create a string stream
    output = "";
    
    // Use a temporary file approach for large exports
    // For small exports, we can build directly into string
    size_t estimatedSize = estimateExportSize(options);
    
    if (estimatedSize > 0 && estimatedSize < 32768) {  // 32KB threshold
        output.reserve(estimatedSize);
        result = exportToStream(output, options);
    } else {
        strcpy(result.errorMessage, "Export too large for string");
        Logger::warn("ExportManager", "Export exceeds string buffer size");
    }

    return result;
}

ExportManager::ExportResult ExportManager::exportToStream(Print& stream, 
                                                           const ExportOptions& options) {
    if (!_initialized) {
        ExportResult result;
        strcpy(result.errorMessage, "Not initialized");
        return result;
    }

    // Dispatch to format-specific export function
    if (options.format == Format::CSV) {
        return exportCSV(stream, options);
    } else {
        return exportJSON(stream, options);
    }
}

bool ExportManager::getAvailableRange(unsigned long& startTime, unsigned long& endTime) {
    if (!_initialized) {
        return false;
    }

    File file = SPIFFS.open(_sourceFile, FILE_READ);
    if (!file) {
        return false;
    }

    startTime = 0;
    endTime = 0;
    bool firstLine = true;

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        
        if (line.length() == 0 || line.startsWith("timestamp")) {
            continue;  // Skip empty lines and header
        }

        unsigned long timestamp = extractTimestamp(line);
        if (timestamp > 0) {
            if (firstLine) {
                startTime = timestamp;
                firstLine = false;
            }
            endTime = timestamp;
        }
    }

    file.close();
    return (startTime > 0 && endTime > 0);
}

size_t ExportManager::estimateExportSize(const ExportOptions& options) {
    if (!_initialized) {
        return 0;
    }

    File file = SPIFFS.open(_sourceFile, FILE_READ);
    if (!file) {
        return 0;
    }

    size_t recordCount = 0;
    size_t avgLineSize = 0;
    size_t totalSize = 0;

    // Sample first 10 lines to estimate average size
    for (int i = 0; i < 10 && file.available(); i++) {
        String line = file.readStringUntil('\n');
        avgLineSize += line.length();
        recordCount++;
    }

    if (recordCount > 0) {
        avgLineSize /= recordCount;
        
        // Count total lines
        size_t totalLines = recordCount;
        while (file.available()) {
            file.readStringUntil('\n');
            totalLines++;
        }

        // Estimate size based on format
        if (options.format == Format::CSV) {
            totalSize = totalLines * avgLineSize;
        } else {
            // JSON adds overhead
            totalSize = totalLines * (avgLineSize + 50);  // ~50 bytes JSON overhead per record
        }
    }

    file.close();
    return totalSize;
}

void ExportManager::setSourceFile(const char* filename) {
    if (filename != nullptr) {
        strncpy(_sourceFile, filename, sizeof(_sourceFile) - 1);
        _sourceFile[sizeof(_sourceFile) - 1] = '\0';
    }
}

ExportManager::ExportResult ExportManager::exportCSV(Print& stream, 
                                                      const ExportOptions& options) {
    ExportResult result;

    File file = SPIFFS.open(_sourceFile, FILE_READ);
    if (!file) {
        strcpy(result.errorMessage, "Source file not found");
        Logger::error("ExportManager", result.errorMessage);
        return result;
    }

    // Write header if requested
    if (options.includeHeader) {
        writeCSVHeader(stream, options.fieldMask);
        result.bytesWritten += 1;  // newline
    }

    // Process each line
    bool firstLine = true;
    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();

        if (line.length() == 0) {
            continue;
        }

        // Skip original header
        if (firstLine && line.startsWith("timestamp")) {
            firstLine = false;
            continue;
        }

        // Check time range
        if (options.startTime > 0 || options.endTime > 0) {
            if (!isInTimeRange(line, options.startTime, options.endTime)) {
                continue;
            }
        }

        // Check max records
        if (options.maxRecords > 0 && result.recordCount >= options.maxRecords) {
            break;
        }

        // Write line
        stream.println(line);
        result.bytesWritten += line.length() + 1;
        result.recordCount++;
    }

    file.close();
    result.success = true;

    Logger::info("ExportManager", ("Exported " + String(result.recordCount) + " records (CSV)").c_str());
    return result;
}

ExportManager::ExportResult ExportManager::exportJSON(Print& stream, 
                                                       const ExportOptions& options) {
    ExportResult result;

    File file = SPIFFS.open(_sourceFile, FILE_READ);
    if (!file) {
        strcpy(result.errorMessage, "Source file not found");
        Logger::error("ExportManager", result.errorMessage);
        return result;
    }

    // Start JSON array
    stream.print("{");
    result.bytesWritten += 1;

    // Add metadata if requested
    if (options.includeHeader) {
        stream.print("\"metadata\":{");
        stream.print("\"source\":\"");
        stream.print(_sourceFile);
        stream.print("\",\"format\":\"JSON\",");
        stream.print("\"exportTime\":");
        stream.print(millis());
        stream.print("},");
        result.bytesWritten += 100;  // approximate
    }

    stream.print("\"data\":[");
    result.bytesWritten += 8;

    // Process each line
    bool firstRecord = true;
    bool skipHeader = true;
    
    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();

        if (line.length() == 0) {
            continue;
        }

        // Skip original header
        if (skipHeader && line.startsWith("timestamp")) {
            skipHeader = false;
            continue;
        }

        // Check time range
        if (options.startTime > 0 || options.endTime > 0) {
            if (!isInTimeRange(line, options.startTime, options.endTime)) {
                continue;
            }
        }

        // Check max records
        if (options.maxRecords > 0 && result.recordCount >= options.maxRecords) {
            break;
        }

        // Add comma between records
        if (!firstRecord) {
            stream.print(",");
            result.bytesWritten += 1;
        }
        firstRecord = false;

        // Convert CSV line to JSON
        DynamicJsonDocument doc(512);
        JsonObject obj = doc.to<JsonObject>();
        csvLineToJson(line, obj, options.fieldMask);

        String jsonLine;
        serializeJson(obj, jsonLine);
        stream.print(jsonLine);
        
        result.bytesWritten += jsonLine.length();
        result.recordCount++;
    }

    // Close JSON array and object
    stream.print("]}");
    result.bytesWritten += 2;

    file.close();
    result.success = true;

    Logger::info("ExportManager", ("Exported " + String(result.recordCount) + " records (JSON)").c_str());
    return result;
}

void ExportManager::writeCSVHeader(Print& stream, uint16_t fieldMask) {
    bool firstField = true;
    
    for (uint8_t i = 0; i < 16; i++) {
        if (fieldMask & (1 << i)) {
            if (!firstField) {
                stream.print(",");
            }
            firstField = false;
            stream.print(getFieldName(i));
        }
    }
    stream.println();
}

bool ExportManager::isInTimeRange(const String& line, unsigned long startTime, 
                                   unsigned long endTime) {
    unsigned long timestamp = extractTimestamp(line);
    
    if (timestamp == 0) {
        return false;
    }

    if (startTime > 0 && timestamp < startTime) {
        return false;
    }

    if (endTime > 0 && timestamp > endTime) {
        return false;
    }

    return true;
}

unsigned long ExportManager::extractTimestamp(const String& line) {
    int commaIndex = line.indexOf(',');
    if (commaIndex <= 0) {
        return 0;
    }

    String timestampStr = line.substring(0, commaIndex);
    return timestampStr.toInt();
}

void ExportManager::csvLineToJson(const String& line, JsonObject& obj, uint16_t fieldMask) {
    int startIndex = 0;
    uint8_t fieldIndex = 0;

    for (uint8_t i = 0; i < 16 && startIndex < line.length(); i++) {
        int commaIndex = line.indexOf(',', startIndex);
        if (commaIndex < 0) {
            commaIndex = line.length();
        }

        String value = line.substring(startIndex, commaIndex);
        
        if (fieldMask & (1 << i)) {
            const char* fieldName = getFieldName(i);
            
            // Try to parse as float, otherwise use as string
            if (i == 0) {  // Timestamp
                obj[fieldName] = value.toInt();
            } else {
                obj[fieldName] = value.toFloat();
            }
        }

        startIndex = commaIndex + 1;
    }
}

const char* ExportManager::getFieldName(uint8_t fieldBit) {
    // Map field bits to names (from DataLogger::LogField)
    static const char* fieldNames[] = {
        "timestamp",        // 0x0001
        "voltageA",         // 0x0002
        "voltageB",         // 0x0004
        "voltageC",         // 0x0008
        "currentA",         // 0x0010
        "currentB",         // 0x0020
        "currentC",         // 0x0040
        "powerTotal",       // 0x0080
        "powerFactor",      // 0x0100
        "frequency",        // 0x0200
        "temperature",      // 0x0400
        "energyImport",     // 0x0800
        "energyExport",     // 0x1000
        "field13",
        "field14",
        "field15"
    };

    if (fieldBit < 16) {
        return fieldNames[fieldBit];
    }
    return "unknown";
}
