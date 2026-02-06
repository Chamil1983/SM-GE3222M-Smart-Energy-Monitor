/**
 * @file SPIFFSManager.h
 * @brief SPIFFS file system management for ESP32
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Handles SPIFFS mounting, formatting, and file operations
 */

#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <vector>

class SPIFFSManager {
public:
    // Singleton instance
    static SPIFFSManager& getInstance() {
        static SPIFFSManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    SPIFFSManager(const SPIFFSManager&) = delete;
    SPIFFSManager& operator=(const SPIFFSManager&) = delete;

    // Initialize and mount SPIFFS
    bool init(bool formatOnFail = true);

    // Format SPIFFS
    bool format();

    // Space information
    size_t getTotalSpace();
    size_t getUsedSpace();
    size_t getFreeSpace();
    float getUsagePercent();

    // File operations
    bool exists(const char* path);
    bool remove(const char* path);
    size_t getFileSize(const char* path);
    std::vector<String> listFiles(const char* dirname = "/");

    // Health check
    bool healthCheck();
    bool needsCleanup();

    // Status
    bool isMounted() const { return _mounted; }

private:
    SPIFFSManager() : _mounted(false), _totalSpace(0), _usedSpace(0) {}
    ~SPIFFSManager() {}

    bool _mounted;
    size_t _totalSpace;
    size_t _usedSpace;

    void updateSpaceInfo();
    void listFilesRecursive(File dir, const char* dirname, std::vector<String>& fileList);
};

