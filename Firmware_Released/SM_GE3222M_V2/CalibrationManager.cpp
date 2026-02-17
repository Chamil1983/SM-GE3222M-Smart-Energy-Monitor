#include "CalibrationManager.h"

CalibrationManager::CalibrationManager() {
}

bool CalibrationManager::loadCalibration(CalibrationConfig& config) {
    if (!m_preferences.begin(NVS_NAMESPACE, false)) {
        Logger::getInstance().error("CalibrationManager: Failed to open NVS namespace");
        config = getDefaultCalibration();
        return false;
    }

    bool exists = m_preferences.isKey("lineFreq");
    
    if (!exists) {
        Logger::getInstance().info("CalibrationManager: No calibration found in NVS, using defaults");
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
        Logger::getInstance().warn("CalibrationManager: calRegs size mismatch");
    }
    
    len = m_preferences.getBytes("harCalRegs", config.harCalRegs, sizeof(config.harCalRegs));
    if (len != sizeof(config.harCalRegs)) {
        Logger::getInstance().warn("CalibrationManager: harCalRegs size mismatch");
    }
    
    len = m_preferences.getBytes("measCalRegs", config.measCalRegs, sizeof(config.measCalRegs));
    if (len != sizeof(config.measCalRegs)) {
        Logger::getInstance().warn("CalibrationManager: measCalRegs size mismatch");
    }
    
    m_preferences.end();
    
    Logger::getInstance().info("CalibrationManager: Loaded calibration from NVS");
    return true;
}

bool CalibrationManager::saveCalibration(const CalibrationConfig& config) {
    if (!m_preferences.begin(NVS_NAMESPACE, false)) {
        Logger::getInstance().error("CalibrationManager: Failed to open NVS namespace for write");
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
    
    Logger::getInstance().info("CalibrationManager: Saved calibration to NVS");
    return true;
}

CalibrationConfig CalibrationManager::getDefaultCalibration() {
    CalibrationConfig config;

    // Factory defaults extracted from SM_GE3222M V1.0 -> Definitions.h (ATM90E36 section)
    // LineFreq: MMODE0 value for 50Hz 3P4W
    config.lineFreq = 389;          // 0x0185
    config.pgaGain  = 0x557F;       // 0b0101010101111111 (MMODE1)

    // V1.0 uses 0 for thresholds by default (can be tuned later)
    config.pmpga    = 0;
    config.pStartTh = 0x0000;
    config.qStartTh = 0x0000;
    config.sStartTh = 0x0000;
    config.pPhaseTh = 0x0000;
    config.qPhaseTh = 0x0000;
    config.sPhaseTh = 0x0000;

    // Measurement calibration (V1.0 values)
    // Format: [A_UGain, A_IGain, A_UOffset, A_IOffset, B_UGain, B_IGain, B_UOffset, B_IOffset,
    //          C_UGain, C_IGain, C_UOffset, C_IOffset, N_IGain, N_IOffset, CSThree]
    //
    // Voltage gains
    const uint16_t vGain1 = 38800;  // VoltageGain1
    const uint16_t vGain2 = 38500;  // VoltageGain2
    const uint16_t vGain3 = 38800;  // VoltageGain3

    // Current gains (CT1/CT2/CT3/Neutral)
    const uint16_t iGainA = 45578;  // CurrentGainCT1
    const uint16_t iGainB = 45578;  // CurrentGainCT2
    const uint16_t iGainC = 45578;  // CurrentGainCT3
    const uint16_t iGainN = 45578;  // CurrentGainCTN

    // Phase A
    config.measCalRegs[0] = vGain1;
    config.measCalRegs[1] = iGainA;
    config.measCalRegs[2] = 0;
    config.measCalRegs[3] = 0;

    // Phase B
    config.measCalRegs[4] = vGain2;
    config.measCalRegs[5] = iGainB;
    config.measCalRegs[6] = 0;
    config.measCalRegs[7] = 0;

    // Phase C
    config.measCalRegs[8]  = vGain3;
    config.measCalRegs[9]  = iGainC;
    config.measCalRegs[10] = 0;
    config.measCalRegs[11] = 0;

    // Neutral
    config.measCalRegs[12] = iGainN;
    config.measCalRegs[13] = 0;

    // Checksum placeholder (not used by V2 wrapper)
    config.measCalRegs[14] = 0;

    Logger::getInstance().info("CalibrationManager: Using V1.0 factory defaults (ATM90E36)");

    return config;
}
