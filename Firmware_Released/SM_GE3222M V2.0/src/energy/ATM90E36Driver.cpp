/**
 * @file ATM90E36Driver.cpp
 * @brief Implementation of ATM90E36 energy metering IC driver
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 */

#include "ATM90E36Driver.h"

// ============================================================
// Singleton Implementation
// ============================================================

ATM90E36Driver::ATM90E36Driver() 
    : initialized(false),
      lineFreq(0),
      pgagain(0),
      ugain1(0),
      ugain2(0),
      ugain3(0),
      igainA(0),
      igainB(0),
      igainC(0),
      igainN(0) {
}

ATM90E36Driver::~ATM90E36Driver() {
}

ATM90E36Driver& ATM90E36Driver::getInstance() {
    static ATM90E36Driver instance;
    return instance;
}

// ============================================================
// Core SPI Communication
// ============================================================

unsigned short ATM90E36Driver::CommEnergyIC(unsigned char rw, unsigned short address, unsigned short val) {
    SPIBus& spi = SPIBus::getInstance();
    
    if (!spi.isInitialized()) {
        ErrorHandler::getInstance().reportError(ErrorCode::SPI_COMM_FAIL, "ATM90E36", "SPI bus not initialized");
        return 0xFFFF;
    }
    
    unsigned short output;
    unsigned short address1;
    
    // Swap MSB and LSB of value
    output = (val >> 8) | (val << 8);
    val = output;
    
    // Set R/W flag
    address |= rw << 15;
    
    // Swap byte address
    address1 = (address >> 8) | (address << 8);
    address = address1;
    
    // Begin SPI transaction
    if (!spi.beginTransaction(SPIDevice::ATM90E36)) {
        ErrorHandler::getInstance().reportError(ErrorCode::SPI_COMM_FAIL, "ATM90E36", "Failed to begin SPI transaction");
        return 0xFFFF;
    }
    
    // Chip select LOW
    digitalWrite(PIN_SPI_CS_ATM, LOW);
    delayMicroseconds(10);
    
    // Write address (2 bytes)
    unsigned char* adata = (unsigned char*)&address;
    for (byte i = 0; i < 2; i++) {
        spi.transfer8(*adata);
        adata++;
    }
    
    // Wait 4us for data to become valid
    delayMicroseconds(4);
    
    // Read or write data (2 bytes)
    unsigned char* data = (unsigned char*)&val;
    if (rw) {
        // READ operation
        for (byte i = 0; i < 2; i++) {
            *data = spi.transfer8(0x00);
            data++;
        }
    } else {
        // WRITE operation
        for (byte i = 0; i < 2; i++) {
            spi.transfer8(*data);
            data++;
        }
    }
    
    // Chip select HIGH
    digitalWrite(PIN_SPI_CS_ATM, HIGH);
    delayMicroseconds(10);
    
    spi.endTransaction();
    
    // Reverse MSB and LSB for return value
    output = (val >> 8) | (val << 8);
    return output;
}

int ATM90E36Driver::Read32Register(signed short regh_addr, signed short regl_addr) {
    int val, val_h, val_l;
    
    // 3-read mechanism for reliability
    val_h = CommEnergyIC(READ, regh_addr, 0xFFFF);
    val_l = CommEnergyIC(READ, regl_addr, 0xFFFF);
    val = CommEnergyIC(READ, regh_addr, 0xFFFF);
    
    val = val_h << 16;
    val |= val_l;
    
    return val;
}

// ============================================================
// Initialization
// ============================================================

