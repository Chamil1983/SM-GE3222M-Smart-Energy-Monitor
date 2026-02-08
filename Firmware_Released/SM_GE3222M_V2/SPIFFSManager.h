/**
 * @file SPIFFSManager.h
 * @brief SPIFFS file system manager for SM-GE3222M V2.0
 * @details Singleton pattern - Manages SPIFFS operations
 */

#ifndef SPIFFS_MANAGER_H
#define SPIFFS_MANAGER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <vector>

struct SPIFFSInfo {
    size_t totalBytes;
    size_t usedBytes;
    size_t freeBytes;
    
    SPIFFSInfo() : totalBytes(0), usedBytes(0), freeBytes(0) {}
};

class SPIFFSManager {
public:
    static SPIFFSManager& getInstance();
    
    bool init(bool formatOnFail = true);
    SPIFFSInfo getInfo();
    
    bool fileExists(const String& path);
    String readFile(const String& path);
    bool writeFile(const String& path, const String& content);
    bool deleteFile(const String& path);
    std::vector<String> listFiles(const String& dir = "/");
    
private:
    SPIFFSManager();
    ~SPIFFSManager();
    SPIFFSManager(const SPIFFSManager&) = delete;
    SPIFFSManager& operator=(const SPIFFSManager&) = delete;
    
    bool _initialized;
};

#endif // SPIFFS_MANAGER_H
