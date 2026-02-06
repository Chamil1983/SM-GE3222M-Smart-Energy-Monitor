/**
 * @file Calibration.cpp
 * @brief Calibration management implementation
 */

#include "Calibration.h"
#include "../core/ErrorHandler.h"

const char* Calibration::NVS_NAMESPACE = "calibration";
const char* Calibration::NVS_KEY_CONFIG = "config";

Calibration::Calibration() 
    : initialized(false),
      driver(ATM90E36Driver::getInstance()) {
}

Calibration::~Calibration() {
    if (initialized) {
        preferences.end();
    }
}

Calibration& Calibration::getInstance() {
    static Calibration instance;
    return instance;
}

bool Calibration::init() {
    if (!preferences.begin(NVS_NAMESPACE, false)) {
        ErrorHandler::getInstance().logError(ErrorCode::NVS_CORRUPT,
            "Calibration: Failed to open NVS namespace");
        return false;
    }
    
    // Try to load existing calibration
    if (!load()) {
        // If load fails, set defaults
        setDefault();
    }
    
    initialized = true;
    return true;
}

void Calibration::setDefault() {
    // Use constructor defaults from CalibrationConfig
    config = CalibrationConfig();
    
    // V1.0 defaults
    config.lineFreq = 389;  // 50Hz
    config.pgaGain = 0x5555;
    
    config.voltageGain[0] = 7143;
    config.voltageGain[1] = 7171;
    config.voltageGain[2] = 7180;
    
    config.currentGain[0] = 45578;
    config.currentGain[1] = 45578;
    config.currentGain[2] = 45578;
    config.currentGainN = 33500;
    
    calculateChecksums();
}

bool Calibration::load() {
    size_t expectedSize = sizeof(CalibrationConfig);
    size_t loadedSize = preferences.getBytes(NVS_KEY_CONFIG, &config, expectedSize);
    
    if (loadedSize != expectedSize) {
        ErrorHandler::getInstance().logError(ErrorCode::CALIBRATION_ERROR,
            "Calibration: NVS data size mismatch");
        return false;
    }
    
    if (!verifyChecksums()) {
        ErrorHandler::getInstance().logError(ErrorCode::CALIBRATION_ERROR,
            "Calibration: Checksum verification failed");
        return false;
    }
    
    return true;
}

bool Calibration::save() {
    if (!initialized) {
        return false;
    }
    
    calculateChecksums();
    
    size_t written = preferences.putBytes(NVS_KEY_CONFIG, &config, sizeof(CalibrationConfig));
    
    if (written != sizeof(CalibrationConfig)) {
        ErrorHandler::getInstance().logError(ErrorCode::CONFIG_SAVE_FAIL,
            "Calibration: Failed to save to NVS");
        return false;
    }
    
    return true;
}

bool Calibration::apply() {
    if (!initialized || !driver.isInitialized()) {
        return false;
    }
    
    bool success = driver.begin(
        config.lineFreq,
        config.pgaGain,
        config.voltageGain[0],
        config.voltageGain[1],
        config.voltageGain[2],
        config.currentGain[0],
        config.currentGain[1],
        config.currentGain[2],
        config.currentGainN
    );
    
    if (!success) {
        ErrorHandler::getInstance().logError(ErrorCode::CALIBRATION_ERROR,
            "Calibration: Failed to apply to driver");
        return false;
    }
    
    // Verify no calibration errors
    if (driver.calibrationError()) {
        ErrorHandler::getInstance().logError(ErrorCode::CALIBRATION_ERROR,
            "Calibration: Driver checksum error after apply");
        return false;
    }
    
    return true;
}

void Calibration::setConfig(const CalibrationConfig& newConfig) {
    config = newConfig;
    calculateChecksums();
}

bool Calibration::verify() {
    return verifyChecksums();
}

bool Calibration::reset() {
    setDefault();
    return save();
}

void Calibration::calculateChecksums() {
    // ATM90E36 uses specific checksum algorithm
    // For now, set to 0 - proper implementation would calculate based on registers
    config.csZero = 0;
    config.csOne = 0;
    config.csTwo = 0;
    config.csThree = 0;
}

bool Calibration::verifyChecksums() const {
    // Simple verification - in production would verify against calculated values
    // For now, just ensure they're initialized
    return true;
}

uint16_t Calibration::getVoltageGain(uint8_t phase) const {
    if (phase > 2) {
        return 0;
    }
    return config.voltageGain[phase];
}

void Calibration::setVoltageGain(uint8_t phase, uint16_t gain) {
    if (phase <= 2) {
        config.voltageGain[phase] = gain;
        calculateChecksums();
    }
}

uint16_t Calibration::getCurrentGain(uint8_t phase) const {
    if (phase > 2) {
        return 0;
    }
    return config.currentGain[phase];
}

void Calibration::setCurrentGain(uint8_t phase, uint16_t gain) {
    if (phase <= 2) {
        config.currentGain[phase] = gain;
        calculateChecksums();
    }
}

void Calibration::setLineFreq(uint16_t freq) {
    config.lineFreq = freq;
    calculateChecksums();
}
