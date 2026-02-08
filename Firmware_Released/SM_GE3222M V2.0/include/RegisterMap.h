/**
 * @file RegisterMap.h
 * @brief ATM90E36 register address definitions
 * @author Microcode Engineering
 * @date 2026-02-08
 * 
 * Complete register map ported from V1.0 EnergyATM90E36.h
 * Supports ATM90E36 3-Phase Energy Metering IC
 */

#ifndef REGISTERMAP_H
#define REGISTERMAP_H

#include <cstdint>

// ==================== SPI Communication ====================
#define ATM_SPI_WRITE 0  ///< Write operation
#define ATM_SPI_READ  1  ///< Read operation

// ==================== Status Registers ====================
#define REG_SOFT_RESET      0x00  ///< Software Reset
#define REG_SYS_STATUS0     0x01  ///< System Status 0
#define REG_SYS_STATUS1     0x02  ///< System Status 1
#define REG_FUNC_EN0        0x03  ///< Function Enable 0
#define REG_FUNC_EN1        0x04  ///< Function Enable 1
#define REG_ZX_CONFIG       0x07  ///< Zero-Crossing Config
#define REG_SAG_TH          0x08  ///< Voltage Sag Threshold
#define REG_PHASE_LOSS_TH   0x09  ///< Voltage Phase Loss Threshold
#define REG_IN_WARN_TH0     0x0A  ///< Neutral Current Line Threshold
#define REG_IN_WARN_TH1     0x0B  ///< Voltage ADC Threshold
#define REG_THD_NU_TH       0x0C  ///< Voltage THD Threshold
#define REG_THD_NI_TH       0x0D  ///< Current THD Threshold
#define REG_DMA_CTRL        0x0E  ///< DMA Interrupt Control
#define REG_LAST_SPI_DATA   0x0F  ///< Last Read/Write SPI Value

// ==================== Configuration Registers ====================
#define REG_CONFIG_START    0x30  ///< Configuration Start
#define REG_PL_CONST_H      0x31  ///< High Word of PL_Constant
#define REG_PL_CONST_L      0x32  ///< Low Word of PL_Constant
#define REG_M_MODE0         0x33  ///< Metering Mode Config 0
#define REG_M_MODE1         0x34  ///< Metering Mode Config 1
#define REG_P_START_TH      0x35  ///< Active Power Startup Threshold
#define REG_Q_START_TH      0x36  ///< Reactive Power Startup Threshold
#define REG_S_START_TH      0x37  ///< Apparent Power Startup Threshold
#define REG_P_PHASE_TH      0x38  ///< Active Power Accumulation Threshold
#define REG_Q_PHASE_TH      0x39  ///< Reactive Power Accumulation Threshold
#define REG_S_PHASE_TH      0x3A  ///< Apparent Power Accumulation Threshold
#define REG_CS_ZERO         0x3B  ///< Checksum 0

// ==================== Calibration Registers ====================
#define REG_CAL_START       0x40  ///< Calibration Start
#define REG_P_OFFSET_A      0x41  ///< Phase A Active Power Offset
#define REG_Q_OFFSET_A      0x42  ///< Phase A Reactive Power Offset
#define REG_P_OFFSET_B      0x43  ///< Phase B Active Power Offset
#define REG_Q_OFFSET_B      0x44  ///< Phase B Reactive Power Offset
#define REG_P_OFFSET_C      0x45  ///< Phase C Active Power Offset
#define REG_Q_OFFSET_C      0x46  ///< Phase C Reactive Power Offset
#define REG_GAIN_A          0x47  ///< Phase A Calibration Gain
#define REG_PHI_A           0x48  ///< Phase A Calibration Angle
#define REG_GAIN_B          0x49  ///< Phase B Calibration Gain
#define REG_PHI_B           0x4A  ///< Phase B Calibration Angle
#define REG_GAIN_C          0x4B  ///< Phase C Calibration Gain
#define REG_PHI_C           0x4C  ///< Phase C Calibration Angle
#define REG_CS_ONE          0x4D  ///< Checksum 1

