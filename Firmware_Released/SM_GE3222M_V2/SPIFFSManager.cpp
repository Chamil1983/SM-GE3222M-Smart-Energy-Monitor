/**
 * @file SPIFFSManager.cpp
 * @brief SPIFFS file system manager implementation
 */

#include "SPIFFSManager.h"
#include "Logger.h"
#include <FS.h>

SPIFFSManager& SPIFFSManager::getInstance() {
    static SPIFFSManager instance;
    return instance;
}

SPIFFSManager::SPIFFSManager() : _initialized(false) {
}

SPIFFSManager::~SPIFFSManager() {
    if (_initialized) {
        SPIFFS.end();
    }
}

bool SPIFFSManager::init(bool formatOnFail) {
    if (_initialized) {
        return true;
    }
    
    Logger& logger = Logger::getInstance();
    
    if (!SPIFFS.begin(formatOnFail)) {
        logger.error("Failed to mount SPIFFS");
        return false;
    }
    
    _initialized = true;
    
    SPIFFSInfo info = getInfo();
    logger.info("SPIFFS mounted: %u/%u bytes used", info.usedBytes, info.totalBytes);
    
    return true;
}

SPIFFSInfo SPIFFSManager::getInfo() {
    SPIFFSInfo info;
    
    if (!_initialized) {
        Logger::getInstance().warn("SPIFFS not initialized");
        return info;
    }
    
    info.totalBytes = SPIFFS.totalBytes();
    info.usedBytes = SPIFFS.usedBytes();
    info.freeBytes = info.totalBytes - info.usedBytes;
    
    return info;
}

bool SPIFFSManager::fileExists(const String& path) {
    if (!_initialized) {
        Logger::getInstance().warn("SPIFFS not initialized");
        return false;
    }
    
    return SPIFFS.exists(path);
}

String SPIFFSManager::readFile(const String& path) {
    if (!_initialized) {
        Logger::getInstance().error("SPIFFS not initialized");
        return "";
    }
    
    if (!SPIFFS.exists(path)) {
        Logger::getInstance().warn("File does not exist: %s", path.c_str());
        return "";
    }
    
    File file = SPIFFS.open(path, "r");
    if (!file) {
        Logger::getInstance().error("Failed to open file for reading: %s", path.c_str());
        return "";
    }
    
    String content = file.readString();
    file.close();
    
    Logger::getInstance().debug("Read %d bytes from %s", content.length(), path.c_str());
    return content;
}

bool SPIFFSManager::writeFile(const String& path, const String& content) {
    if (!_initialized) {
        Logger::getInstance().error("SPIFFS not initialized");
        return false;
    }
    
    File file = SPIFFS.open(path, "w");
    if (!file) {
        Logger::getInstance().error("Failed to open file for writing: %s", path.c_str());
        return false;
    }
    
    size_t bytesWritten = file.print(content);
    file.close();
    
    if (bytesWritten != content.length()) {
        Logger::getInstance().error("Failed to write complete file: %s", path.c_str());
        return false;
    }
    
    Logger::getInstance().debug("Wrote %d bytes to %s", bytesWritten, path.c_str());
    return true;
}

bool SPIFFSManager::deleteFile(const String& path) {
    if (!_initialized) {
        Logger::getInstance().error("SPIFFS not initialized");
        return false;
    }
    
    if (!SPIFFS.exists(path)) {
        Logger::getInstance().warn("File does not exist: %s", path.c_str());
        return false;
    }
    
    if (!SPIFFS.remove(path)) {
        Logger::getInstance().error("Failed to delete file: %s", path.c_str());
        return false;
    }
    
    Logger::getInstance().debug("Deleted file: %s", path.c_str());
    return true;
}

std::vector<String> SPIFFSManager::listFiles(const String& dir) {
    std::vector<String> files;
    
    if (!_initialized) {
        Logger::getInstance().error("SPIFFS not initialized");
        return files;
    }
    
    File root = SPIFFS.open(dir);
    if (!root) {
        Logger::getInstance().error("Failed to open directory: %s", dir.c_str());
        return files;
    }
    
    if (!root.isDirectory()) {
        Logger::getInstance().error("Not a directory: %s", dir.c_str());
        root.close();
        return files;
    }
    
    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            files.push_back(String(file.name()));
        }
        file = root.openNextFile();
    }
    
    root.close();
    
    Logger::getInstance().debug("Listed %d files in %s", files.size(), dir.c_str());
    return files;
}
