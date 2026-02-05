/*
   -------------------------------------------------------------------
   EmonESP Serial to Emoncms gateway
   -------------------------------------------------------------------
   Adaptation of Chris Howells OpenEVSE ESP Wifi
   by Trystan Lea, Glyn Hudson, OpenEnergyMonitor

   Modified to use with the CircuitSetup.us Split Phase Energy Meter by jdeglavina

   All adaptation GNU General Public License as below.

   -------------------------------------------------------------------

   This file is part of OpenEnergyMonitor.org project.
   EmonESP is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.
   EmonESP is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with EmonESP; see the file COPYING.  If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef _EMONESP_CONFIG_H
#define _EMONESP_CONFIG_H

#include <Arduino.h>
#include <EEPROM.h>                   // Save config settings
#include <Wire.h>
#include "EEPROM_Defs.h"



// -------------------------------------------------------------------
// Load and save the EmonESP config.
//
// This initial implementation saves the config to the EEPROM area of flash
// -------------------------------------------------------------------

// Global config varables

// Global config varables
extern const String node_type;
extern const String node_description;
extern uint16_t node_id;
extern String node_name;
extern String node_describe;
extern char id[41];

extern String Vendor_Name;
extern String Product_Code;
extern String Revision_Number;



extern String meter_name;
extern String model;
extern String manufacturer;
extern String serial_no;
extern String hardware_rev;
extern String firmware_ver;
extern String mac_add;
extern String node;
extern uint16_t node_ID;




// Wifi Network Strings
extern String esid;
extern String epass;

extern const char* ssid; // Change this to your WiFi SSID
extern const char* password; // Change this to your WiFi password

//extern IPAddress staticIP_1;
//extern IPAddress gateway_1;
//extern IPAddress subnet_1;

extern const char* MQTT_server;
extern int mqtt_port;

// Web server authentication (leave blank for none)
extern String www_username;
extern String www_password;

// EMONCMS SERVER strings
extern String emoncms_server;
extern String emoncms_path;
extern String emoncms_node;
extern String emoncms_apikey;
extern String emoncms_fingerprint;

// MQTT Settings
extern String mqtt_server;
extern String mqtt_topic;
extern String mqtt_user;
extern String mqtt_pass;
extern String mqtt_feed_prefix;

// Calibration Settings for SM energy meter
extern String voltage1_cal;
extern String voltage2_cal;
extern String voltage3_cal;
extern String ct1_cal;
extern String ct2_cal;
extern String ct3_cal;
extern String freq_cal;
extern String gain_cal;


//================================================================================
        // Metering and Measurement CALIBRATION VARIABLE
//================================================================================
extern unsigned short     PHAA_ACTIVEPOWEROFFSET;                //  (u16) Voltage Sag Threshold
extern unsigned short     HarCalRegisters[7];
extern unsigned short     CalRegisters[13];
extern unsigned short     MeasurCalRegisters[15];


extern unsigned short voltageread;


//================================================================================
// Energy Register VARIABLE
//================================================================================
extern float    TotalForwardActiveEnergy;          //  (Float) Total Forward Active Energy
extern float    PhaA_ForwardActiveEnergy;          //  (Float) Phase A Forward Active Energy
extern float    PhaB_ForwardActiveEnergy;          //  (Float) Phase B Forward Active Energy
extern float    PhaC_ForwardActiveEnergy;          //  (Float) Phase C Forward Active Energy
extern float    TotalReverseActiveEnergy;          //  (Float) Total Reverse Active Energy
extern float    PhaA_ReverseActiveEnergy;          //  (Float) Phase A Reverse Active Energy
extern float    PhaB_ReverseActiveEnergy;          //  (Float) Phase B Reverse Active Energy
extern float    PhaC_ReverseActiveEnergy;          //  (Float) Phase C Reverse Active Energy
extern float    TotalForwardReactiveEnergy;          //  (Float) Total Forward Reactive Energy
extern float    PhaA_ForwardReactiveEnergy;          //  (Float) Phase A Forward Reactive Energy
extern float    PhaB_ForwardReactiveEnergy;          //  (Float) Phase B Forward Reactive Energy
extern float    PhaC_ForwardReactiveEnergy;          //  (Float) Phase C Forward Reactive Energy
extern float    TotalReverseReactiveEnergy;          //  (Float) Total Reverse Reactive Energy
extern float    PhaA_ReverseReactiveEnergy;          //  (Float) Phase A Reverse Reactive Energy
extern float    PhaB_ReverseReactiveEnergy;          //  (Float) Phase B Reverse Reactive Energy
extern float    PhaC_ReverseReactiveEnergy;          //  (Float) Phase C Reverse Reactive Energy
extern float    Total_ArithmeticSum_ApparentEnergy;          //  (Float) Total (Arithmetic Sum) Apparent Energy
extern float    PhaA_ApparentEnergy;          //  (Float) Phase A Apparent Energy
extern float    PhaB_ApparentEnergy;          //  (Float) Phase B Apparent Energy
extern float    PhaC_ApparentEnergy;          //  (Float) Phase C Apparent Energy
extern float    VectorSum_TotalApparentEnergy;          //  (Float) (Vector Sum) Total Apparent Energy

extern unsigned short MeteringStatus0;             //  Metering Status 0
extern unsigned short MeteringStatus1;             //  Metering Status 1


extern float    VectorSum_TotalApparentPower;          //  (Float) (Vector Sum) Total Apparent Power

//================================================================================
//Power and Power Factor Register VARIABLES
//================================================================================
extern float     Total_PhasesumActivePower;             //  (Float) Total (all-phase-sum) Active Power
extern float     PhaA_ActivePower;                      //  Phase A Active Power
extern float     PhaB_ActivePower;                      //  Phase B Active Power
extern float     PhaC_ActivePower;                      //  Phase C Active Power
extern float     Total_PhasesumReactivePower;           //  Total (all-phase-sum) Reactive Power
extern float     PhaA_ReactivePower;                    //   Phase A Reactive Power   
extern float     PhaB_ReactivePower;                    //   Phase B Reactive Power
extern float     PhaC_ReactivePower;                    //   Phase C Reactive Power
extern float     Total_ArithmeticSumapparentpower;      //  Total (Arithmetic Sum) apparent power
extern float     PhaA_apparentpower;                    //  Phase A apparent power
extern float     PhaB_apparentpower;                    //  Phase B apparent power
extern float     PhaC_apparentpower;                    //  Phase C apparent power
extern float     Totalpowerfactor;                      //   Total power factor
extern float     PhaA_powerfactor;                      //  Phase A power factor
extern float     PhaB_powerfactor;                      //  Phase B power factor
extern float     PhaC_powerfactor;                      //  Phase C power factor

//==============================================================================
//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
extern float     Totalactivefundamentalpower;             //  (extern float ) Total active fundamental power
extern float     PhaA_activefundamentalpower;             //  (extern float )  Phase A active fundamental power
extern float     PhaB_activefundamentalpower;             //  (extern float ) Phase B active fundamental power
extern float     PhaC_activefundamentalpower;             //  (extern float ) Phase C active fundamental power
extern float     Totalactiveharmonicpower;             //  (extern float ) Total active harmonic power
extern float     PhaA_activeharmonicpower;             //  (extern float ) phase A active harmonic power
extern float     PhaB_activeharmonicpower;             //  (extern float ) phase B active harmonic power
extern float     PhaC_activeharmonicpower;             //  (extern float ) phase C active harmonic power
extern float     N_Line_SampledcurrentRMS;             //  (extern float ) N Line Sampled current RMS
extern float     PhaA_voltageRMS;                       //  (extern float ) phase A voltage RMS
extern float     PhaB_voltageRMS;             //  (extern float ) phase B voltage RMS
extern float     PhaC_voltageRMS;             //  (extern float ) phase C voltage RMS
extern float     N_Line_calculatedcurrentRMS;             //  (extern float ) N Line calculated current RMS
extern float     PhaA_currentRMS;             //  (extern float ) phase A current RMS
extern float     PhaB_currentRMS;             //  (extern float ) phase B current RMS
extern float     PhaC_currentRMS;             //  (extern float ) phase C current RMS
extern float     PhaA_voltageTHDN;             //  (extern float ) phase A voltage THD+N
extern float     PhaB_voltageTHDN;             //  (extern float ) phase B voltage THD+N
extern float     PhaC_voltageTHDN;             //  (extern float ) phase C voltage THD+N
extern float     PhaA_currentTHDN;             //  (extern float ) phase A current THD+N
extern float     PhaB_currentTHDN;             //  (extern float ) phase B current THD+N
extern float     PhaC_currentTHDN;             //  (extern float ) phase C current THD+N
extern float     Frequency;             //  (extern float ) Frequency
extern float     PhaA_meanphaseangle;             //  (extern float )  phase A mean phase angle
extern float     PhaB_meanphaseangle;             //  (extern float ) phase B mean phase angle
extern float     PhaC_meanphaseangle;             //  (extern float )  phase C mean phase angle
extern float     Measuredtemperature;             //  (extern float )  Measured temperature
extern float     PhaA_voltagephaseangle;             //  (extern float )  phase A voltage phase angle
extern float     PhaB_voltagephaseangle;             //  (extern float )  phase B voltage phase angle 
extern float     PhaC_voltagephaseangle;             //  (extern float )  phase C voltage phase angle 





extern float Hum, Temp_C, Temp_F, hif, hic;


//extern boolean Config_State;
//extern int BoudRate, TCP_PublishInterval, Server_Port, MDBUS_BAUD;
//extern String Config_Mode;
//extern String Server_IP, Host_IP, MDBUS_IP;
//extern uint8_t ModSlave_ID;
extern unsigned short Line_Freq, UGainA, UGainB, UGainC, IGainA, IGainB, IGainC, IGainN;

extern unsigned short LineFreq, PGAGain, VoltageGain1, VoltageGain2, VoltageGain3, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN;
extern unsigned int MMode0_Reg, MMode1_Reg, PMPGA_Reg, PStartTh_Reg, QStartTh_Reg, SStartTh_Reg, PPhaseTh_Reg, QPhaseTh_Reg, SPhaseTh_Reg;
//================================================================================
        // EEPROM VARIABLE
//================================================================================

//================================================================================
        // Energy Register VARIABLE
//================================================================================
extern float EEPROM_TotalForwardActiveEnergy;          //  (Float) Total Forward Active Energy
extern float EEPROM_PhaA_ForwardActiveEnergy;          //  (Float) Phase A Forward Active Energy
extern float EEPROM_PhaB_ForwardActiveEnergy;          //  (Float) Phase B Forward Active Energy
extern float EEPROM_PhaC_ForwardActiveEnergy;          //  (Float) Phase C Forward Active Energy
extern float EEPROM_TotalReverseActiveEnergy;          //  (Float) Total Reverse Active Energy
extern float EEPROM_PhaA_ReverseActiveEnergy;          //  (Float) Phase A Reverse Active Energy
extern float EEPROM_PhaB_ReverseActiveEnergy;          //  (Float) Phase B Reverse Active Energy
extern float EEPROM_PhaC_ReverseActiveEnergy;          //  (Float) Phase C Reverse Active Energy
extern float EEPROM_TotalForwardReactiveEnergy;          //  (Float) Total Forward Reactive Energy
extern float EEPROM_PhaA_ForwardReactiveEnergy;          //  (Float) Phase A Forward Reactive Energy
extern float EEPROM_PhaB_ForwardReactiveEnergy;          //  (Float) Phase B Forward Reactive Energy
extern float EEPROM_PhaC_ForwardReactiveEnergy;          //  (Float) Phase C Forward Reactive Energy
extern float EEPROM_TotalReverseReactiveEnergy;          //  (Float) Total Reverse Reactive Energy
extern float EEPROM_PhaA_ReverseReactivEEnergy;          //  (Float) Phase A Reverse Reactive Energy
extern float EEPROM_PhaB_ReverseReactivEEnergy;          //  (Float) Phase B Reverse Reactive Energy
extern float EEPROM_PhaC_ReverseReactivEEnergy;          //  (Float) Phase C Reverse Reactive Energy
extern float EEPROM_Total_ArithmeticSum_ApparentEnergy;          //  (Float) Total (Arithmetic Sum) Apparent Energy

extern float EEPROM_PhaA_ApparentEnergy;          //  (Float) Phase A Apparent Energy
extern float EEPROM_PhaB_ApparentEnergy;          //  (Float) Phase B Apparent Energy
extern float EEPROM_PhaC_ApparentEnergy;          //  (Float) Phase C Apparent Energy

extern float EEPROM_VectorSum_TotalApparentEnergy;          //  (Float) (Vector Sum) Total Apparent Energy

extern unsigned short EEPROM_MeteringStatus0;             //  Metering Status 0
extern unsigned short EEPROM_MeteringStatus1;             //  Metering Status 1


extern float EEPROM_VectorSum_TotalApparentPower;          //  (Float) (Vector Sum) Total Apparent Power


//Power and Power Factor Register VARIABLES
//================================================================================
extern float EEPROM_Total_PhasesumActivePower;             //  (Float) Total (all-phase-sum) Active Power
extern float EEPROM_PhaA_ActivePower;                      //  Phase A Active Power
extern float EEPROM_PhaB_ActivePower;                      //  Phase B Active Power
extern float EEPROM_PhaC_ActivePower;                      //  Phase C Active Power
extern float EEPROM_Total_PhasesumReactivePower;           //  Total (all-phase-sum) Reactive Power
extern float EEPROM_PhaA_ReactivePower;                    //   Phase A Reactive Power   
extern float EEPROM_PhaB_ReactivePower;                    //   Phase B Reactive Power
extern float EEPROM_PhaC_ReactivePower;                    //   Phase C Reactive Power
extern float EEPROM_Total_ArithmeticSumapparentpower;      //  Total (Arithmetic Sum) apparent power
extern float EEPROM_PhaA_apparentpower;                    //  Phase A apparent power
extern float EEPROM_PhaB_apparentpower;                    //  Phase B apparent power
extern float EEPROM_PhaC_apparentpower;                    //  Phase C apparent power
extern float EEPROM_Totalpowerfactor;                      //   Total power factor
extern float EEPROM_PhaA_powerfactor;                      //  Phase A power factor
extern float EEPROM_PhaB_powerfactor;                      //  Phase B power factor
extern float EEPROM_PhaC_powerfactor;                      //  Phase C power factor

//==============================================================================
//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
extern float EEPROM_Totalactivefundamentalpower;             //  (Float) Total active fundamental power
extern float EEPROM_PhaA_activefundamentalpower;             //  (Float)  Phase A active fundamental power
extern float EEPROM_PhaB_activefundamentalpower;             //  (Float) Phase B active fundamental power
extern float EEPROM_PhaC_activefundamentalpower;             //  (Float) Phase C active fundamental power
extern float EEPROM_Totalactiveharmonicpower;             //  (Float) Total active harmonic power
extern float EEPROM_PhaA_activeharmonicpower;             //  (Float) phase A active harmonic power
extern float EEPROM_PhaB_activeharmonicpower;             //  (Float) phase B active harmonic power
extern float EEPROM_PhaC_activeharmonicpower;             //  (Float) phase C active harmonic power
extern float EEPROM_N_Line_SampledcurrentRMS;             //  (Float) N Line Sampled current RMS
extern float EEPROM_PhaA_voltageRMS;                       //  (Float) phase A voltage RMS
extern float EEPROM_PhaB_voltageRMS;             //  (Float) phase B voltage RMS
extern float EEPROM_PhaC_voltageRMS;             //  (Float) phase C voltage RMS
extern float EEPROM_N_Line_calculatedcurrentRMS;             //  (Float) N Line calculated current RMS
extern float EEPROM_PhaA_currentRMS;             //  (Float) phase A current RMS
extern float EEPROM_PhaB_currentRMS;             //  (Float) phase B current RMS
extern float EEPROM_PhaC_currentRMS;             //  (Float) phase C current RMS
extern float EEPROM_PhaA_voltageTHDN;             //  (Float) phase A voltage THD+N
extern float EEPROM_PhaB_voltageTHDN;             //  (Float) phase B voltage THD+N
extern float EEPROM_PhaC_voltageTHDN;             //  (Float) phase C voltage THD+N
extern float EEPROM_PhaA_currentTHDN;             //  (Float) phase A current THD+N
extern float EEPROM_PhaB_currentTHDN;             //  (Float) phase B current THD+N
extern float EEPROM_PhaC_currentTHDN;             //  (Float) phase C current THD+N

extern float EEPROM_Frequency;             //  (Float) Frequency
extern float EEPROM_PhaA_meanphaseangle;             //  (Float)  phase A mean phase angle
extern float EEPROM_PhaB_meanphaseangle;             //  (Float) phase B mean phase angle
extern float EEPROM_PhaC_meanphaseangle;             //  (Float)  phase C mean phase angle
extern float EEPROM_Measuredtemperature;             //  (Float)  Measured temperature
extern float EEPROM_PhaA_voltagephaseangle;             //  (Float)  phase A voltage phase angle
extern float EEPROM_PhaB_voltagephaseangle;             //  (Float)  phase B voltage phase angle 
extern float EEPROM_PhaC_voltagephaseangle;             //  (Float)  phase C voltage phase angle


extern float    EEPROM_Temperature_Val;          //  EEPROM Read Value - Temparature Value °C
extern float    EEPROM_TemperatureF_Val;          //  EEPROM Read Value - Temparature Value ° F
extern float    EEPROM_Humidity_Val;          //  EEPROM Read Value - Humidity Value %
extern float    EEPROM_HeatIndex_Temp_Val;          //  EEPROM Read Value - Temparature Value °C
extern float    EEPROM_HeatIndex_TempF_Val;          //  EEPROM Read Value - Temparature Value °F

//================================================================================

extern char EEPROM_Vendor_Name[30];
extern char EEPROM_Product_Code[15];
extern char EEPROM_Revision_Number[8];

extern char EEPROM_Meter_name[20];
extern char EEPROM_Model[20];
extern char EEPROM_Manufacturer[30];
extern char EEPROM_Serial_no[30];
extern char EEPROM_Hardware_rev[8];
extern char EEPROM_Firmware_ver[20];

extern char EEPROM_MAC_add[30];
extern char EEPROM_node[20];
extern uint16_t EEPROM_node_ID;

extern boolean EEPROM_Config_State;
extern int EEPROM_BoudRate, EEPROM_TCP_PublishInterval, EEPROM_MDBUS_BaudRate;
extern String EEPROM_Config_Mode;
extern String EEPROM_Server_IP, EEPROM_Server_Port, EEPROM_Host_IP, EEPROM_MDBUS_IP;
extern uint8_t EEPROM_ModSlave_ID;
extern unsigned short EEPROM_Line_Freq, EEPROM_PGAGain, EEPROM_UGainA, EEPROM_UGainB, EEPROM_UGainC, EEPROM_IGainA, EEPROM_IGainB, EEPROM_IGainC, EEPROM_IGainN;


// -------------------------------------------------------------------
// Load saved settings
// -------------------------------------------------------------------
extern void config_load_settings();

extern void EEPROM_Updtate_DHT_DATA(float& Temp_C, float& Temp_F, float& Hum, float& hic, float& hif);
extern void EEPROM_Read_DHT_DATA(void);

extern void Save_CalConfiguration(void);
extern void Load_CalConfiguration(void);
extern void EEPROM_READ_Power_Registers(void);

extern void EEPROM_Update_ManufactureInfo(void);
extern void EEPROM_READ_ManufactureInfo(void);

extern void EEPROM_Update_Power_Registers(void);
// -------------------------------------------------------------------
// Save the EmonCMS server details
// -------------------------------------------------------------------
extern void config_save_emoncms(String server, String path, String node, String apikey, String fingerprint);

// -------------------------------------------------------------------
// Save the MQTT broker details
// -------------------------------------------------------------------
extern void config_save_mqtt(String server, String topic, String prefix, String user, String pass);

// -------------------------------------------------------------------
// Save the Calibration details
// -------------------------------------------------------------------
#ifdef SOLAR_METER
extern void config_save_cal(String voltage, String ct1, String ct2, String freq, String gain, String svoltage, String sct1, String sct2);
#else
extern void config_save_cal(String voltage1, String voltage2, String voltage3, String ct1, String ct2, String ct3, String freq, String gain);
#endif

extern void CallibrationSave(void);
extern void LoadCallibration(void);
extern void PrintCallibrationSettings(void);

// -------------------------------------------------------------------
// Save the admin/web interface details
// -------------------------------------------------------------------
extern void config_save_admin(String user, String pass);

// -------------------------------------------------------------------
// Save the Wifi details
// -------------------------------------------------------------------
extern void config_save_wifi(String qsid, String qpass);

// -------------------------------------------------------------------
// Reset the config back to defaults
// -------------------------------------------------------------------
extern void config_reset();

extern String getMacAddress();
extern void EEPROM_Write_String(uint32_t Adds, String val);


extern void EEPROM_Read_String(uint32_t Adds, String DataRead);

#endif // _EMONESP_CONFIG_H
