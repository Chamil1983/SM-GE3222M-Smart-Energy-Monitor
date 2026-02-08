#pragma once

// SM-GE3222M V2.0 - ATM90E36 Register Map
// Complete register definitions for ATM90E36 Energy IC

#include <cstdint>

// ============================================================================
// CONFIGURATION REGISTERS (Page 0)
// ============================================================================

// Configuration Start & Checksums
constexpr uint16_t REG_CONFIG_START     = 0x30;  // Configuration Start (0x5678)
constexpr uint16_t REG_PL_CONST_H       = 0x31;  // High Word of PL_Constant
constexpr uint16_t REG_PL_CONST_L       = 0x32;  // Low Word of PL_Constant
constexpr uint16_t REG_MMODE0           = 0x33;  // Metering Mode Config
constexpr uint16_t REG_MMODE1           = 0x34;  // PGA Gain Configuration
constexpr uint16_t REG_P_START_TH       = 0x35;  // Startup Power Threshold (P)
constexpr uint16_t REG_Q_START_TH       = 0x36;  // Startup Power Threshold (Q)
constexpr uint16_t REG_S_START_TH       = 0x37;  // Startup Power Threshold (S)
constexpr uint16_t REG_P_PHASE_TH       = 0x38;  // Startup Power Accum Threshold (P)
constexpr uint16_t REG_Q_PHASE_TH       = 0x39;  // Startup Power Accum Threshold (Q)
constexpr uint16_t REG_S_PHASE_TH       = 0x3A;  // Startup Power Accum Threshold (S)
constexpr uint16_t REG_CS_ZERO          = 0x3B;  // Checksum 0

// ============================================================================
// CALIBRATION REGISTERS (Page 1)
// ============================================================================
constexpr uint16_t REG_CAL_START        = 0x40;  // Calibration Start (0x5678)
constexpr uint16_t REG_P_OFFSET_A       = 0x41;  // Phase A Power Offset (P)
constexpr uint16_t REG_Q_OFFSET_A       = 0x42;  // Phase A Power Offset (Q)
constexpr uint16_t REG_P_OFFSET_B       = 0x43;  // Phase B Power Offset (P)
constexpr uint16_t REG_Q_OFFSET_B       = 0x44;  // Phase B Power Offset (Q)
constexpr uint16_t REG_P_OFFSET_C       = 0x45;  // Phase C Power Offset (P)
constexpr uint16_t REG_Q_OFFSET_C       = 0x46;  // Phase C Power Offset (Q)
constexpr uint16_t REG_GAIN_A           = 0x47;  // Phase A Calibration Gain
constexpr uint16_t REG_PHI_A            = 0x48;  // Phase A Calibration Angle
constexpr uint16_t REG_GAIN_B           = 0x49;  // Phase B Calibration Gain
constexpr uint16_t REG_PHI_B            = 0x4A;  // Phase B Calibration Angle
constexpr uint16_t REG_GAIN_C           = 0x4B;  // Phase C Calibration Gain
constexpr uint16_t REG_PHI_C            = 0x4C;  // Phase C Calibration Angle
constexpr uint16_t REG_CS_ONE           = 0x4D;  // Checksum 1

// ============================================================================
// HARMONIC CALIBRATION REGISTERS (Page 2)
// ============================================================================
constexpr uint16_t REG_HARM_START       = 0x50;  // Harmonic Cal Start (0x5678)
constexpr uint16_t REG_P_OFFSET_AF      = 0x51;  // Phase A Fundamental Power Offset (P)
constexpr uint16_t REG_P_OFFSET_BF      = 0x52;  // Phase B Fundamental Power Offset (P)
constexpr uint16_t REG_P_OFFSET_CF      = 0x53;  // Phase C Fundamental Power Offset (P)
constexpr uint16_t REG_P_GAIN_AF        = 0x54;  // Phase A Fundamental Power Gain (P)
constexpr uint16_t REG_P_GAIN_BF        = 0x55;  // Phase B Fundamental Power Gain (P)
constexpr uint16_t REG_P_GAIN_CF        = 0x56;  // Phase C Fundamental Power Gain (P)
constexpr uint16_t REG_CS_TWO           = 0x57;  // Checksum 2

