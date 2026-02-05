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

#include "emonesp.h"
#include "config.h"
#include <SparkFun_External_EEPROM.h>


ExternalEEPROM ExtMem;

#define EEPROM_ADDRESS 0x50    //Address of 24LC256 eeprom chip


bool eeprom_autoupdate = true; //specify if you wanna control updates
const String terminating_char = "|"; //ensure its something you wont use in any String!!!!!!




// Wifi Network Strings
String esid = "";
String epass = "";

//const char* ssid = "Google NODE"; // Change this to your WiFi SSID
//const char* password = "36002016"; // Change this to your WiFi password

//const char* ssid = "SM-FE102M(WiFi)"; // Change this to your WiFi SSID
//const char* password = ""; // Change this to your WiFi password
 

//IPAddress staticIP_1(192, 168, 86, 10);
//IPAddress gateway_1(192, 168, 86, 1);
//IPAddress subnet_1(255, 255, 255, 0);

const char* MQTT_server = "192.168.102.154";
int mqtt_port = 1883;


// Web server authentication (leave blank for none)
String www_username = "admin";
String www_password = "admin";

// EMONCMS SERVER strings
String emoncms_server = "";
String emoncms_path = "";
String emoncms_node = "";
String emoncms_apikey = "";
String emoncms_fingerprint = "";

// MQTT Settings
String mqtt_server = "";
String mqtt_topic = "";
String mqtt_user = "";
String mqtt_pass = "";
String mqtt_feed_prefix = "";

// Calibration Settings
String voltage1_cal = "";
String voltage2_cal = "";
String voltage3_cal = "";
String ct1_cal = "";
String ct2_cal = "";
String ct3_cal = "";
String freq_cal = "";
String gain_cal = "";

#define BAUD_2400       
#define BAUD_4800       
#define BAUD_9600       
#define BAUD_19200      
#define BAUD_38400      
#define BAUD_57600      
#define BAUD_115200     
#define BAUD_230400     
#define BAUD_250000     
#define BAUD_500000     
#define BAUD_1000000   


//================================================================================
        // Metering and Measurement CALIBRATION VARIABLE
//================================================================================

unsigned short     CalRegisters[13];
unsigned short     HarCalRegisters[7];
unsigned short     MeasurCalRegisters[15];



unsigned short voltageread;



//================================================================================


//================================================================================
        // Energy Register VARIABLE
//================================================================================
float    TotalForwardActiveEnergy;          //  (Float) Total Forward Active Energy
float    PhaA_ForwardActiveEnergy;          //  (Float) Phase A Forward Active Energy
float    PhaB_ForwardActiveEnergy;          //  (Float) Phase B Forward Active Energy
float    PhaC_ForwardActiveEnergy;          //  (Float) Phase C Forward Active Energy
float    TotalReverseActiveEnergy;          //  (Float) Total Reverse Active Energy
float    PhaA_ReverseActiveEnergy;          //  (Float) Phase A Reverse Active Energy
float    PhaB_ReverseActiveEnergy;          //  (Float) Phase B Reverse Active Energy
float    PhaC_ReverseActiveEnergy;          //  (Float) Phase C Reverse Active Energy
float    TotalForwardReactiveEnergy;          //  (Float) Total Forward Reactive Energy
float    PhaA_ForwardReactiveEnergy;          //  (Float) Phase A Forward Reactive Energy
float    PhaB_ForwardReactiveEnergy;          //  (Float) Phase B Forward Reactive Energy
float    PhaC_ForwardReactiveEnergy;          //  (Float) Phase C Forward Reactive Energy
float    TotalReverseReactiveEnergy;          //  (Float) Total Reverse Reactive Energy
float    PhaA_ReverseReactiveEnergy;          //  (Float) Phase A Reverse Reactive Energy
float    PhaB_ReverseReactiveEnergy;          //  (Float) Phase B Reverse Reactive Energy
float    PhaC_ReverseReactiveEnergy;          //  (Float) Phase C Reverse Reactive Energy
float    Total_ArithmeticSum_ApparentEnergy;          //  (Float) Total (Arithmetic Sum) Apparent Energy

float    PhaA_ApparentEnergy;          //  (Float) Phase A Apparent Energy
float    PhaB_ApparentEnergy;          //  (Float) Phase B Apparent Energy
float    PhaC_ApparentEnergy;          //  (Float) Phase C Apparent Energy

float    VectorSum_TotalApparentEnergy;          //  (Float) (Vector Sum) Total Apparent Energy

unsigned short MeteringStatus0;             //  Metering Status 0
unsigned short MeteringStatus1;             //  Metering Status 1


float    VectorSum_TotalApparentPower;          //  (Float) (Vector Sum) Total Apparent Power


//Power and Power Factor Register VARIABLES
//================================================================================
float    Total_PhasesumActivePower;             //  (Float) Total (all-phase-sum) Active Power
float    PhaA_ActivePower;                      //  Phase A Active Power
float    PhaB_ActivePower;                      //  Phase B Active Power
float    PhaC_ActivePower;                      //  Phase C Active Power
float    Total_PhasesumReactivePower;           //  Total (all-phase-sum) Reactive Power
float    PhaA_ReactivePower;                    //   Phase A Reactive Power   
float    PhaB_ReactivePower;                    //   Phase B Reactive Power
float    PhaC_ReactivePower;                    //   Phase C Reactive Power
float    Total_ArithmeticSumapparentpower;      //  Total (Arithmetic Sum) apparent power
float    PhaA_apparentpower;                    //  Phase A apparent power
float    PhaB_apparentpower;                    //  Phase B apparent power
float    PhaC_apparentpower;                    //  Phase C apparent power
float    Totalpowerfactor;                      //   Total power factor
float    PhaA_powerfactor;                      //  Phase A power factor
float    PhaB_powerfactor;                      //  Phase B power factor
float    PhaC_powerfactor;                      //  Phase C power factor

//==============================================================================
//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
float    Totalactivefundamentalpower;             //  (Float) Total active fundamental power
float    PhaA_activefundamentalpower;             //  (Float)  Phase A active fundamental power
float    PhaB_activefundamentalpower;             //  (Float) Phase B active fundamental power
float    PhaC_activefundamentalpower;             //  (Float) Phase C active fundamental power
float    Totalactiveharmonicpower;             //  (Float) Total active harmonic power
float    PhaA_activeharmonicpower;             //  (Float) phase A active harmonic power
float    PhaB_activeharmonicpower;             //  (Float) phase B active harmonic power
float    PhaC_activeharmonicpower;             //  (Float) phase C active harmonic power
float    N_Line_SampledcurrentRMS;             //  (Float) N Line Sampled current RMS
float    PhaA_voltageRMS;                       //  (Float) phase A voltage RMS
float    PhaB_voltageRMS;             //  (Float) phase B voltage RMS
float    PhaC_voltageRMS;             //  (Float) phase C voltage RMS
float    N_Line_calculatedcurrentRMS;             //  (Float) N Line calculated current RMS
float    PhaA_currentRMS;             //  (Float) phase A current RMS
float    PhaB_currentRMS;             //  (Float) phase B current RMS
float    PhaC_currentRMS;             //  (Float) phase C current RMS
float    PhaA_voltageTHDN;             //  (Float) phase A voltage THD+N
float    PhaB_voltageTHDN;             //  (Float) phase B voltage THD+N
float    PhaC_voltageTHDN;             //  (Float) phase C voltage THD+N
float    PhaA_currentTHDN;             //  (Float) phase A current THD+N
float    PhaB_currentTHDN;             //  (Float) phase B current THD+N
float    PhaC_currentTHDN;             //  (Float) phase C current THD+N