bool ATM90E36Driver::begin(unsigned short lineFreq, unsigned short pgagain,
                            unsigned short ugain1, unsigned short ugain2, unsigned short ugain3,
                            unsigned short igainA, unsigned short igainB, unsigned short igainC,
                            unsigned short igainN) {
    
    // Store configuration
    this->lineFreq = lineFreq;
    this->pgagain = pgagain;
    this->ugain1 = ugain1;
    this->ugain2 = ugain2;
    this->ugain3 = ugain3;
    this->igainA = igainA;
    this->igainB = igainB;
    this->igainC = igainC;
    this->igainN = igainN;
    
    // Initialize SPI bus
    SPIBus& spi = SPIBus::getInstance();
    if (!spi.isInitialized()) {
        if (!spi.init()) {
            ErrorHandler::getInstance().reportError(ErrorCode::SPI_COMM_FAIL, "ATM90E36", "SPI initialization failed");
            return false;
        }
    }
    
    // Configure CS pin
    pinMode(PIN_SPI_CS_ATM, OUTPUT);
    digitalWrite(PIN_SPI_CS_ATM, HIGH);
    
    // Calculate voltage sag threshold based on frequency
    unsigned short vSagTh;
    unsigned short sagV;
    if (lineFreq == 4485 || lineFreq == 5231) {
        sagV = 90;  // 60Hz configuration
    } else {
        sagV = 190; // 50Hz configuration
    }
    vSagTh = (sagV * 100 * sqrt(2)) / (2 * ugain1 / 32768);
    
    // Soft reset
    CommEnergyIC(WRITE, SoftReset, 0x789A);
    delay(100);
    
    // Function enable registers
    CommEnergyIC(WRITE, FuncEn0, 0x0000);
    CommEnergyIC(WRITE, FuncEn1, 0x0000);
    CommEnergyIC(WRITE, SagTh, 0x0001);
    
    // Configuration registers
    CommEnergyIC(WRITE, ConfigStart, 0x5678);
    CommEnergyIC(WRITE, PLconstH, 0x0861);
    CommEnergyIC(WRITE, PLconstL, 0xC468);
    CommEnergyIC(WRITE, MMode0, lineFreq);
    CommEnergyIC(WRITE, MMode1, pgagain);
    CommEnergyIC(WRITE, PStartTh, 0x0000);
    CommEnergyIC(WRITE, QStartTh, 0x0000);
    CommEnergyIC(WRITE, SStartTh, 0x0000);
    CommEnergyIC(WRITE, PPhaseTh, 0x0000);
    CommEnergyIC(WRITE, QPhaseTh, 0x0000);
    CommEnergyIC(WRITE, SPhaseTh, 0x0000);
    CommEnergyIC(WRITE, CSZero, 0x4741);
    
    // Calibration registers
    CommEnergyIC(WRITE, CalStart, 0x5678);
    CommEnergyIC(WRITE, GainA, 0x0000);
    CommEnergyIC(WRITE, PhiA, 0x0000);
    CommEnergyIC(WRITE, GainB, 0x0000);
    CommEnergyIC(WRITE, PhiB, 0x0000);
    CommEnergyIC(WRITE, GainC, 0x0000);
    CommEnergyIC(WRITE, PhiC, 0x0000);
    CommEnergyIC(WRITE, PoffsetA, 0x0000);
    CommEnergyIC(WRITE, QoffsetA, 0x0000);
    CommEnergyIC(WRITE, PoffsetB, 0x0000);
    CommEnergyIC(WRITE, QoffsetB, 0x0000);
    CommEnergyIC(WRITE, PoffsetC, 0x0000);
    CommEnergyIC(WRITE, QoffsetC, 0x0000);
    CommEnergyIC(WRITE, CSOne, 0x0000);
    
    // Harmonic calibration registers
    CommEnergyIC(WRITE, HarmStart, 0x5678);
    CommEnergyIC(WRITE, POffsetAF, 0x0000);
    CommEnergyIC(WRITE, POffsetBF, 0x0000);
    CommEnergyIC(WRITE, POffsetCF, 0x0000);
    CommEnergyIC(WRITE, PGainAF, 0x0000);
    CommEnergyIC(WRITE, PGainBF, 0x0000);
    CommEnergyIC(WRITE, PGainCF, 0x0000);
    CommEnergyIC(WRITE, CSTwo, 0x0000);
    
    // Measurement calibration registers
    CommEnergyIC(WRITE, AdjStart, 0x5678);
    
    // Phase A
    CommEnergyIC(WRITE, UgainA, ugain1);
    CommEnergyIC(WRITE, IgainA, igainA);
    CommEnergyIC(WRITE, UoffsetA, 0x0000);
    CommEnergyIC(WRITE, IoffsetA, 0x0000);
    
    // Phase B
    CommEnergyIC(WRITE, UgainB, ugain2);
    CommEnergyIC(WRITE, IgainB, igainB);
    CommEnergyIC(WRITE, UoffsetB, 0x0000);
    CommEnergyIC(WRITE, IoffsetB, 0x0000);
    
    // Phase C
    CommEnergyIC(WRITE, UgainC, ugain3);
    CommEnergyIC(WRITE, IgainC, igainC);
    CommEnergyIC(WRITE, UoffsetC, 0x0000);
    CommEnergyIC(WRITE, IoffsetC, 0x0000);
    
    // Neutral
    CommEnergyIC(WRITE, IgainN, igainN);
    CommEnergyIC(WRITE, IoffsetN, 0x0000);
    CommEnergyIC(WRITE, CSThree, 0x02F6);
    
    delay(100);
    
    // Verify initialization by checking if we can read a known register
    unsigned short sysStatus = GetSysStatus0();
    if (sysStatus == 0xFFFF) {
        ErrorHandler::getInstance().reportError(ErrorCode::ATM_INIT_FAIL, "ATM90E36", "Failed to communicate with IC");
        return false;
    }
    
    initialized = true;
    return true;
}