// ============================================================================
// MEASUREMENT CALIBRATION REGISTERS (Page 3)
// ============================================================================
constexpr uint16_t REG_ADJ_START        = 0x60;  // Measurement Cal Start (0x5678)
constexpr uint16_t REG_U_GAIN_A         = 0x61;  // Phase A Voltage RMS Gain
constexpr uint16_t REG_I_GAIN_A         = 0x62;  // Phase A Current RMS Gain
constexpr uint16_t REG_U_OFFSET_A       = 0x63;  // Phase A Voltage Offset
constexpr uint16_t REG_I_OFFSET_A       = 0x64;  // Phase A Current Offset
constexpr uint16_t REG_U_GAIN_B         = 0x65;  // Phase B Voltage RMS Gain
constexpr uint16_t REG_I_GAIN_B         = 0x66;  // Phase B Current RMS Gain
constexpr uint16_t REG_U_OFFSET_B       = 0x67;  // Phase B Voltage Offset
constexpr uint16_t REG_I_OFFSET_B       = 0x68;  // Phase B Current Offset
constexpr uint16_t REG_U_GAIN_C         = 0x69;  // Phase C Voltage RMS Gain
constexpr uint16_t REG_I_GAIN_C         = 0x6A;  // Phase C Current RMS Gain
constexpr uint16_t REG_U_OFFSET_C       = 0x6B;  // Phase C Voltage Offset
constexpr uint16_t REG_I_OFFSET_C       = 0x6C;  // Phase C Current Offset
constexpr uint16_t REG_I_GAIN_N         = 0x6D;  // Neutral Current Gain
constexpr uint16_t REG_I_OFFSET_N       = 0x6E;  // Neutral Current Offset
constexpr uint16_t REG_CS_THREE         = 0x6F;  // Checksum 3

// ============================================================================
// SYSTEM REGISTERS
// ============================================================================
constexpr uint16_t REG_SOFT_RESET       = 0x00;  // Software Reset
constexpr uint16_t REG_SYS_STATUS0      = 0x01;  // System Status 0
constexpr uint16_t REG_SYS_STATUS1      = 0x02;  // System Status 1
constexpr uint16_t REG_FUNC_EN0         = 0x03;  // Function Enable 0
constexpr uint16_t REG_FUNC_EN1         = 0x04;  // Function Enable 1
constexpr uint16_t REG_ZX_CONFIG        = 0x07;  // Zero-Crossing Config
constexpr uint16_t REG_SAG_TH           = 0x08;  // Voltage Sag Threshold
constexpr uint16_t REG_PHASE_LOSS_TH    = 0x09;  // Voltage Phase Loss Threshold
constexpr uint16_t REG_IN_WARN_TH0      = 0x0A;  // Neutral Current Warning Threshold
constexpr uint16_t REG_IN_WARN_TH1      = 0x0B;  // Voltage ADC Threshold
constexpr uint16_t REG_THD_NU_TH        = 0x0C;  // Voltage THD Threshold
constexpr uint16_t REG_THD_NI_TH        = 0x0D;  // Current THD Threshold
constexpr uint16_t REG_DMA_CTRL         = 0x0E;  // DMA Interface Control
constexpr uint16_t REG_LAST_SPI_DATA    = 0x0F;  // Last Read/Write SPI Value

// ============================================================================
// ENERGY REGISTERS (16-bit, pairs for 32-bit values)
// ============================================================================
// Forward Active Energy
constexpr uint16_t REG_AP_ENERGY_T      = 0x80;  // Total Forward Active Energy
constexpr uint16_t REG_AP_ENERGY_A      = 0x81;  // Phase A Forward Active Energy
constexpr uint16_t REG_AP_ENERGY_B      = 0x82;  // Phase B Forward Active Energy
constexpr uint16_t REG_AP_ENERGY_C      = 0x83;  // Phase C Forward Active Energy