float    Frequency;             //  (Float) Frequency
float    PhaA_meanphaseangle;             //  (Float)  phase A mean phase angle
float    PhaB_meanphaseangle;             //  (Float) phase B mean phase angle
float    PhaC_meanphaseangle;             //  (Float)  phase C mean phase angle
float    Measuredtemperature;             //  (Float)  Measured temperature
float    PhaA_voltagephaseangle;             //  (Float)  phase A voltage phase angle
float    PhaB_voltagephaseangle;             //  (Float)  phase B voltage phase angle 
float    PhaC_voltagephaseangle;             //  (Float)  phase C voltage phase angle 



//================================================================================
//  DESIGN CONFIGURATION VARIABLES
//SystemConfigurationFlag SystemConfiguration;    //  (b32)
//EventConfigurationFlag  EventConfiguration;     //  (b32)

unsigned short      MeteringModeConfiguration;          //  (u16) Metering Mode Configuration



//================================================================================



//================================================================================
        // EEPROM VARIABLE
//================================================================================

//================================================================================
        // Energy Register VARIABLE
//================================================================================
float    EEPROM_TotalForwardActiveEnergy;          //  (Float) Total Forward Active Energy
float    EEPROM_PhaA_ForwardActiveEnergy;          //  (Float) Phase A Forward Active Energy
float    EEPROM_PhaB_ForwardActiveEnergy;          //  (Float) Phase B Forward Active Energy
float    EEPROM_PhaC_ForwardActiveEnergy;          //  (Float) Phase C Forward Active Energy
float    EEPROM_TotalReverseActiveEnergy;          //  (Float) Total Reverse Active Energy
float    EEPROM_PhaA_ReverseActiveEnergy;          //  (Float) Phase A Reverse Active Energy
float    EEPROM_PhaB_ReverseActiveEnergy;          //  (Float) Phase B Reverse Active Energy
float    EEPROM_PhaC_ReverseActiveEnergy;          //  (Float) Phase C Reverse Active Energy
float    EEPROM_TotalForwardReactiveEnergy;          //  (Float) Total Forward Reactive Energy
float    EEPROM_PhaA_ForwardReactiveEnergy;          //  (Float) Phase A Forward Reactive Energy
float    EEPROM_PhaB_ForwardReactiveEnergy;          //  (Float) Phase B Forward Reactive Energy
float    EEPROM_PhaC_ForwardReactiveEnergy;          //  (Float) Phase C Forward Reactive Energy
float    EEPROM_TotalReverseReactiveEnergy;          //  (Float) Total Reverse Reactive Energy
float    EEPROM_PhaA_ReverseReactivEEnergy;          //  (Float) Phase A Reverse Reactive Energy
float    EEPROM_PhaB_ReverseReactivEEnergy;          //  (Float) Phase B Reverse Reactive Energy
float    EEPROM_PhaC_ReverseReactivEEnergy;          //  (Float) Phase C Reverse Reactive Energy
float    EEPROM_Total_ArithmeticSum_ApparentEnergy;          //  (Float) Total (Arithmetic Sum) Apparent Energy

float    EEPROM_PhaA_ApparentEnergy;          //  (Float) Phase A Apparent Energy
float    EEPROM_PhaB_ApparentEnergy;          //  (Float) Phase B Apparent Energy
float    EEPROM_PhaC_ApparentEnergy;          //  (Float) Phase C Apparent Energy

float    EEPROM_VectorSum_TotalApparentEnergy;          //  (Float) (Vector Sum) Total Apparent Energy

unsigned short EEPROM_MeteringStatus0;             //  Metering Status 0
unsigned short EEPROM_MeteringStatus1;             //  Metering Status 1


float    EEPROM_VectorSum_TotalApparentPower;          //  (Float) (Vector Sum) Total Apparent Power



unsigned short Line_Freq, UGainA, UGainB, UGainC, IGainA, IGainB, IGainC, IGainN;

//Power and Power Factor Register VARIABLES Readed from EEPROM
//================================================================================
float    EEPROM_Total_PhasesumActivePower;             //  (Float) Total (all-phase-sum) Active Power
float    EEPROM_PhaA_ActivePower;                      //  Phase A Active Power
float    EEPROM_PhaB_ActivePower;                      //  Phase B Active Power
float    EEPROM_PhaC_ActivePower;                      //  Phase C Active Power
float    EEPROM_Total_PhasesumReactivePower;           //  Total (all-phase-sum) Reactive Power
float    EEPROM_PhaA_ReactivePower;                    //   Phase A Reactive Power   
float    EEPROM_PhaB_ReactivePower;                    //   Phase B Reactive Power
float    EEPROM_PhaC_ReactivePower;                    //   Phase C Reactive Power
float    EEPROM_Total_ArithmeticSumapparentpower;      //  Total (Arithmetic Sum) apparent power
float    EEPROM_PhaA_apparentpower;                    //  Phase A apparent power
float    EEPROM_PhaB_apparentpower;                    //  Phase B apparent power
float    EEPROM_PhaC_apparentpower;                    //  Phase C apparent power
float    EEPROM_Totalpowerfactor;                      //   Total power factor
float    EEPROM_PhaA_powerfactor;                      //  Phase A power factor
float    EEPROM_PhaB_powerfactor;                      //  Phase B power factor
float    EEPROM_PhaC_powerfactor;                      //  Phase C power factor

//==============================================================================
//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
float    EEPROM_Totalactivefundamentalpower;             //  (Float) Total active fundamental power
float    EEPROM_PhaA_activefundamentalpower;             //  (Float)  Phase A active fundamental power
float    EEPROM_PhaB_activefundamentalpower;             //  (Float) Phase B active fundamental power
float    EEPROM_PhaC_activefundamentalpower;             //  (Float) Phase C active fundamental power
float    EEPROM_Totalactiveharmonicpower;             //  (Float) Total active harmonic power
float    EEPROM_PhaA_activeharmonicpower;             //  (Float) phase A active harmonic power
float    EEPROM_PhaB_activeharmonicpower;             //  (Float) phase B active harmonic power
float    EEPROM_PhaC_activeharmonicpower;             //  (Float) phase C active harmonic power
float    EEPROM_N_Line_SampledcurrentRMS;             //  (Float) N Line Sampled current RMS
float    EEPROM_PhaA_voltageRMS;                       //  (Float) phase A voltage RMS
float    EEPROM_PhaB_voltageRMS;             //  (Float) phase B voltage RMS
float    EEPROM_PhaC_voltageRMS;             //  (Float) phase C voltage RMS
float    EEPROM_N_Line_calculatedcurrentRMS;             //  (Float) N Line calculated current RMS
float    EEPROM_PhaA_currentRMS;             //  (Float) phase A current RMS
float    EEPROM_PhaB_currentRMS;             //  (Float) phase B current RMS
float    EEPROM_PhaC_currentRMS;             //  (Float) phase C current RMS
float    EEPROM_PhaA_voltageTHDN;             //  (Float) phase A voltage THD+N
float    EEPROM_PhaB_voltageTHDN;             //  (Float) phase B voltage THD+N
float    EEPROM_PhaC_voltageTHDN;             //  (Float) phase C voltage THD+N
float    EEPROM_PhaA_currentTHDN;             //  (Float) phase A current THD+N
float    EEPROM_PhaB_currentTHDN;             //  (Float) phase B current THD+N
float    EEPROM_PhaC_currentTHDN;             //  (Float) phase C current THD+N

