/**
 * @file SPIFFSManager.cpp
 * @brief SPIFFS file system management implementation
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "SPIFFSManager.h"
#include "../core/ErrorHandler.h"

bool SPIFFSManager::init(bool formatOnFail) {
    if (_mounted) {
        return true;
    }

    ErrorHandler::getInstance().logMessage(LogLevel::INFO, "Mounting SPIFFS...");

    if (!SPIFFS.begin(false)) {
        ErrorHandler::getInstance().logMessage(LogLevel::WARN, "SPIFFS mount failed");
        
        if (formatOnFail) {
            ErrorHandler::getInstance().logMessage(LogLevel::INFO, "Formatting SPIFFS...");
            if (!SPIFFS.begin(true)) {
                ErrorHandler::getInstance().setError(ErrorCode::SPIFFS_MOUNT_FAIL);
                return false;
            }
        } else {
            ErrorHandler::getInstance().setError(ErrorCode::SPIFFS_MOUNT_FAIL);
            return false;
        }
    }

    _mounted = true;
    updateSpaceInfo();

    ErrorHandler::getInstance().logMessage(LogLevel::INFO, 
        String("SPIFFS mounted - Total: ") + String(_totalSpace) + 
        " bytes, Used: " + String(_usedSpace) + " bytes");

    if (needsCleanup()) {
        ErrorHandler::getInstance().logMessage(LogLevel::WARN, 
            "SPIFFS storage > 90% full - cleanup recommended");
    }

    return true;
}

bool SPIFFSManager::format() {
    ErrorHandler::getInstance().logMessage(LogLevel::WARN, "Formatting SPIFFS...");
    
    if (_mounted) {
        SPIFFS.end();
        _mounted = false;
    }

    if (!SPIFFS.format()) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "SPIFFS format failed");
        return false;
    }

    ErrorHandler::getInstance().logMessage(LogLevel::INFO, "SPIFFS formatted successfully");
    return init(false);
}

void SPIFFSManager::updateSpaceInfo() {
    _totalSpace = SPIFFS.totalBytes();
    _usedSpace = SPIFFS.usedBytes();
}

size_t SPIFFSManager::getTotalSpace() {
    if (_mounted) {
        updateSpaceInfo();
    }
    return _totalSpace;
}

size_t SPIFFSManager::getUsedSpace() {
    if (_mounted) {
        updateSpaceInfo();
    }
    return _usedSpace;
}

size_t SPIFFSManager::getFreeSpace() {
    if (_mounted) {
        updateSpaceInfo();
    }
    return _totalSpace - _usedSpace;
}

float SPIFFSManager::getUsagePercent() {
    if (_totalSpace == 0) {
        return 0.0f;
    }
    return (float)_usedSpace / (float)_totalSpace * 100.0f;
}

bool SPIFFSManager::exists(const char* path) {
    if (!_mounted) {
        return false;
    }
    return SPIFFS.exists(path);
}

bool SPIFFSManager::remove(const char* path) {
    if (!_mounted) {
        return false;
    }

    if (!SPIFFS.exists(path)) {
        return false;
    }

    bool result = SPIFFS.remove(path);
    if (result) {
        ErrorHandler::getInstance().logMessage(LogLevel::INFO, 
            String("File deleted: ") + String(path));
        updateSpaceInfo();
    }
    return result;
}

size_t SPIFFSManager::getFileSize(const char* path) {
    if (!_mounted || !SPIFFS.exists(path)) {
        return 0;
    }

    File file = SPIFFS.open(path, "r");
    if (!file) {
        return 0;
    }

    size_t size = file.size();
    file.close();
    return size;
}

std::vector<String> SPIFFSManager::listFiles(const char* dirname) {
    std::vector<String> fileList;

    if (!_mounted) {
        return fileList;
    }

    File root = SPIFFS.open(dirname);
    if (!root) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "Failed to open directory");
        return fileList;
    }

    if (!root.isDirectory()) {
        root.close();
        return fileList;
    }

    listFilesRecursive(root, dirname, fileList);
    root.close();

    return fileList;
}

void SPIFFSManager::listFilesRecursive(File dir, const char* dirname, std::vector<String>& fileList) {
    File file = dir.openNextFile();
    
    while (file) {
        if (file.isDirectory()) {
            listFilesRecursive(file, file.name(), fileList);
        } else {
            fileList.push_back(String(file.name()));
        }
        file = dir.openNextFile();
    }
}

bool SPIFFSManager::healthCheck() {
    if (!_mounted) {
        return false;
    }

    updateSpaceInfo();

    // Check if SPIFFS is readable
    File testFile = SPIFFS.open("/health_check.tmp", "w");
    if (!testFile) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "SPIFFS health check failed - cannot write");
        return false;
    }
    testFile.print("test");
    testFile.close();

    // Try to read it back
    testFile = SPIFFS.open("/health_check.tmp", "r");
    if (!testFile) {
        ErrorHandler::getInstance().logMessage(LogLevel::ERROR, "SPIFFS health check failed - cannot read");
        return false;
    }
    testFile.close();

    // Clean up
    SPIFFS.remove("/health_check.tmp");

    ErrorHandler::getInstance().logMessage(LogLevel::DEBUG, 
        String("SPIFFS health OK - ") + String(getUsagePercent(), 1) + "% used");
    
    return true;
}

bool SPIFFSManager::needsCleanup() {
    return getUsagePercent() >= 90.0f;
}