// Reverse Active Energy
constexpr uint16_t REG_AN_ENERGY_T      = 0x84;  // Total Reverse Active Energy
constexpr uint16_t REG_AN_ENERGY_A      = 0x85;  // Phase A Reverse Active Energy
constexpr uint16_t REG_AN_ENERGY_B      = 0x86;  // Phase B Reverse Active Energy
constexpr uint16_t REG_AN_ENERGY_C      = 0x87;  // Phase C Reverse Active Energy

// Forward Reactive Energy
constexpr uint16_t REG_RP_ENERGY_T      = 0x88;  // Total Forward Reactive Energy
constexpr uint16_t REG_RP_ENERGY_A      = 0x89;  // Phase A Forward Reactive Energy
constexpr uint16_t REG_RP_ENERGY_B      = 0x8A;  // Phase B Forward Reactive Energy
constexpr uint16_t REG_RP_ENERGY_C      = 0x8B;  // Phase C Forward Reactive Energy

// Reverse Reactive Energy
constexpr uint16_t REG_RN_ENERGY_T      = 0x8C;  // Total Reverse Reactive Energy
constexpr uint16_t REG_RN_ENERGY_A      = 0x8D;  // Phase A Reverse Reactive Energy
constexpr uint16_t REG_RN_ENERGY_B      = 0x8E;  // Phase B Reverse Reactive Energy
constexpr uint16_t REG_RN_ENERGY_C      = 0x8F;  // Phase C Reverse Reactive Energy

// Apparent Energy
constexpr uint16_t REG_SA_ENERGY_T      = 0x90;  // Total Apparent Energy
constexpr uint16_t REG_S_ENERGY_A       = 0x91;  // Phase A Apparent Energy
constexpr uint16_t REG_S_ENERGY_B       = 0x92;  // Phase B Apparent Energy
constexpr uint16_t REG_S_ENERGY_C       = 0x93;  // Phase C Apparent Energy
constexpr uint16_t REG_SV_ENERGY_T      = 0x94;  // Total Apparent Energy (Arithmetic)

// Energy Status
constexpr uint16_t REG_EN_STATUS0       = 0x95;  // Metering Status 0
constexpr uint16_t REG_EN_STATUS1       = 0x96;  // Metering Status 1
constexpr uint16_t REG_SV_MEAN_T        = 0x98;  // Total Apparent Energy (Vector)
constexpr uint16_t REG_SV_MEAN_T_LSB    = 0x99;  // LSB of Vector Sum

// ============================================================================
// FUNDAMENTAL/HARMONIC ENERGY REGISTERS
// ============================================================================
// Forward Fundamental Energy
constexpr uint16_t REG_AP_ENERGY_TF     = 0xA0;  // Total Forward Fundamental Energy
constexpr uint16_t REG_AP_ENERGY_AF     = 0xA1;  // Phase A Forward Fundamental Energy
constexpr uint16_t REG_AP_ENERGY_BF     = 0xA2;  // Phase B Forward Fundamental Energy
constexpr uint16_t REG_AP_ENERGY_CF     = 0xA3;  // Phase C Forward Fundamental Energy

// Reverse Fundamental Energy
constexpr uint16_t REG_AN_ENERGY_TF     = 0xA4;  // Total Reverse Fundamental Energy
constexpr uint16_t REG_AN_ENERGY_AF     = 0xA5;  // Phase A Reverse Fundamental Energy
constexpr uint16_t REG_AN_ENERGY_BF     = 0xA6;  // Phase B Reverse Fundamental Energy
constexpr uint16_t REG_AN_ENERGY_CF     = 0xA7;  // Phase C Reverse Fundamental Energy

// Forward Harmonic Energy
constexpr uint16_t REG_AP_ENERGY_TH     = 0xA8;  // Total Forward Harmonic Energy
constexpr uint16_t REG_AP_ENERGY_AH     = 0xA9;  // Phase A Forward Harmonic Energy
constexpr uint16_t REG_AP_ENERGY_BH     = 0xAA;  // Phase B Forward Harmonic Energy
constexpr uint16_t REG_AP_ENERGY_CH     = 0xAB;  // Phase C Forward Harmonic Energy