// ==================== Harmonic Calibration Registers ====================
#define REG_HARM_START      0x50  ///< Harmonic Calibration Start
#define REG_P_OFFSET_AF     0x51  ///< Phase A Fundamental Active Power Offset
#define REG_P_OFFSET_BF     0x52  ///< Phase B Fundamental Active Power Offset
#define REG_P_OFFSET_CF     0x53  ///< Phase C Fundamental Active Power Offset
#define REG_P_GAIN_AF       0x54  ///< Phase A Fundamental Active Power Gain
#define REG_P_GAIN_BF       0x55  ///< Phase B Fundamental Active Power Gain
#define REG_P_GAIN_CF       0x56  ///< Phase C Fundamental Active Power Gain
#define REG_CS_TWO          0x57  ///< Checksum 2

// ==================== Measurement Calibration Registers ====================
#define REG_ADJ_START       0x60  ///< Measurement Calibration Start
#define REG_U_GAIN_A        0x61  ///< Phase A Voltage RMS Gain
#define REG_I_GAIN_A        0x62  ///< Phase A Current RMS Gain
#define REG_U_OFFSET_A      0x63  ///< Phase A Voltage Offset
#define REG_I_OFFSET_A      0x64  ///< Phase A Current Offset
#define REG_U_GAIN_B        0x65  ///< Phase B Voltage RMS Gain
#define REG_I_GAIN_B        0x66  ///< Phase B Current RMS Gain
#define REG_U_OFFSET_B      0x67  ///< Phase B Voltage Offset
#define REG_I_OFFSET_B      0x68  ///< Phase B Current Offset
#define REG_U_GAIN_C        0x69  ///< Phase C Voltage RMS Gain
#define REG_I_GAIN_C        0x6A  ///< Phase C Current RMS Gain
#define REG_U_OFFSET_C      0x6B  ///< Phase C Voltage Offset
#define REG_I_OFFSET_C      0x6C  ///< Phase C Current Offset
#define REG_I_GAIN_N        0x6D  ///< Neutral Current Gain
#define REG_I_OFFSET_N      0x6E  ///< Neutral Current Offset
#define REG_CS_THREE        0x6F  ///< Checksum 3

// ==================== Energy Registers ====================
#define REG_AP_ENERGY_T     0x80  ///< Total Forward Active Energy
#define REG_AP_ENERGY_A     0x81  ///< Phase A Forward Active Energy
#define REG_AP_ENERGY_B     0x82  ///< Phase B Forward Active Energy
#define REG_AP_ENERGY_C     0x83  ///< Phase C Forward Active Energy
#define REG_AN_ENERGY_T     0x84  ///< Total Reverse Active Energy
#define REG_AN_ENERGY_A     0x85  ///< Phase A Reverse Active Energy
#define REG_AN_ENERGY_B     0x86  ///< Phase B Reverse Active Energy
#define REG_AN_ENERGY_C     0x87  ///< Phase C Reverse Active Energy
#define REG_RP_ENERGY_T     0x88  ///< Total Forward Reactive Energy
#define REG_RP_ENERGY_A     0x89  ///< Phase A Forward Reactive Energy
#define REG_RP_ENERGY_B     0x8A  ///< Phase B Forward Reactive Energy
#define REG_RP_ENERGY_C     0x8B  ///< Phase C Forward Reactive Energy
#define REG_RN_ENERGY_T     0x8C  ///< Total Reverse Reactive Energy
#define REG_RN_ENERGY_A     0x8D  ///< Phase A Reverse Reactive Energy
#define REG_RN_ENERGY_B     0x8E  ///< Phase B Reverse Reactive Energy
#define REG_RN_ENERGY_C     0x8F  ///< Phase C Reverse Reactive Energy
#define REG_SA_ENERGY_T     0x90  ///< Total Apparent Energy
#define REG_S_ENERGY_A      0x91  ///< Phase A Apparent Energy
#define REG_S_ENERGY_B      0x92  ///< Phase B Apparent Energy
#define REG_S_ENERGY_C      0x93  ///< Phase C Apparent Energy
#define REG_SV_ENERGY_T     0x94  ///< Total Apparent Energy (Arithmetic)
#define REG_EN_STATUS0      0x95  ///< Metering Status 0
#define REG_EN_STATUS1      0x96  ///< Metering Status 1

