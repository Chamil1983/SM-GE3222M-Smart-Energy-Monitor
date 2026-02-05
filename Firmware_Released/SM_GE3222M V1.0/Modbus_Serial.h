// Modbus_Serial.h

#ifndef _MODBUS_SERIAL_h
#define _MODBUS_SERIAL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <darray.h>
#include <Modbus.h>
#include <ModbusAPI.h>
#include <Ethernet.h> 
#include <ModbusEthernet.h>

#include <ModbusIP_ESP8266.h>
#include <ModbusRTU.h>
#include <ModbusSettings.h>
#include <ModbusTCP.h>
//#include <ModbusTCPTemplate.h>
#include <ModbusTLS.h>



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

extern boolean EnableMODBUS_ETH;


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




extern uint16_t GetHoldreg(TRegister* reg, uint16_t val);
extern uint16_t SetHoldreg(TRegister* reg, uint16_t val);


extern float GetIreg(TRegister* reg, float val);
extern float SetIreg(TRegister* reg, float val);

extern void Modbus_Poling(void);
extern void Load_Reg_Callibration(void);
extern void Load_Power_Registers(void);
extern void Setup_Modbus(void);
extern void InitModbus(void);
extern void Configure_MODBUS_Registers(void);
extern String getMacAddress();
extern void floatconv(float input, uint16_t* ha, uint16_t* hb);
extern void led_ModbusFlash(int ton, int toff);
extern void Flash_LED(uint8_t SelectOutput, int ton, int toff, uint16_t numFlash);
//extern char Stringconv(char* input, char strconv[30]);
//extern char StrtoCharconv(String str);
#endif

