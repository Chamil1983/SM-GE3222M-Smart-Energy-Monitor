/**
 * @file ExportManager.h
 * @brief Data export functionality for logged meter data
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Export logged data as CSV or JSON with date range filtering
 */

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include "../../include/GlobalTypes.h"

/**
 * @class ExportManager
 * @brief Manages data export from DataLogger with filtering and formatting
 * 
 * Features:
 * - Export to CSV or JSON format
 * - Date/time range filtering
 * - Field selection
 * - Web endpoint integration
 * - Stream-based export for large datasets
 * - Progress tracking
 */
class ExportManager {
public:
    /**
     * @brief Export format types
     */
    enum class Format {
        CSV,        // Comma-separated values
        JSON        // JSON array of objects
    };

    /**
     * @brief Export options structure
     */
    struct ExportOptions {
        Format format;
        unsigned long startTime;    // Unix timestamp (0 = from beginning)
        unsigned long endTime;      // Unix timestamp (0 = to end)
        uint16_t fieldMask;         // Field selection mask (from DataLogger)
        bool includeHeader;         // Include CSV header or JSON metadata
        size_t maxRecords;          // Maximum records to export (0 = unlimited)
        
        ExportOptions() 
            : format(Format::CSV)
            , startTime(0)
            , endTime(0)
            , fieldMask(0xFFFF)
            , includeHeader(true)
            , maxRecords(0) {}
    };

    /**
     * @brief Export result structure
     */
    struct ExportResult {
        bool success;
        size_t recordCount;
        size_t bytesWritten;
        char errorMessage[64];
        
        ExportResult() 
            : success(false)
            , recordCount(0)
            , bytesWritten(0) {
            errorMessage[0] = '\0';
        }
    };

    /**
     * @brief Get singleton instance
     * @return Reference to ExportManager instance
     */
    static ExportManager& getInstance();

    /**
     * @brief Initialize export manager
     * @return true if successful, false on failure
     */
    bool init();

    /**
     * @brief Export data to file
     * @param outputPath Output file path
     * @param options Export options
     * @return Export result
     */
    ExportResult exportToFile(const char* outputPath, const ExportOptions& options);

    /**
     * @brief Export data to string
     * @param output Output string
     * @param options Export options
     * @return Export result
     */
    ExportResult exportToString(String& output, const ExportOptions& options);

    /**
     * @brief Export data via HTTP response stream
     * @param client HTTP client for streaming
     * @param options Export options
     * @return Export result
     */
    ExportResult exportToStream(Print& stream, const ExportOptions& options);

    /**
     * @brief Get available date range from log file
     * @param startTime Output: earliest timestamp
     * @param endTime Output: latest timestamp
     * @return true if successful
     */
    bool getAvailableRange(unsigned long& startTime, unsigned long& endTime);

    /**
     * @brief Estimate export size
     * @param options Export options
     * @return Estimated size in bytes (0 if error)
     */
    size_t estimateExportSize(const ExportOptions& options);

    /**
     * @brief Set source data file
     * @param filename Source file path
     */
    void setSourceFile(const char* filename);

    /**
     * @brief Get current source file
     * @return Source file path
     */
    const char* getSourceFile() const { return _sourceFile; }

private:
    ExportManager();
    ~ExportManager() = default;
    ExportManager(const ExportManager&) = delete;
    ExportManager& operator=(const ExportManager&) = delete;

    /**
     * @brief Export in CSV format
     * @param stream Output stream
     * @param options Export options
     * @return Export result
     */
    ExportResult exportCSV(Print& stream, const ExportOptions& options);

    /**
     * @brief Export in JSON format
     * @param stream Output stream
     * @param options Export options
     * @return Export result
     */
    ExportResult exportJSON(Print& stream, const ExportOptions& options);

    /**
     * @brief Write CSV header
     * @param stream Output stream
     * @param fieldMask Field selection mask
     */
    void writeCSVHeader(Print& stream, uint16_t fieldMask);

    /**
     * @brief Parse CSV line and check if within time range
     * @param line CSV line
     * @param startTime Start timestamp
     * @param endTime End timestamp
     * @return true if within range
     */
    bool isInTimeRange(const String& line, unsigned long startTime, unsigned long endTime);

    /**
     * @brief Extract timestamp from CSV line
     * @param line CSV line
     * @return Timestamp (0 if not found)
     */
    unsigned long extractTimestamp(const String& line);

    /**
     * @brief Convert CSV line to JSON object
     * @param line CSV line
     * @param obj Output JSON object
     * @param fieldMask Field selection mask
     */
    void csvLineToJson(const String& line, JsonObject& obj, uint16_t fieldMask);

    /**
     * @brief Get field name from mask bit
     * @param fieldBit Field bit position
     * @return Field name
     */
    const char* getFieldName(uint8_t fieldBit);

    bool _initialized;
    char _sourceFile[64];
};