bool ATM90E36Driver::calibrationError() {
    bool CS0, CS1, CS2, CS3;
    unsigned short systemstatus0 = GetSysStatus0();
    
    CS0 = (systemstatus0 & 0x4000) != 0;
    CS1 = (systemstatus0 & 0x1000) != 0;
    CS2 = (systemstatus0 & 0x0400) != 0;
    CS3 = (systemstatus0 & 0x0100) != 0;
    
    if (CS0 || CS1 || CS2 || CS3) {
        ErrorHandler::getInstance().reportError(ErrorCode::CALIBRATION_ERROR, "ATM90E36", 
            "Checksum error detected");
        return true;
    }
    
    return false;
}

// ============================================================
// Voltage Measurements
// ============================================================

double ATM90E36Driver::GetLineVoltage1() {
    unsigned short voltage = CommEnergyIC(READ, UrmsA, 0xFFFF);
    return (double)voltage / 100.0;
}

double ATM90E36Driver::GetLineVoltage2() {
    unsigned short voltage = CommEnergyIC(READ, UrmsB, 0xFFFF);
    return (double)voltage / 100.0;
}

double ATM90E36Driver::GetLineVoltage3() {
    unsigned short voltage = CommEnergyIC(READ, UrmsC, 0xFFFF);
    return (double)voltage / 100.0;
}

// ============================================================
// Current Measurements
// ============================================================

double ATM90E36Driver::GetLineCurrentCT1() {
    unsigned short current = CommEnergyIC(READ, IrmsA, 0xFFFF);
    return (double)current / 1000.0;
}

double ATM90E36Driver::GetLineCurrentCT2() {
    unsigned short current = CommEnergyIC(READ, IrmsB, 0xFFFF);
    return (double)current / 1000.0;
}

double ATM90E36Driver::GetLineCurrentCT3() {
    unsigned short current = CommEnergyIC(READ, IrmsC, 0xFFFF);
    return (double)current / 1000.0;
}

double ATM90E36Driver::GetLineCurrentCTN() {
    unsigned short current = CommEnergyIC(READ, IrmsN, 0xFFFF);
    return (double)current / 1000.0;
}

double ATM90E36Driver::GetLineCurrentCTNSamp() {
    unsigned short current = CommEnergyIC(READ, IrmsN1, 0xFFFF);
    return (double)current / 1000.0;
}