// ==================== Power & Power Factor Registers ====================
#define REG_P_MEAN_T        0xB0  ///< Total Mean Active Power
#define REG_P_MEAN_A        0xB1  ///< Phase A Mean Active Power
#define REG_P_MEAN_B        0xB2  ///< Phase B Mean Active Power
#define REG_P_MEAN_C        0xB3  ///< Phase C Mean Active Power
#define REG_Q_MEAN_T        0xB4  ///< Total Mean Reactive Power
#define REG_Q_MEAN_A        0xB5  ///< Phase A Mean Reactive Power
#define REG_Q_MEAN_B        0xB6  ///< Phase B Mean Reactive Power
#define REG_Q_MEAN_C        0xB7  ///< Phase C Mean Reactive Power
#define REG_S_MEAN_T        0xB8  ///< Total Mean Apparent Power
#define REG_S_MEAN_A        0xB9  ///< Phase A Mean Apparent Power
#define REG_S_MEAN_B        0xBA  ///< Phase B Mean Apparent Power
#define REG_S_MEAN_C        0xBB  ///< Phase C Mean Apparent Power
#define REG_PF_MEAN_T       0xBC  ///< Total Power Factor
#define REG_PF_MEAN_A       0xBD  ///< Phase A Power Factor
#define REG_PF_MEAN_B       0xBE  ///< Phase B Power Factor
#define REG_PF_MEAN_C       0xBF  ///< Phase C Power Factor

// ==================== RMS Voltage & Current Registers ====================
#define REG_U_RMS_A         0xD9  ///< Phase A RMS Voltage
#define REG_U_RMS_B         0xDA  ///< Phase B RMS Voltage
#define REG_U_RMS_C         0xDB  ///< Phase C RMS Voltage
#define REG_I_RMS_N0        0xDC  ///< Calculated Neutral RMS Current
#define REG_I_RMS_A         0xDD  ///< Phase A RMS Current
#define REG_I_RMS_B         0xDE  ///< Phase B RMS Current
#define REG_I_RMS_C         0xDF  ///< Phase C RMS Current
#define REG_I_RMS_N1        0xE0  ///< Measured Neutral RMS Current

// ==================== THD & Other Measurement Registers ====================
#define REG_THD_NU_A        0xF1  ///< Phase A Voltage THD
#define REG_THD_NU_B        0xF2  ///< Phase B Voltage THD
#define REG_THD_NU_C        0xF3  ///< Phase C Voltage THD
#define REG_THD_NI_A        0xF5  ///< Phase A Current THD
#define REG_THD_NI_B        0xF6  ///< Phase B Current THD
#define REG_THD_NI_C        0xF7  ///< Phase C Current THD
#define REG_FREQ            0xF8  ///< Line Frequency
#define REG_P_ANGLE_A       0xF9  ///< Phase A Phase Angle
#define REG_P_ANGLE_B       0xFA  ///< Phase B Phase Angle
#define REG_P_ANGLE_C       0xFB  ///< Phase C Phase Angle
#define REG_TEMP            0xFC  ///< Temperature

// ==================== Register Configuration Values ====================
// ConfigStart/CalStart/HarmStart/AdjStart unlock values
#define REG_START_UNLOCK    0x5678  ///< Unlock value for calibration registers
#define REG_START_LOCK      0x8765  ///< Lock value for calibration registers

// Soft Reset
#define SOFT_RESET_VALUE    0x789A  ///< Software reset command

#endif // REGISTERMAP_H