// Reverse Harmonic Energy
constexpr uint16_t REG_AN_ENERGY_TH     = 0xAC;  // Total Reverse Harmonic Energy
constexpr uint16_t REG_AN_ENERGY_AH     = 0xAD;  // Phase A Reverse Harmonic Energy
constexpr uint16_t REG_AN_ENERGY_BH     = 0xAE;  // Phase B Reverse Harmonic Energy
constexpr uint16_t REG_AN_ENERGY_CH     = 0xAF;  // Phase C Reverse Harmonic Energy

// ============================================================================
// POWER & POWER FACTOR REGISTERS (16-bit signed)
// ============================================================================
// Active Power
constexpr uint16_t REG_P_MEAN_T         = 0xB0;  // Total Mean Active Power
constexpr uint16_t REG_P_MEAN_A         = 0xB1;  // Phase A Mean Active Power
constexpr uint16_t REG_P_MEAN_B         = 0xB2;  // Phase B Mean Active Power
constexpr uint16_t REG_P_MEAN_C         = 0xB3;  // Phase C Mean Active Power

// Reactive Power
constexpr uint16_t REG_Q_MEAN_T         = 0xB4;  // Total Mean Reactive Power
constexpr uint16_t REG_Q_MEAN_A         = 0xB5;  // Phase A Mean Reactive Power
constexpr uint16_t REG_Q_MEAN_B         = 0xB6;  // Phase B Mean Reactive Power
constexpr uint16_t REG_Q_MEAN_C         = 0xB7;  // Phase C Mean Reactive Power

// Apparent Power
constexpr uint16_t REG_S_MEAN_T         = 0xB8;  // Total Mean Apparent Power
constexpr uint16_t REG_S_MEAN_A         = 0xB9;  // Phase A Mean Apparent Power
constexpr uint16_t REG_S_MEAN_B         = 0xBA;  // Phase B Mean Apparent Power
constexpr uint16_t REG_S_MEAN_C         = 0xBB;  // Phase C Mean Apparent Power

// Power Factor
constexpr uint16_t REG_PF_MEAN_T        = 0xBC;  // Total Mean Power Factor
constexpr uint16_t REG_PF_MEAN_A        = 0xBD;  // Phase A Power Factor
constexpr uint16_t REG_PF_MEAN_B        = 0xBE;  // Phase B Power Factor
constexpr uint16_t REG_PF_MEAN_C        = 0xBF;  // Phase C Power Factor

// ============================================================================
// POWER LSB REGISTERS (for 32-bit power values)
// ============================================================================
constexpr uint16_t REG_P_MEAN_T_LSB     = 0xC0;  // Total Active Power LSB
constexpr uint16_t REG_P_MEAN_A_LSB     = 0xC1;  // Phase A Active Power LSB
constexpr uint16_t REG_P_MEAN_B_LSB     = 0xC2;  // Phase B Active Power LSB
constexpr uint16_t REG_P_MEAN_C_LSB     = 0xC3;  // Phase C Active Power LSB
constexpr uint16_t REG_Q_MEAN_T_LSB     = 0xC4;  // Total Reactive Power LSB
constexpr uint16_t REG_Q_MEAN_A_LSB     = 0xC5;  // Phase A Reactive Power LSB
constexpr uint16_t REG_Q_MEAN_B_LSB     = 0xC6;  // Phase B Reactive Power LSB
constexpr uint16_t REG_Q_MEAN_C_LSB     = 0xC7;  // Phase C Reactive Power LSB

// ============================================================================
// FUNDAMENTAL POWER & POWER FACTOR REGISTERS
// ============================================================================
constexpr uint16_t REG_P_MEAN_TF        = 0xC8;  // Total Mean Fundamental Active Power
constexpr uint16_t REG_P_MEAN_AF        = 0xC9;  // Phase A Fundamental Active Power
constexpr uint16_t REG_P_MEAN_BF        = 0xCA;  // Phase B Fundamental Active Power
constexpr uint16_t REG_P_MEAN_CF        = 0xCB;  // Phase C Fundamental Active Power
constexpr uint16_t REG_PF_MEAN_TF       = 0xCC;  // Total Fundamental Power Factor
constexpr uint16_t REG_PF_MEAN_AF       = 0xCD;  // Phase A Fundamental Power Factor
constexpr uint16_t REG_PF_MEAN_BF       = 0xCE;  // Phase B Fundamental Power Factor
constexpr uint16_t REG_PF_MEAN_CF       = 0xCF;  // Phase C Fundamental Power Factor

