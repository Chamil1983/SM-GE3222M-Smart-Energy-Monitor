#include "CalibrationManager.h"

CalibrationManager::CalibrationManager() {
}

bool CalibrationManager::loadCalibration(CalibrationConfig& config) {
    if (!m_preferences.begin(NVS_NAMESPACE, true)) {
        Logger::getInstance().error("CalibrationManager", "Failed to open NVS namespace");
        config = getDefaultCalibration();
        return false;
    }

    bool exists = m_preferences.isKey("lineFreq");
    
    if (!exists) {
        Logger::getInstance().info("CalibrationManager", "No calibration found in NVS, using defaults");
        m_preferences.end();
        config = getDefaultCalibration();
        return false;
    }

    config.lineFreq = m_preferences.getUShort("lineFreq", 0);
    config.pgaGain = m_preferences.getUShort("pgaGain", 0);
    config.pmpga = m_preferences.getUShort("pmpga", 0);
    
    config.pStartTh = m_preferences.getUShort("pStartTh", 0x08BD);
    config.qStartTh = m_preferences.getUShort("qStartTh", 0x0AEC);
    config.sStartTh = m_preferences.getUShort("sStartTh", 0x0AEC);
    config.pPhaseTh = m_preferences.getUShort("pPhaseTh", 0x00D8);
    config.qPhaseTh = m_preferences.getUShort("qPhaseTh", 0x00D8);
    config.sPhaseTh = m_preferences.getUShort("sPhaseTh", 0x00D8);
    
    size_t len = m_preferences.getBytes("calRegs", config.calRegs, sizeof(config.calRegs));
    if (len != sizeof(config.calRegs)) {
        Logger::getInstance().warn("CalibrationManager", "calRegs size mismatch");
    }
    
    len = m_preferences.getBytes("harCalRegs", config.harCalRegs, sizeof(config.harCalRegs));
    if (len != sizeof(config.harCalRegs)) {
        Logger::getInstance().warn("CalibrationManager", "harCalRegs size mismatch");
    }
    
    len = m_preferences.getBytes("measCalRegs", config.measCalRegs, sizeof(config.measCalRegs));
    if (len != sizeof(config.measCalRegs)) {
        Logger::getInstance().warn("CalibrationManager", "measCalRegs size mismatch");
    }
    
    m_preferences.end();
    
    Logger::getInstance().info("CalibrationManager", "Loaded calibration from NVS");
    return true;
}

bool CalibrationManager::saveCalibration(const CalibrationConfig& config) {
    if (!m_preferences.begin(NVS_NAMESPACE, false)) {
        Logger::getInstance().error("CalibrationManager", "Failed to open NVS namespace for write");
        return false;
    }

    m_preferences.putUShort("lineFreq", config.lineFreq);
    m_preferences.putUShort("pgaGain", config.pgaGain);
    m_preferences.putUShort("pmpga", config.pmpga);
    
    m_preferences.putUShort("pStartTh", config.pStartTh);
    m_preferences.putUShort("qStartTh", config.qStartTh);
    m_preferences.putUShort("sStartTh", config.sStartTh);
    m_preferences.putUShort("pPhaseTh", config.pPhaseTh);
    m_preferences.putUShort("qPhaseTh", config.qPhaseTh);
    m_preferences.putUShort("sPhaseTh", config.sPhaseTh);
    
    m_preferences.putBytes("calRegs", config.calRegs, sizeof(config.calRegs));
    m_preferences.putBytes("harCalRegs", config.harCalRegs, sizeof(config.harCalRegs));
    m_preferences.putBytes("measCalRegs", config.measCalRegs, sizeof(config.measCalRegs));
    
    m_preferences.end();
    
    Logger::getInstance().info("CalibrationManager", "Saved calibration to NVS");
    return true;
}

CalibrationConfig CalibrationManager::getDefaultCalibration() {
    CalibrationConfig config;
    
    // V1.0 Factory Defaults
    config.lineFreq = 389;  // 50Hz setting for ATM90E36
    config.pgaGain = 0x0505010101;  // PGA Gain configuration
    config.pmpga = 0;
    
    // Thresholds (V1.0 defaults)
    config.pStartTh = 0x08BD;
    config.qStartTh = 0x0AEC;
    config.sStartTh = 0x0AEC;
    config.pPhaseTh = 0x00D8;
    config.qPhaseTh = 0x00D8;
    config.sPhaseTh = 0x00D8;
    
    // Measurement calibration (V1.0 values)
    // Format: [A_UGain, A_IGain, A_UOffset, A_IOffset, B_UGain, B_IGain, B_UOffset, B_IOffset, 
    //          C_UGain, C_IGain, C_UOffset, C_IOffset, N_IGain, N_IOffset, CSThree]
    config.measCalRegs[0] = 7143;   // Phase A Voltage Gain
    config.measCalRegs[1] = 45578;  // Phase A Current Gain
    config.measCalRegs[2] = 0;      // Phase A Voltage Offset
    config.measCalRegs[3] = 0;      // Phase A Current Offset
    
    config.measCalRegs[4] = 7171;   // Phase B Voltage Gain
    config.measCalRegs[5] = 45578;  // Phase B Current Gain
    config.measCalRegs[6] = 0;      // Phase B Voltage Offset
    config.measCalRegs[7] = 0;      // Phase B Current Offset
    
    config.measCalRegs[8] = 7180;   // Phase C Voltage Gain
    config.measCalRegs[9] = 45578;  // Phase C Current Gain
    config.measCalRegs[10] = 0;     // Phase C Voltage Offset
    config.measCalRegs[11] = 0;     // Phase C Current Offset
    
    config.measCalRegs[12] = 45578; // Neutral Current Gain
    config.measCalRegs[13] = 0;     // Neutral Current Offset
    config.measCalRegs[14] = 0;     // Checksum (calculated by driver)
    
    // Power calibration registers (default to zero, can be tuned)
    for (int i = 0; i < 13; i++) {
        config.calRegs[i] = 0;
    }
    
    // Harmonic calibration registers (default to zero)
    for (int i = 0; i < 7; i++) {
        config.harCalRegs[i] = 0;
    }
    
    Logger::getInstance().debug("CalibrationManager", "Generated factory default calibration");
    
    return config;
}