float    EEPROM_Frequency;             //  (Float) Frequency
float    EEPROM_PhaA_meanphaseangle;             //  (Float)  phase A mean phase angle
float    EEPROM_PhaB_meanphaseangle;             //  (Float) phase B mean phase angle
float    EEPROM_PhaC_meanphaseangle;             //  (Float)  phase C mean phase angle
float    EEPROM_Measuredtemperature;             //  (Float)  Measured temperature
float    EEPROM_PhaA_voltagephaseangle;             //  (Float)  phase A voltage phase angle
float    EEPROM_PhaB_voltagephaseangle;             //  (Float)  phase B voltage phase angle 
float    EEPROM_PhaC_voltagephaseangle;             //  (Float)  phase C voltage phase angle 



float    EEPROM_Temperature_Val;          //  EEPROM Read Value - Temparature Value °C
float    EEPROM_TemperatureF_Val;          //  EEPROM Read Value - Temparature Value ° F
float    EEPROM_Humidity_Val;          //  EEPROM Read Value - Humidity Value %
float    EEPROM_HeatIndex_Temp_Val;          //  EEPROM Read Value - Temparature Value °C
float    EEPROM_HeatIndex_TempF_Val;          //  EEPROM Read Value - Temparature Value °F

//================================================================================

char EEPROM_Vendor_Name[30];
char EEPROM_Product_Code[15];
char EEPROM_Revision_Number[8];

char EEPROM_Meter_name[20];
char EEPROM_Model[20];
char EEPROM_Manufacturer[30];
char EEPROM_Serial_no[30];
char EEPROM_Hardware_rev[8];
char EEPROM_Firmware_ver[20];

char EEPROM_MAC_add[30];
char EEPROM_node[20];
uint16_t EEPROM_node_ID;



//boolean EEPROM_Config_State;
//int EEPROM_BoudRate,  EEPROM_Server_Port, EEPROM_TCP_PublishInterval, EEPROM_MDBUS_BAUD;
//String EEPROM_Config_Mode;
//String EEPROM_Server_IP, EEPROM_Host_IP, EEPROM_MDBUS_IP;
//uint8_t EEPROM_ModSlave_ID;
unsigned short EEPROM_Line_Freq, EEPROM_PGAGain, EEPROM_UGainA, EEPROM_UGainB, EEPROM_UGainC, EEPROM_IGainA, EEPROM_IGainB, EEPROM_IGainC, EEPROM_IGainN;


//=========================================================================================================================
/*
 ___________________________________________________________________________________________
|                 |                                                                         |
|                 |                                                                         |
|   EEPROM ADD    | DESCRIPTION                                                             |
|-------------------------------------------------------------------------------------------|
|                                         PAGE 0                                            |
|-------------------------------------------------------------------------------------------|
|     0x0000      | (u16) -> WiFi SSID                                                      |
|     0x0002      | (u16) -> WiFi Password                                                  |
|     0x0004      | (u16) -> Gain Active Power                                              |
|     0x0006      | (u16) -> Gain Reactive Power                                            |
|     0x0008      | (u16) -> Offset Current RMS                                             |
|     0x000C      | (u16) -> Offset Active Power                                            |
|-------------------------------------------------------------------------------------------|
|                                         PAGE 1                                            |
|-------------------------------------------------------------------------------------------|
|     0x0010      | (s32) -> Offset Reactive Power                                          |
|     0x0014      | (s16) -> DC Offset Current                                              |
|     0x0016      | (s16) -> Phase Compensation                                             |
|     0x0018      | (u16) -> Apparent Power Divisor                                         |
|     0x001A      | (b32) -> System Configuration                                           |
|     0x001E      | (u16) -> Free (Not Used)                                                |
|-------------------------------------------------------------------------------------------|
|                                         PAGE 2                                            |
|-------------------------------------------------------------------------------------------|
|     0x0020      | (b32) -> Event Configuration                                            |
|     0x0024      | (b32) -> Range                                                          |
|     0x0028      | (u32) -> Calibration Current                                            |
|     0x002C      | (u16) -> Calibration Voltage                                            |
|     0x002E      | (u16) -> Free (Not Used)                                                |
|-------------------------------------------------------------------------------------------|
|                                         PAGE 3                                            |
|-------------------------------------------------------------------------------------------|
|     0x0030      | (u32) -> Calibration Active Power                                       |
|     0x0034      | (u32) -> Calibration Reactive Power                                     |
|     0x0038      | (u16) -> Line Frequency Reference                                       |
|     0x003A      | (u16) -> Accumulator Interval Parameter                                 |
|     0x003C      | (u16) -> Voltage Sag Limit                                              |
|     0x003E      | (u16) -> Voltage Surge Limit                                            |
|-------------------------------------------------------------------------------------------|
|                                         PAGE 4                                            |
|-------------------------------------------------------------------------------------------|
|     0x0040      | (u32) -> OverCurrent Limit                                              |
|     0x0044      | (u32) -> OverPower Limit                                                |
|     0x0048      | (u16) -> Temperature Compensation for Frequency                         |
|     0x004A      | (u16) -> Temperature Compensation for Current                           |
|     0x004C      | (u16) -> Temperature Compensation for Power                             |
|     0x004E      | (u16) -> Ambient Temperature Reference Voltage                          |
|-------------------------------------------------------------------------------------------|
|                                         PAGE 5                                            |
|-------------------------------------------------------------------------------------------|
|     0x0050      | (u16) -> PWM Period                                                     |
|     0x0052      | (u16) -> PWM Duty Cycle                                                 |
|     0x0054      | (u16) -> MinMax Pointer 1                                               |
|     0x0056      | (u16) -> MinMax Pointer 2                                               |
|     0x0058      | (u16) -> OverTemperature Limit                                          |
|     0x005A      | (u16) -> Energy Control                                                 |
|     0x005C      | (u16) -> PWM Control                                                    |
|     0x005E      | (u16) -> No Load Threshold                                              |
|-------------------------------------------------------------------------------------------|
|                                   PAGE 6 - Page 31                                        |
|-------------------------------------------------------------------------------------------|
| 0x0060 - 0x01FF | Reserved (Free Space)                                                   |
|_________________|_________________________________________________________________________|
*/
//=========================================================================================================================