// ============================================================
// THD Measurements
// ============================================================

double ATM90E36Driver::GetLineVoltage1THDN() {
    unsigned short voltage = CommEnergyIC(READ, THDNUA, 0xFFFF);
    return (double)voltage / 100.0;
}

double ATM90E36Driver::GetLineVoltage2THDN() {
    unsigned short voltage = CommEnergyIC(READ, THDNUB, 0xFFFF);
    return (double)voltage / 100.0;
}

double ATM90E36Driver::GetLineVoltage3THDN() {
    unsigned short voltage = CommEnergyIC(READ, THDNUC, 0xFFFF);
    return (double)voltage / 100.0;
}

double ATM90E36Driver::GetLineCurrentCT1THDN() {
    unsigned short current = CommEnergyIC(READ, THDNIA, 0xFFFF);
    return (double)current / 1000.0;
}

double ATM90E36Driver::GetLineCurrentCT2THDN() {
    unsigned short current = CommEnergyIC(READ, THDNIB, 0xFFFF);
    return (double)current / 1000.0;
}

double ATM90E36Driver::GetLineCurrentCT3THDN() {
    unsigned short current = CommEnergyIC(READ, THDNIC, 0xFFFF);
    return (double)current / 1000.0;
}

// ============================================================
// Active Power Measurements
// ============================================================