// ============================================================================
// VOLTAGE & CURRENT RMS REGISTERS
// ============================================================================
constexpr uint16_t REG_U_RMS_A          = 0xD0;  // Phase A Voltage RMS
constexpr uint16_t REG_U_RMS_B          = 0xD1;  // Phase B Voltage RMS
constexpr uint16_t REG_U_RMS_C          = 0xD2;  // Phase C Voltage RMS
constexpr uint16_t REG_I_RMS_N          = 0xD3;  // Neutral Current RMS
constexpr uint16_t REG_I_RMS_A          = 0xD4;  // Phase A Current RMS
constexpr uint16_t REG_I_RMS_B          = 0xD5;  // Phase B Current RMS
constexpr uint16_t REG_I_RMS_C          = 0xD6;  // Phase C Current RMS

// ============================================================================
// THD & PHASE ANGLE REGISTERS
// ============================================================================
constexpr uint16_t REG_THD_NU_A         = 0xD8;  // Phase A Voltage THD
constexpr uint16_t REG_THD_NU_B         = 0xD9;  // Phase B Voltage THD
constexpr uint16_t REG_THD_NU_C         = 0xDA;  // Phase C Voltage THD
constexpr uint16_t REG_THD_NI_A         = 0xDC;  // Phase A Current THD
constexpr uint16_t REG_THD_NI_B         = 0xDD;  // Phase B Current THD
constexpr uint16_t REG_THD_NI_C         = 0xDE;  // Phase C Current THD

constexpr uint16_t REG_P_ANGLE_A        = 0xE0;  // Phase A Mean Phase Angle
constexpr uint16_t REG_P_ANGLE_B        = 0xE1;  // Phase B Mean Phase Angle
constexpr uint16_t REG_P_ANGLE_C        = 0xE2;  // Phase C Mean Phase Angle
constexpr uint16_t REG_U_ANGLE_A        = 0xE4;  // Phase A Voltage Phase Angle
constexpr uint16_t REG_U_ANGLE_B        = 0xE5;  // Phase B Voltage Phase Angle
constexpr uint16_t REG_U_ANGLE_C        = 0xE6;  // Phase C Voltage Phase Angle

// ============================================================================
// FREQUENCY & TEMPERATURE REGISTERS
// ============================================================================
constexpr uint16_t REG_FREQUENCY        = 0xE8;  // Line Frequency
constexpr uint16_t REG_TEMPERATURE      = 0xE9;  // Internal Die Temperature

// ============================================================================
// CALIBRATION CONSTANTS
// ============================================================================
constexpr uint16_t CAL_CONFIG_START     = 0x5678;  // Configuration start magic
constexpr uint16_t CAL_CS_ZERO          = 0x4741;  // CS0 expected value

// Default PLconstant for 50Hz, 1000 pulses/kWh
constexpr uint16_t DEFAULT_PL_CONST_H   = 0x0861;
constexpr uint16_t DEFAULT_PL_CONST_L   = 0xC468;

// SPI Protocol Constants
constexpr uint16_t SPI_READ_FLAG        = 0x8000;  // Bit 15 = 1 for READ
constexpr uint16_t SPI_WRITE_FLAG       = 0x0000;  // Bit 15 = 0 for WRITE

// Scaling Factors
constexpr float VOLTAGE_SCALE           = 100.0;   // Voltage registers / 100
constexpr float CURRENT_SCALE           = 1000.0;  // Current registers / 1000
constexpr float POWER_SCALE             = 1000.0;  // Power registers / 1000
constexpr float PF_SCALE                = 1000.0;  // Power factor / 1000
constexpr float FREQUENCY_SCALE         = 100.0;   // Frequency / 100
constexpr float THD_SCALE               = 100.0;   // THD / 100
constexpr float ANGLE_SCALE             = 10.0;    // Angle / 10