// -------------------------------------------------------------------
// Load saved settings from EEPROM
// -------------------------------------------------------------------
void config_load_settings()
{
  //EEPROM.begin(EEPROM_SIZE);

    Wire.begin();

  // Load WiFi values

    ExtMem.get(EEPROM_ESID_START, esid); //location to read, thing to put data into
    ExtMem.get(EEPROM_EPASS_START, epass); //location to read, thing to put data into

  if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
  {
      DBUGS.println("No memory detected. Freezing.");
      while (true);
  }

  Load_CalConfiguration();
  
      // EmonCMS settings
  ExtMem.get(EEPROM_EMON_API_KEY_START, emoncms_apikey); //location to read, thing to put data into
  ExtMem.get(EEPROM_EMON_SERVER_START, emoncms_server);
  ExtMem.get(EEPROM_EMON_PATH_START, emoncms_path);
  ExtMem.get(EEPROM_EMON_NODE_START,emoncms_node);
  ExtMem.get(EEPROM_EMON_FINGERPRINT_START,emoncms_fingerprint);

  // MQTT settings
  ExtMem.get(EEPROM_MQTT_SERVER_START, mqtt_server);
  ExtMem.get(EEPROM_MQTT_TOPIC_START, mqtt_topic);
  ExtMem.get(EEPROM_MQTT_FEED_PREFIX_START, mqtt_feed_prefix);
  ExtMem.get(EEPROM_MQTT_USER_START, mqtt_user);
  ExtMem.get(EEPROM_MQTT_PASS_START, mqtt_pass);


  //N_LineReactivePowerOffset = voltage_cal
  ExtMem.get(EEPROM_CAL_VOLTAGE1_START, voltage1_cal);
  ExtMem.get(EEPROM_CAL_VOLTAGE2_START, voltage2_cal);
  ExtMem.get(EEPROM_CAL_VOLTAGE3_START, voltage3_cal);

  // Calibration settings
  //ExtMem.get(EEPROM_CAL_VOLTAGE_START, voltage_cal);
  ExtMem.get(EEPROM_CAL_CT1_START, ct1_cal);
  ExtMem.get(EEPROM_CAL_CT2_START, ct2_cal);
  ExtMem.get(EEPROM_CAL_CT3_START, ct3_cal);
  ExtMem.get(EEPROM_CAL_FREQ_START,  freq_cal);
  ExtMem.get(EEPROM_CAL_GAIN_START, gain_cal);

#ifdef SOLAR_METER
  ExtMem.get((EEPROM_CAL_SVOLTAGE_START,  svoltage_cal);
  ExtMem.get((EEPROM_CAL_SCT1_START,  sct1_cal);
  ExtMem.get((EEPROM_CAL_SCT2_START,  sct2_cal);
#endif

  // Web server credentials
  ExtMem.get(EEPROM_WWW_USER_START,  www_username);
  ExtMem.get(EEPROM_WWW_PASS_START,  www_password);
  

  
    
}


void config_save_cal(String voltage1, String voltage2, String voltage3, String ct1, String ct2, String ct3, String freq, String gain)
{
    Wire.begin();

  voltage1_cal = voltage1;
  voltage2_cal = voltage2;
  voltage3_cal = voltage3;
  ct1_cal = ct1;
  ct2_cal = ct2;
  ct3_cal = ct3;
  freq_cal = freq;
  gain_cal = gain;

  EEPROM_Write_String(EEPROM_CAL_VOLTAGE1_START,  voltage1_cal);
  EEPROM_Write_String(EEPROM_CAL_VOLTAGE2_START, voltage2_cal);
  EEPROM_Write_String(EEPROM_CAL_VOLTAGE3_START, voltage3_cal);
  EEPROM_Write_String(EEPROM_CAL_CT1_START,  ct1_cal);
  EEPROM_Write_String(EEPROM_CAL_CT2_START,  ct2_cal);
  EEPROM_Write_String(EEPROM_CAL_CT3_START, ct3_cal);
  EEPROM_Write_String(EEPROM_CAL_FREQ_START,  freq_cal);
  EEPROM_Write_String(EEPROM_CAL_GAIN_START,  gain_cal);




}



void CallibrationSave(void) {

    Wire.begin();

    if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
    {
        DBUGS.println("No memory detected. Freezing.");
        while (true);
    }

    ExtMem.put(EEPROM_FREQUENCYCAL_START, MMode0_Reg);
    ExtMem.put(EEPROM_PGAGAIN_START, MMode1_Reg);
    ExtMem.put(EEPROM_VOLTAGEGAIN1_START, MeasurCalRegisters[0]);
    ExtMem.put(EEPROM_VOLTAGEGAIN2_START, MeasurCalRegisters[4]);
    ExtMem.put(EEPROM_VOLTAGEGAIN3_START, MeasurCalRegisters[8]);
    ExtMem.put(EEPROM_CURRENTGAIN1_START, MeasurCalRegisters[1]);
    ExtMem.put(EEPROM_CURRENTGAIN2_START, MeasurCalRegisters[5]);
    ExtMem.put(EEPROM_CURRENTGAIN3_START, MeasurCalRegisters[9]);
    ExtMem.put(EEPROM_CURRENTGAIN_N_START, MeasurCalRegisters[12]);

   // ExtMem.put(EEPROM_BOUDRATE_START, BoudRate);


}

void LoadCallibration(void) {


    ExtMem.get(EEPROM_FREQUENCYCAL_START, EEPROM_Line_Freq);
    ExtMem.get(EEPROM_PGAGAIN_START, EEPROM_PGAGain);
    ExtMem.get(EEPROM_VOLTAGEGAIN1_START, EEPROM_UGainA);
    ExtMem.get(EEPROM_VOLTAGEGAIN2_START, EEPROM_UGainB);
    ExtMem.get(EEPROM_VOLTAGEGAIN3_START, EEPROM_UGainC);
    ExtMem.get(EEPROM_CURRENTGAIN1_START, EEPROM_IGainA);
    ExtMem.get(EEPROM_CURRENTGAIN2_START, EEPROM_IGainB);
    ExtMem.get(EEPROM_CURRENTGAIN3_START, EEPROM_IGainC);
    ExtMem.get(EEPROM_CURRENTGAIN_N_START, EEPROM_IGainN);
    //ExtMem.get(EEPROM_BOUDRATE_START, EEPROM_BoudRate);

    LineFreq = EEPROM_Line_Freq;
    PGAGain = EEPROM_PGAGain;
    VoltageGain1 = EEPROM_UGainA;
    VoltageGain2 = EEPROM_UGainB;
    VoltageGain3 = EEPROM_UGainC;
    CurrentGainCT1 = EEPROM_IGainA;
    CurrentGainCT2 = EEPROM_IGainB;
    CurrentGainCT3 = EEPROM_IGainC;
    CurrentGainCTN = EEPROM_IGainN;
   // BoudRate = EEPROM_BoudRate;


}


void PrintCallibrationSettings(void) {

    DBUGS.println();
    DBUGS.println();

    DBUGS.printf("Frequency Gain : %d\n", EEPROM_Line_Freq);
    DBUGS.printf("PGA Gain : %d\n", EEPROM_PGAGain);
    DBUGS.printf("Voltage Gain1 : %d\n", EEPROM_UGainA);
    DBUGS.printf("Voltage Gain2 : %d\n", EEPROM_UGainB);
    DBUGS.printf("Voltage Gain3 : %d\n", EEPROM_UGainC);
    DBUGS.printf("Current Gain CT1 : %d\n", EEPROM_IGainA);
    DBUGS.printf("Current Gain CT2 : %d\n", EEPROM_IGainB);
    DBUGS.printf("Current Gain CT3 : %d\n", EEPROM_IGainC);
    DBUGS.printf("Current Gain CTN : %d\n", EEPROM_IGainN);
   // DBUGS.printf("Serial Baudrate : %s\n", EEPROM_BoudRate);
}

void Save_CalConfiguration(void) {

    Wire.begin();


    if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
    {
        DBUGS.println("No memory detected. Freezing.");
        while (true);
    }

    uint8_t regadd = 0;

    do {

        ExtMem.put((EEPROM_PHAA_ACTIVEPOWEROFFSET_START + (regadd*16)), CalRegisters[regadd + 65]);
        
    } while (++regadd < sizeof(CalRegisters));

    regadd = 0;

    do {
        ExtMem.put((EEPROM_PHAA_FUNDACTIVEPOWEROFFSET_START + (regadd * 16)), HarCalRegisters[regadd + 81]);
        
    } while (++regadd < sizeof(HarCalRegisters));

    regadd = 0;

    do {
        ExtMem.put((EEPROM_PHAA_VOLTAGERMSGAIN_START + (regadd * 16)), HarCalRegisters[regadd + 97]);
        
    } while (++regadd < sizeof(MeasurCalRegisters));


}


void Load_CalConfiguration(void) {

    
    uint8_t regadd = 0;

    do {

        ExtMem.get((EEPROM_PHAA_ACTIVEPOWEROFFSET_START + (regadd * 16)), CalRegisters[regadd + 65]);

    } while (++regadd < sizeof(CalRegisters));

    regadd = 0;

    do {
        ExtMem.get((EEPROM_PHAA_FUNDACTIVEPOWEROFFSET_START + (regadd * 16)), HarCalRegisters[regadd + 81]);

    } while (++regadd < sizeof(HarCalRegisters));

    regadd = 0;

    do {
        ExtMem.get((EEPROM_PHAA_VOLTAGERMSGAIN_START + (regadd * 16)), HarCalRegisters[regadd + 97]);

    } while (++regadd < sizeof(MeasurCalRegisters));



}



void EEPROM_Update_ManufactureInfo(void) {

    Wire.begin();


    if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
    {
        DBUGS.println("No memory detected. Freezing.");
        while (true);
    }

    ExtMem.put(EEPROM_VENDOR_NAME_START, Vendor_Name);
    ExtMem.put(EEPROM_PRODUCT_CODEE_START, Product_Code);
    ExtMem.put(EEPROM_REVISION_NUMBER_START, Revision_Number);
    ExtMem.put(EEPROM_METER_NAME_START, meter_name);
    ExtMem.put(EEPROM_MODEL_START, model);
    ExtMem.put(EEPROM_MANUFACTURER_START, manufacturer);
    ExtMem.put(EEPROM_SERIAL_NO_START, serial_no);
    ExtMem.put(EEPROM_HARDWARE_REV_START, hardware_rev);
    ExtMem.put(EEPROM_FIRMWARE_VER_START, firmware_ver);
    ExtMem.put(EEPROM_MAC_ADD_START, mac_add);
    ExtMem.put(EEPROM_NODE_START, node);
    ExtMem.put(EEPROM_NODE_ID_START, node_ID);


}


void EEPROM_READ_ManufactureInfo(void) {

    ExtMem.get(EEPROM_VENDOR_NAME_START, EEPROM_Vendor_Name);
    ExtMem.get(EEPROM_PRODUCT_CODEE_START, EEPROM_Product_Code);
    ExtMem.get(EEPROM_REVISION_NUMBER_START, EEPROM_Revision_Number);
    ExtMem.get(EEPROM_METER_NAME_START, EEPROM_Meter_name);
    ExtMem.get(EEPROM_MODEL_START, EEPROM_Model);
    ExtMem.get(EEPROM_MANUFACTURER_START, EEPROM_Manufacturer);
    ExtMem.get(EEPROM_SERIAL_NO_START, EEPROM_Serial_no);
    ExtMem.get(EEPROM_HARDWARE_REV_START, EEPROM_Hardware_rev);
    ExtMem.get(EEPROM_FIRMWARE_VER_START, EEPROM_Firmware_ver);
    ExtMem.get(EEPROM_MAC_ADD_START, EEPROM_MAC_add);
    ExtMem.get(EEPROM_NODE_START, EEPROM_node);
    ExtMem.get(EEPROM_NODE_ID_START, EEPROM_node_ID);



}


void EEPROM_Update_Power_Registers(void) {

    ExtMem.put(EEPROM_TOTALFWDACTIVEENERGY_START, TotalForwardActiveEnergy);
    ExtMem.put(EEPROM_PHAA_FWDACTIVEENERGY_START, PhaA_ForwardActiveEnergy);
    ExtMem.put(EEPROM_PHAB_FWDACTIVEENERGY_START, PhaB_ForwardActiveEnergy);
    ExtMem.put(EEPROM_PHAC_FWDACTIVEENERGY_START, PhaC_ForwardActiveEnergy);
    ExtMem.put(EEPROM_TOTALREVACTIVEENERGY_START, TotalReverseActiveEnergy);
    ExtMem.put(EEPROM_PHAA_REVACTIVEENERGY_START, PhaA_ReverseActiveEnergy);
    ExtMem.put(EEPROM_PHAB_REVACTIVEENERGY_START, PhaB_ReverseActiveEnergy);
    ExtMem.put(EEPROM_PHAC_REVACTIVEENERGY_START, PhaC_ReverseActiveEnergy);
    ExtMem.put(EEPROM_TOTALFWDREACTIVEENERGY_START, TotalForwardReactiveEnergy);
    ExtMem.put(EEPROM_PHAA_FWDREACTIVEENERGY_START, PhaA_ForwardReactiveEnergy);
    ExtMem.put(EEPROM_PHAB_FWDREACTIVEENERGY_START, PhaB_ForwardReactiveEnergy);
    ExtMem.put(EEPROM_PHAC_FWDREACTIVEENERGY_START, PhaC_ForwardReactiveEnergy);
    ExtMem.put(EEPROM_TOTALREVREACTIVEENERGY_START, TotalReverseReactiveEnergy);
    ExtMem.put(EEPROM_PHAA_REVREACTIVEENERGY_START, PhaA_ReverseReactiveEnergy);
    ExtMem.put(EEPROM_PHAB_REVREACTIVEENERGY_START, PhaB_ReverseReactiveEnergy);
    ExtMem.put(EEPROM_PHAC_REVREACTIVEENERGY_START, PhaC_ReverseReactiveEnergy);
    ExtMem.put(EEPROM_TOTAL_ARITHMETICSUMAPPARENTENERGY_START, Total_ArithmeticSum_ApparentEnergy);
    ExtMem.put(EEPROM_PHAA_APPARENTENERGY_START, PhaA_ApparentEnergy);
    ExtMem.put(EEPROM_PHAB_APPARENTENERGY_START, PhaB_ApparentEnergy);
    ExtMem.put(EEPROM_PHAC_APPARENTENERGY_START, PhaC_ApparentEnergy);
    ExtMem.put(EEPROM_TOTAL_VECTORSUMAPPARENTENERGY_START, VectorSum_TotalApparentEnergy);
    ExtMem.put(EEPROM_TOTAL_ALLPHASESUMACTIVEPOWER_START, Total_PhasesumActivePower);
    ExtMem.put(EEPROM_EEPROM_PHAA_ACTIVEPOWER_START, PhaA_ActivePower);
    ExtMem.put(EEPROM_EEPROM_PHAB_ACTIVEPOWER_START, PhaB_ActivePower);
    ExtMem.put(EEPROM_EEPROM_PHAC_ACTIVEPOWER_START, PhaC_ActivePower);
    ExtMem.put(EEPROM_TOTAL_ALLPHASESUMREACTIVEPOWER_START, Total_PhasesumReactivePower);
    ExtMem.put(EEPROM_PHAA_REACTIVEPOWER_START, PhaA_ReactivePower);
    ExtMem.put(EEPROM_PHAB_REACTIVEPOWER_START, PhaB_ReactivePower);
    ExtMem.put(EEPROM_PHAC_REACTIVEPOWER_START, PhaC_ReactivePower);
    ExtMem.put(EEPROM_TOTAL_ARITHMETICSUMAPPARENTPOWER_START, Total_ArithmeticSumapparentpower);
    ExtMem.put(EEPROM_PHAA_APPARENTPOWER_START, PhaA_apparentpower);
    ExtMem.put(EEPROM_PHAB_APPARENTPOWER_START, PhaB_apparentpower);
    ExtMem.put(EEPROM_PHAC_APPARENTPOWER_START, PhaC_apparentpower);
    ExtMem.put(EEPROM_TOTALPOWERFACTOR_START, Totalpowerfactor);
    ExtMem.put(EEPROM_PHAA_POWERFACTOR_START, PhaA_powerfactor);
    ExtMem.put(EEPROM_PHAB_POWERFACTOR_START, PhaB_powerfactor);
    ExtMem.put(EEPROM_PHAC_POWERFACTOR_START, PhaC_powerfactor);
    ExtMem.put(EEPROM_PMEANTF_START, Totalactivefundamentalpower);
    ExtMem.put(EEPROM_PMEANAF_START, PhaA_activefundamentalpower);
    ExtMem.put(EEPROM_PMEANBF_START, PhaB_activefundamentalpower);
    ExtMem.put(EEPROM_PMEANCF_START, PhaC_activefundamentalpower);
    ExtMem.put(EEPROM_PMEANTH_START, Totalactiveharmonicpower);
    ExtMem.put(EEPROM_PMEANAH_START, PhaA_activeharmonicpower);
    ExtMem.put(EEPROM_PMEANBH_START, PhaB_activeharmonicpower);
    ExtMem.put(EEPROM_PMEANCH_START, PhaC_activeharmonicpower);
    ExtMem.put(EEPROM_IRMSN1_START, N_Line_SampledcurrentRMS);
    ExtMem.put(EEPROM_URMSA_START, PhaA_voltageRMS);
    ExtMem.put(EEPROM_URMSB_START, PhaB_voltageRMS);
    ExtMem.put(EEPROM_URMSC_START, PhaC_voltageRMS);
    ExtMem.put(EEPROM_IRMSN0_START, N_Line_calculatedcurrentRMS);
    ExtMem.put(EEPROM_IRMSA_START, PhaA_currentRMS);
    ExtMem.put(EEPROM_IRMSB_START, PhaB_currentRMS);
    ExtMem.put(EEPROM_IRMSC_START, PhaC_currentRMS);
    ExtMem.put(EEPROM_THDNUA_START, PhaA_voltageTHDN);
    ExtMem.put(EEPROM_THDNUB_START, PhaB_voltageTHDN);
    ExtMem.put(EEPROM_THDNUC_START, PhaC_voltageTHDN);
    ExtMem.put(EEPROM_THDNIA_START, PhaA_currentTHDN);
    ExtMem.put(EEPROM_THDNIB_START, PhaB_currentTHDN);
    ExtMem.put(EEPROM_THDNIC_START, PhaC_currentTHDN);
    ExtMem.put(EEPROM_FREQ_START, Frequency);
    ExtMem.put(EEPROM_PANGLEA_START, PhaA_meanphaseangle);
    ExtMem.put(EEPROM_PANGLEB_START, PhaB_meanphaseangle);
    ExtMem.put(EEPROM_PANGLEC_START, PhaC_meanphaseangle);
    ExtMem.put(EEPROM_UANGLEA_START, PhaA_voltagephaseangle);
    ExtMem.put(EEPROM_UANGLEB_START, PhaB_voltagephaseangle);
    ExtMem.put(EEPROM_UANGLEC_START, PhaC_voltagephaseangle);


}


void EEPROM_Updtate_DHT_DATA(float &Temp_C, float &Temp_F, float &Hum, float &hic, float &hif) {

    ExtMem.put(EEPROM_TEMPERATURE_START, Temp_C);
    ExtMem.put(EEPROM_TEMPERATURE_F_START, Temp_F);
    ExtMem.put(EEPROM_HUMIDITY_START, Hum);
    ExtMem.put(EEPROM_HEATINDEX_TEMP_START, hic);
    ExtMem.put(EEPROM_HEATINDEX_TEMPF_START, hif);


}







void Display_BIN16(int var) {  // Display BIN from Var
    for (unsigned int i = 0x8000; i; i >>= 1) {
        DBUGS.write(var & i ? '1' : '0');
    }
    DBUGS.print(" ");
}

void Display_HEX(unsigned long var, unsigned char numChars) {  // Display Hex from Var
    unsigned long mask = 0x0000000F;
    mask = mask << 4 * (numChars - 1);

    for (unsigned int i = numChars; i > 0; --i) {
        DBUGS.print(((var & mask) >> (i - 1) * 4), HEX);
        mask = mask >> 4;
    }
    DBUGS.print(" ");
}


void EEPROM_READ_Power_Registers(void) {
    
   
    ExtMem.get(EEPROM_TOTALFWDACTIVEENERGY_START, EEPROM_TotalForwardActiveEnergy);
    ExtMem.get(EEPROM_PHAA_FWDACTIVEENERGY_START, EEPROM_PhaA_ForwardActiveEnergy);
    ExtMem.get(EEPROM_PHAB_FWDACTIVEENERGY_START, EEPROM_PhaB_ForwardActiveEnergy);
    ExtMem.get(EEPROM_PHAC_FWDACTIVEENERGY_START, EEPROM_PhaC_ForwardActiveEnergy);
    ExtMem.get(EEPROM_TOTALREVACTIVEENERGY_START, EEPROM_TotalReverseActiveEnergy);
    ExtMem.get(EEPROM_PHAA_REVACTIVEENERGY_START, EEPROM_PhaA_ReverseActiveEnergy);
    ExtMem.get(EEPROM_PHAB_REVACTIVEENERGY_START, EEPROM_PhaB_ReverseActiveEnergy);
    ExtMem.get(EEPROM_PHAC_REVACTIVEENERGY_START, EEPROM_PhaC_ReverseActiveEnergy);
    ExtMem.get(EEPROM_TOTALFWDREACTIVEENERGY_START, EEPROM_TotalForwardReactiveEnergy);
    ExtMem.get(EEPROM_PHAA_FWDREACTIVEENERGY_START, EEPROM_PhaA_ForwardReactiveEnergy);
    ExtMem.get(EEPROM_PHAB_FWDREACTIVEENERGY_START, EEPROM_PhaB_ForwardReactiveEnergy);
    ExtMem.get(EEPROM_PHAC_FWDREACTIVEENERGY_START, EEPROM_PhaC_ForwardReactiveEnergy);
    ExtMem.get(EEPROM_TOTALREVREACTIVEENERGY_START, EEPROM_TotalReverseReactiveEnergy);
    ExtMem.get(EEPROM_PHAA_REVREACTIVEENERGY_START, EEPROM_PhaA_ReverseReactivEEnergy);
    ExtMem.get(EEPROM_PHAB_REVREACTIVEENERGY_START, EEPROM_PhaB_ReverseReactivEEnergy);
    ExtMem.get(EEPROM_PHAC_REVREACTIVEENERGY_START, EEPROM_PhaC_ReverseReactivEEnergy);
    ExtMem.get(EEPROM_TOTAL_ARITHMETICSUMAPPARENTENERGY_START, EEPROM_Total_ArithmeticSum_ApparentEnergy);
    ExtMem.get(EEPROM_PHAA_APPARENTENERGY_START, EEPROM_PhaA_ApparentEnergy);
    ExtMem.get(EEPROM_PHAB_APPARENTENERGY_START, EEPROM_PhaB_ApparentEnergy);
    ExtMem.get(EEPROM_PHAC_APPARENTENERGY_START, EEPROM_PhaC_ApparentEnergy);
    ExtMem.get(EEPROM_TOTAL_VECTORSUMAPPARENTENERGY_START, EEPROM_VectorSum_TotalApparentEnergy);
    ExtMem.get(EEPROM_TOTAL_ALLPHASESUMACTIVEPOWER_START, EEPROM_Total_PhasesumActivePower);
    ExtMem.get(EEPROM_EEPROM_PHAA_ACTIVEPOWER_START, EEPROM_PhaA_ActivePower);
    ExtMem.get(EEPROM_EEPROM_PHAB_ACTIVEPOWER_START, EEPROM_PhaB_ActivePower);
    ExtMem.get(EEPROM_EEPROM_PHAC_ACTIVEPOWER_START, EEPROM_PhaC_ActivePower);
    ExtMem.get(EEPROM_TOTAL_ALLPHASESUMREACTIVEPOWER_START, EEPROM_Total_PhasesumReactivePower);
    ExtMem.get(EEPROM_PHAA_REACTIVEPOWER_START, EEPROM_PhaA_ReactivePower);
    ExtMem.get(EEPROM_PHAB_REACTIVEPOWER_START, EEPROM_PhaB_ReactivePower);
    ExtMem.get(EEPROM_PHAC_REACTIVEPOWER_START, EEPROM_PhaC_ReactivePower);
    ExtMem.get(EEPROM_TOTAL_ARITHMETICSUMAPPARENTPOWER_START, EEPROM_Total_ArithmeticSumapparentpower);
    ExtMem.get(EEPROM_PHAA_APPARENTPOWER_START, EEPROM_PhaA_apparentpower);
    ExtMem.get(EEPROM_PHAB_APPARENTPOWER_START, EEPROM_PhaB_apparentpower);
    ExtMem.get(EEPROM_PHAC_APPARENTPOWER_START, EEPROM_PhaC_apparentpower);
    ExtMem.get(EEPROM_TOTALPOWERFACTOR_START, EEPROM_Totalpowerfactor);
    ExtMem.get(EEPROM_PHAA_POWERFACTOR_START, EEPROM_PhaA_powerfactor);
    ExtMem.get(EEPROM_PHAB_POWERFACTOR_START, EEPROM_PhaB_powerfactor);
    ExtMem.get(EEPROM_PHAC_POWERFACTOR_START, EEPROM_PhaC_powerfactor);
    ExtMem.get(EEPROM_PMEANTF_START, EEPROM_Totalactivefundamentalpower);
    ExtMem.get(EEPROM_PMEANAF_START, EEPROM_PhaA_activefundamentalpower);
    ExtMem.get(EEPROM_PMEANBF_START, EEPROM_PhaB_activefundamentalpower);
    ExtMem.get(EEPROM_PMEANCF_START, EEPROM_PhaC_activefundamentalpower);
    ExtMem.get(EEPROM_PMEANTH_START, EEPROM_Totalactiveharmonicpower);
    ExtMem.get(EEPROM_PMEANAH_START, EEPROM_PhaA_activeharmonicpower);
    ExtMem.get(EEPROM_PMEANBH_START, EEPROM_PhaB_activeharmonicpower);
    ExtMem.get(EEPROM_PMEANCH_START, EEPROM_PhaC_activeharmonicpower);
    ExtMem.get(EEPROM_IRMSN1_START, EEPROM_N_Line_SampledcurrentRMS);
    ExtMem.get(EEPROM_URMSA_START, EEPROM_PhaA_voltageRMS);
    ExtMem.get(EEPROM_URMSB_START, EEPROM_PhaB_voltageRMS);
    ExtMem.get(EEPROM_URMSC_START, EEPROM_PhaC_voltageRMS);
    ExtMem.get(EEPROM_IRMSN0_START, EEPROM_N_Line_calculatedcurrentRMS);
    ExtMem.get(EEPROM_IRMSA_START, EEPROM_PhaA_currentRMS);
    ExtMem.get(EEPROM_IRMSB_START, EEPROM_PhaB_currentRMS);
    ExtMem.get(EEPROM_IRMSC_START, EEPROM_PhaC_currentRMS);
    ExtMem.get(EEPROM_THDNUA_START, EEPROM_PhaA_voltageTHDN);
    ExtMem.get(EEPROM_THDNUB_START, EEPROM_PhaB_voltageTHDN);
    ExtMem.get(EEPROM_THDNUC_START, EEPROM_PhaC_voltageTHDN);
    ExtMem.get(EEPROM_THDNIA_START, EEPROM_PhaA_currentTHDN);
    ExtMem.get(EEPROM_THDNIB_START, EEPROM_PhaB_currentTHDN);
    ExtMem.get(EEPROM_THDNIC_START, EEPROM_PhaC_currentTHDN);
    ExtMem.get(EEPROM_FREQ_START, EEPROM_Frequency);
    ExtMem.get(EEPROM_PANGLEA_START, EEPROM_PhaA_meanphaseangle);
    ExtMem.get(EEPROM_PANGLEB_START, EEPROM_PhaB_meanphaseangle);
    ExtMem.get(EEPROM_PANGLEC_START, EEPROM_PhaC_meanphaseangle);
    ExtMem.get(EEPROM_UANGLEA_START, EEPROM_PhaA_voltagephaseangle);
    ExtMem.get(EEPROM_UANGLEB_START, EEPROM_PhaB_voltagephaseangle);
    ExtMem.get(EEPROM_UANGLEC_START, EEPROM_PhaC_voltagephaseangle);

}


void EEPROM_Read_DHT_DATA(void) {

    ExtMem.get(EEPROM_TEMPERATURE_START, EEPROM_Temperature_Val);
    ExtMem.get(EEPROM_TEMPERATURE_F_START, EEPROM_TemperatureF_Val);
    ExtMem.get(EEPROM_HUMIDITY_START, EEPROM_Humidity_Val);
    ExtMem.get(EEPROM_HEATINDEX_TEMP_START, EEPROM_HeatIndex_Temp_Val);
    ExtMem.get(EEPROM_HEATINDEX_TEMPF_START, EEPROM_HeatIndex_TempF_Val);

    yield();
    DBUGS.println("Temperature : " + String(EEPROM_Temperature_Val, 2) + "°C");
    yield();
    DBUGS.println("Temperatur (F) : " + String(EEPROM_TemperatureF_Val, 2) + "°F");
    yield();
    DBUGS.println("Humidity : " + String(EEPROM_Humidity_Val, 2) + "%");
    yield();
    DBUGS.println("Heat Index (C) : " + String(EEPROM_HeatIndex_Temp_Val, 2) + "°C");
    yield();
    DBUGS.println("Heat Index (F) : " + String(EEPROM_HeatIndex_TempF_Val, 2) + "°F");




}



void config_save_emoncms(String server, String path, String node, String apikey, String fingerprint)
{
    Wire.begin();

    emoncms_server = server;
    emoncms_path = path;
    emoncms_node = node;
    emoncms_apikey = apikey;
    emoncms_fingerprint = fingerprint;

    //Connect to a EEPROM with ADR0 set to VCC and use the Wire1 hardware to talk to the EEPROM
    if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
    {
        DBUGS.println("No memory detected. Freezing.");
        while (true);
    }

    // save apikey to EEPROM
    ExtMem.putString(EEPROM_EMON_API_KEY_START, emoncms_apikey); //(location, data


    // save emoncms server to EEPROM max 45 characters
    ExtMem.putString(EEPROM_EMON_SERVER_START, emoncms_server); //(location, dat


    // save emoncms server to EEPROM max 16 characters
    ExtMem.putString(EEPROM_EMON_PATH_START, emoncms_path); //(location, dat


    // save emoncms node to EEPROM max 32 characters
    ExtMem.putString(EEPROM_EMON_NODE_START, emoncms_node); //(location, 


    // save emoncms HTTPS fingerprint to EEPROM max 60 characters
    ExtMem.putString(EEPROM_EMON_FINGERPRINT_START, emoncms_fingerprint); //(location, 


   
}

void EEPROM_Write_String(uint32_t Adds, String val) {

    

    if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
    {
        DBUGS.println("No memory detected. Freezing.");
        while (true);
    }

    // Save MQTT server max 45 characters
    ExtMem.put(Adds, val); //(location, data)



}

void config_save_mqtt(String server, String topic, String prefix, String user, String pass)
{
    
    Wire.begin();

    mqtt_server = server;
    mqtt_topic = topic;
    mqtt_feed_prefix = prefix;
    mqtt_user = user;
    mqtt_pass = pass;


    //Connect to a EEPROM with ADR0 set to VCC and use the Wire1 hardware to talk to the EEPROM
    if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
    {
        DBUGS.println("No memory detected. Freezing.");
        while (true);
    }

    // Save MQTT server max 45 characters
    ExtMem.putString(EEPROM_MQTT_SERVER_START, mqtt_server); //(location, data)

    // Save MQTT topic max 32 characters
    ExtMem.putString(EEPROM_MQTT_TOPIC_START, mqtt_topic); //(location, data)


    // Save MQTT topic separator max 10 characters
    ExtMem.putString(EEPROM_MQTT_FEED_PREFIX_START, mqtt_feed_prefix); //(location, data


    // Save MQTT username max 32 characters
    ExtMem.putString(EEPROM_MQTT_USER_START, mqtt_user); //(location, data

    // Save MQTT pass max 64 characters
    ExtMem.putString(EEPROM_MQTT_PASS_START, mqtt_pass); //(location, data
   
}



void config_save_admin(String user, String pass)
{
  //EEPROM.begin(EEPROM_SIZE);

  www_username = user;
  www_password = pass;

  Wire.begin();
  //Connect to a EEPROM with ADR0 set to VCC and use the Wire1 hardware to talk to the EEPROM
  if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
  {
      DBUGS.println("No memory detected. Freezing.");
      while (true);
  }

  ExtMem.put(EEPROM_WWW_USER_START, user); //(location, data)
  ExtMem.put(EEPROM_WWW_PASS_START, pass); //(location, data)



  //EEPROM.commit();
}



void config_save_wifi(String qsid, String qpass)
{

    Wire.begin();

    esid = qsid;
    epass = qpass;


    ExtMem.setMemoryType(256); // Valid types: 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1025, 2048

      //Connect to a EEPROM with ADR0 set to VCC and use the Wire1 hardware to talk to the EEPROM
    if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
    {
        DBUGS.println("No memory detected. Freezing.");
        while (true);
    }




    DBUGS.println();
    DBUGS.println();
    DBUGS.println("Memory detected!");
    DBUGS.println();
    ExtMem.put(EEPROM_ESID_START, qsid); //(location, data)
    //String qsid;
    //ExtMem.get(EEPROM_ESID_START, qsid); //location to read, thing to put data into
    //DBUGS.println("SSID : "+ qsid);


    ExtMem.put(EEPROM_EPASS_START, qpass); //(location, data)
    //String qpass;
    //ExtMem.get(EEPROM_EPASS_START, qpass); //location to read, thing to put data into
    //DBUGS.println("PASSWORD : "+ qpass);
   // DBUGS.println();
    //DBUGS.println();


}


void EEPROM_Read_String(uint32_t Adds, String DataRead) {
    
    Wire.begin();

    if (ExtMem.begin(EEPROM_ADDRESS, Wire) == false) //And Uno will fail to compile here
    {
        DBUGS.println("No memory detected. Freezing.");
        while (true);
    }

    ExtMem.get(Adds, DataRead); //location to read, thing to put data into


//return DataRead;
}


void config_reset()
{
    ExtMem.erase(0xff);
}