double ATM90E36Driver::GetActivePowerCT1() {
    int val = Read32Register(PmeanA, PmeanALSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetActivePowerCT2() {
    int val = Read32Register(PmeanB, PmeanBLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetActivePowerCT3() {
    int val = Read32Register(PmeanC, PmeanCLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetTotalActivePower() {
    int val = Read32Register(PmeanT, PmeanTLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanActivePowerPhaA() {
    int val = Read32Register(PmeanA, PmeanALSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanActivePowerPhaB() {
    int val = Read32Register(PmeanB, PmeanBLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanActivePowerPhaC() {
    int val = Read32Register(PmeanC, PmeanCLSB);
    return (double)val * 0.00032;
}

// ============================================================
// Fundamental Power Measurements
// ============================================================

double ATM90E36Driver::GetTotalActiveFundPower() {
    int val = Read32Register(PmeanTF, PmeanTFLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanActiveFundPowerPhaA() {
    int val = Read32Register(PmeanAF, PmeanAFLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanActiveFundPowerPhaB() {
    int val = Read32Register(PmeanBF, PmeanBFLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanActiveFundPowerPhaC() {
    int val = Read32Register(PmeanCF, PmeanCFLSB);
    return (double)val * 0.00032;
}

// ============================================================
// Harmonic Power Measurements
// ============================================================

double ATM90E36Driver::GetTotalActiveHarPower() {
    int val = Read32Register(PmeanTH, PmeanTHLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanActiveHarPowerPhaA() {
    int val = Read32Register(PmeanAH, PmeanAHLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanActiveHarPowerPhaB() {
    int val = Read32Register(PmeanBH, PmeanBHLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanActiveHarPowerPhaC() {
    int val = Read32Register(PmeanCH, PmeanCHLSB);
    return (double)val * 0.00032;
}

// ============================================================
// Reactive Power Measurements
// ============================================================

double ATM90E36Driver::GetReactivePowerCT1() {
    int val = Read32Register(QmeanA, QmeanALSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetReactivePowerCT2() {
    int val = Read32Register(QmeanB, QmeanBLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetReactivePowerCT3() {
    int val = Read32Register(QmeanC, QmeanCLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetTotalReactivePower() {
    int val = Read32Register(QmeanT, QmeanTLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanReactivePowerPhaA() {
    int val = Read32Register(QmeanA, QmeanALSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanReactivePowerPhaB() {
    int val = Read32Register(QmeanB, QmeanBLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanReactivePowerPhaC() {
    int val = Read32Register(QmeanC, QmeanCLSB);
    return (double)val * 0.00032;
}

// ============================================================
// Apparent Power Measurements
// ============================================================

double ATM90E36Driver::GetApparentPowerCT1() {
    int val = Read32Register(SmeanA, SmeanALSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetApparentPowerCT2() {
    int val = Read32Register(SmeanB, SmeanBLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetApparentPowerCT3() {
    int val = Read32Register(SmeanC, SmeanCLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetTotalApparentPower() {
    int val = Read32Register(SAmeanT, SAmeanTLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanApparentPowerPhaA() {
    int val = Read32Register(SmeanA, SmeanALSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanApparentPowerPhaB() {
    int val = Read32Register(SmeanB, SmeanBLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetMeanApparentPowerPhaC() {
    int val = Read32Register(SmeanC, SmeanCLSB);
    return (double)val * 0.00032;
}

double ATM90E36Driver::GetTotalVSumApparentPower() {
    int val = Read32Register(SVmeanT, SVmeanTLSB);
    return (double)val * 0.00032;
}

// ============================================================
// Power Factor Measurements
// ============================================================

double ATM90E36Driver::GetPowerFactorCT1() {
    signed short pf = (signed short)CommEnergyIC(READ, PFmeanA, 0xFFFF);
    return (double)pf / 1000.0;
}

double ATM90E36Driver::GetPowerFactorCT2() {
    signed short pf = (signed short)CommEnergyIC(READ, PFmeanB, 0xFFFF);
    return (double)pf / 1000.0;
}

double ATM90E36Driver::GetPowerFactorCT3() {
    signed short pf = (signed short)CommEnergyIC(READ, PFmeanC, 0xFFFF);
    return (double)pf / 1000.0;
}

double ATM90E36Driver::GetTotalPowerFactor() {
    signed short pf = (signed short)CommEnergyIC(READ, PFmeanT, 0xFFFF);
    return (double)pf / 1000.0;
}

// ============================================================
// Phase Angle Measurements
// ============================================================

double ATM90E36Driver::GetPhaseCT1() {
    signed short angleA = (signed short)CommEnergyIC(READ, PAngleA, 0xFFFF);
    return (double)angleA / 10.0;
}

double ATM90E36Driver::GetPhaseCT2() {
    signed short angleB = (signed short)CommEnergyIC(READ, PAngleB, 0xFFFF);
    return (double)angleB / 10.0;
}

double ATM90E36Driver::GetPhaseCT3() {
    signed short angleC = (signed short)CommEnergyIC(READ, PAngleC, 0xFFFF);
    return (double)angleC / 10.0;
}

double ATM90E36Driver::GetPhaseangV1() {
    signed short angleA = (signed short)CommEnergyIC(READ, UangleA, 0xFFFF);
    return (double)angleA / 10.0;
}

double ATM90E36Driver::GetPhaseangV2() {
    signed short angleB = (signed short)CommEnergyIC(READ, UangleB, 0xFFFF);
    return (double)angleB / 10.0;
}

double ATM90E36Driver::GetPhaseangV3() {
    signed short angleC = (signed short)CommEnergyIC(READ, UangleC, 0xFFFF);
    return (double)angleC / 10.0;
}

// ============================================================
// Frequency and Temperature
// ============================================================

double ATM90E36Driver::GetFrequency() {
    unsigned short freq = CommEnergyIC(READ, Freq, 0xFFFF);
    return (double)freq / 100.0;
}

double ATM90E36Driver::GetTemperature() {
    short int atemp = (short int)CommEnergyIC(READ, Temp, 0xFFFF);
    return (double)atemp;
}

// ============================================================
// Energy Measurements - Import Active
// ============================================================

double ATM90E36Driver::GetImportEnergy() {
    unsigned short ienergyT = CommEnergyIC(READ, APenergyT, 0xFFFF);
    return (double)ienergyT / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportEnergyPhaA() {
    unsigned short ienergyA = CommEnergyIC(READ, APenergyA, 0xFFFF);
    return (double)ienergyA / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportEnergyPhaB() {
    unsigned short ienergyB = CommEnergyIC(READ, APenergyB, 0xFFFF);
    return (double)ienergyB / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportEnergyPhaC() {
    unsigned short ienergyC = CommEnergyIC(READ, APenergyC, 0xFFFF);
    return (double)ienergyC / 100.0 / 3200.0;
}

// ============================================================
// Energy Measurements - Export Active
// ============================================================

double ATM90E36Driver::GetExportEnergy() {
    unsigned short eenergyT = CommEnergyIC(READ, ANenergyT, 0xFFFF);
    return (double)eenergyT / 100.0 / 3200.0;
}

double ATM90E36Driver::GetExportEnergyPhaA() {
    unsigned short eenergyA = CommEnergyIC(READ, ANenergyA, 0xFFFF);
    return (double)eenergyA / 100.0 / 3200.0;
}

double ATM90E36Driver::GetExportEnergyPhaB() {
    unsigned short eenergyB = CommEnergyIC(READ, ANenergyB, 0xFFFF);
    return (double)eenergyB / 100.0 / 3200.0;
}

double ATM90E36Driver::GetExportEnergyPhaC() {
    unsigned short eenergyC = CommEnergyIC(READ, ANenergyC, 0xFFFF);
    return (double)eenergyC / 100.0 / 3200.0;
}

// ============================================================
// Energy Measurements - Import Reactive
// ============================================================

double ATM90E36Driver::GetImportReactiveEnergy() {
    unsigned short renergyT = CommEnergyIC(READ, RPenergyT, 0xFFFF);
    return (double)renergyT / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportReactiveEnergyPhaA() {
    unsigned short renergyA = CommEnergyIC(READ, RPenergyA, 0xFFFF);
    return (double)renergyA / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportReactiveEnergyPhaB() {
    unsigned short renergyB = CommEnergyIC(READ, RPenergyB, 0xFFFF);
    return (double)renergyB / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportReactiveEnergyPhaC() {
    unsigned short renergyC = CommEnergyIC(READ, RPenergyC, 0xFFFF);
    return (double)renergyC / 100.0 / 3200.0;
}

// ============================================================
// Energy Measurements - Export Reactive
// ============================================================

double ATM90E36Driver::GetExportReactiveEnergy() {
    unsigned short reenergyT = CommEnergyIC(READ, RNenergyT, 0xFFFF);
    return (double)reenergyT / 100.0 / 3200.0;
}

double ATM90E36Driver::GetExportReactiveEnergyPhaA() {
    unsigned short reenergyA = CommEnergyIC(READ, RNenergyA, 0xFFFF);
    return (double)reenergyA / 100.0 / 3200.0;
}

double ATM90E36Driver::GetExportReactiveEnergyPhaB() {
    unsigned short reenergyB = CommEnergyIC(READ, RNenergyB, 0xFFFF);
    return (double)reenergyB / 100.0 / 3200.0;
}

double ATM90E36Driver::GetExportReactiveEnergyPhaC() {
    unsigned short reenergyC = CommEnergyIC(READ, RNenergyC, 0xFFFF);
    return (double)reenergyC / 100.0 / 3200.0;
}

// ============================================================
// Energy Measurements - Apparent
// ============================================================

double ATM90E36Driver::GetImportApparentEnergy() {
    unsigned short senergyT = CommEnergyIC(READ, SAenergyT, 0xFFFF);
    return (double)senergyT / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportApparentEnergyPhaA() {
    unsigned short senergyA = CommEnergyIC(READ, SenergyA, 0xFFFF);
    return (double)senergyA / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportApparentEnergyPhaB() {
    unsigned short senergyB = CommEnergyIC(READ, SenergyB, 0xFFFF);
    return (double)senergyB / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportApparentEnergyPhaC() {
    unsigned short senergyC = CommEnergyIC(READ, SenergyC, 0xFFFF);
    return (double)senergyC / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportApparentEnergyArt() {
    unsigned short saenergyT = CommEnergyIC(READ, SAenergyT, 0xFFFF);
    return (double)saenergyT / 100.0 / 3200.0;
}

double ATM90E36Driver::GetImportApparentEnergyVec() {
    unsigned short svenergyT = CommEnergyIC(READ, SVenergyT, 0xFFFF);
    return (double)svenergyT / 100.0 / 3200.0;
}

// ============================================================
// Status Registers
// ============================================================

unsigned short ATM90E36Driver::GetSysStatus0() {
    return CommEnergyIC(READ, SysStatus0, 0xFFFF);
}

unsigned short ATM90E36Driver::GetSysStatus1() {
    return CommEnergyIC(READ, SysStatus1, 0xFFFF);
}

unsigned short ATM90E36Driver::GetMeterStatus0() {
    return CommEnergyIC(READ, EnStatus0, 0xFFFF);
}

unsigned short ATM90E36Driver::GetMeterStatus1() {
    return CommEnergyIC(READ, EnStatus1, 0xFFFF);
}

// ============================================================
// Calibration and Utility Functions
// ============================================================

double ATM90E36Driver::CalculateVIOffset(unsigned short regh_addr, unsigned short regl_addr) {
    uint32_t val, val_h, val_l;
    uint16_t offset;
    
    val_h = CommEnergyIC(READ, regh_addr, 0xFFFF);
    val_l = CommEnergyIC(READ, regl_addr, 0xFFFF);
    val = CommEnergyIC(READ, regh_addr, 0xFFFF);
    
    val = val_h << 16;
    val |= val_l;
    val = val >> 7;
    val = (~val) + 1;
    
    offset = val;
    return (double)offset;
}

double ATM90E36Driver::CalculatePowerOffset(unsigned short regh_addr, unsigned short regl_addr) {
    uint32_t val, val_h, val_l;
    uint16_t offset;
    
    val_h = CommEnergyIC(READ, regh_addr, 0xFFFF);
    val_l = CommEnergyIC(READ, regl_addr, 0xFFFF);
    val = CommEnergyIC(READ, regh_addr, 0xFFFF);
    
    val = val_h << 16;
    val |= val_l;
    val = (~val) + 1;
    
    offset = val;
    return (double)offset;
}

double ATM90E36Driver::CalibrateVI(unsigned short reg, unsigned short actualVal) {
    uint16_t gain, val, m, gainReg;
    
    // Sample the reading (average of 4 reads)
    val = CommEnergyIC(READ, reg, 0xFFFF);
    val += CommEnergyIC(READ, reg, 0xFFFF);
    val += CommEnergyIC(READ, reg, 0xFFFF);
    val += CommEnergyIC(READ, reg, 0xFFFF);
    
    // Determine gain register based on measurement register
    switch (reg) {
        case UrmsA: gainReg = UgainA; break;
        case UrmsB: gainReg = UgainB; break;
        case UrmsC: gainReg = UgainC; break;
        case IrmsA: gainReg = IgainA; break;
        case IrmsB: gainReg = IgainB; break;
        case IrmsC: gainReg = IgainC; break;
        default: return 0;
    }
    
    gain = CommEnergyIC(READ, gainReg, 0xFFFF);
    m = actualVal;
    m = ((m * gain) / val);
    gain = m;
    
    CommEnergyIC(WRITE, gainReg, gain);
    
    return (double)gain;
}

unsigned short ATM90E36Driver::GetValueRegister(unsigned short registerRead) {
    return CommEnergyIC(READ, registerRead, 0xFFFF);
}

unsigned short ATM90E36Driver::GetValue(unsigned short registerRead) {
    return CommEnergyIC(READ, registerRead, 0xFFFF);
}

unsigned short ATM90E36Driver::GetRegisterValue(unsigned char address) {
    return CommEnergyIC(READ, address, 0xFFFF);
}
