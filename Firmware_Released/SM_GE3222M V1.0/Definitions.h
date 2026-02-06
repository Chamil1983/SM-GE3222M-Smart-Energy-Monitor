// Libraries
#include <Arduino.h>
#include <driver/adc.h>
#include <MCP23017.h>
#include "EnergyATM90E36.h"
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <MicrocontrollerID.h>
#include "emonesp.h"
#include <ezTime.h>
#include "config.h"
#include "Modbus_Serial.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include <Arduino_JSON.h>
#include <ssl_client.h>
#include <WiFiClientSecure.h>
#include <LiquidMenu.h>
#include <Button.h>





// ****************  VARIABLES / DEFINES / STATIC / STRUCTURES / CONSTANTS ****************
extern void DisplayBIN16(int var);
extern void DisplayHEX(unsigned long var, unsigned char numChars);
extern int CalculateADCAverage(int SensorChannel);
extern void PrintUnderline(String sText);
extern void PrintSeparator(String sText);
extern void InitialiseLCD();
extern void ConfigureBoard();
extern void ConfigIdle();
extern void PeripheralsIdle();
extern void ReadDataIdle(void);
extern void InputP1_Idle(void);
extern void InputP2_Idle(void);
extern void TestPeripherals();
extern void PeripheralsIdle();
extern void DigitalControl(uint8_t Pin, uint8_t state);
extern void Flash_LED(uint8_t SelectOutput, int ton, int toff, uint16_t numFlash);
extern void PowerOnLedTest(void);
extern void DisplayBoardConfiguration();
extern void OnOff_Output(uint8_t SelectOutput, uint8_t OnOff);
extern void ScanI2CBus();
extern void ReadPCBTemperature();
extern void ReadADCVoltage();
extern void CheckDCVINVoltage();
extern void ReadCT4Current();
extern String ReadStringCmd_FLASH(uint8_t* FlashPointer, uint8_t Lenght, boolean PrintCR, boolean NoPrint);
extern String getMacAddress();
extern void DebouncingInputPin(void);
extern void DisplayPower_Data(boolean DisplayFull);
extern uint16_t GetHoldreg(TRegister* reg, uint16_t val);
extern uint16_t SetHoldreg(TRegister* reg, uint16_t val);
extern float GetIreg(TRegister* reg, float val);
extern float SetIreg(TRegister* reg, float val);
extern void Modbus_Poling(void);
extern void Load_Reg_Callibration();
extern void Load_Power_Registers(void);
extern void Setup_Modbus(void);
extern void InitModbus(void);
extern void Configure_MODBUS_Registers(void);
extern void floatconv(float input, uint16_t* ha, uint16_t* hb);
extern void PrintTimeStamp();
extern void Setup_Webserver(void);
extern void Webserver_loop(void);
extern void DeviceInit();
extern void MonitorPinOnInputChange();
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Json Variable to Hold Data
JSONVar sysdata, configdata, readings;

// App
String AppVersion = "0324-1.1";
String AppAcronym = "SMENG";
String AppName = " SM-GE3222M ATM90E36 IoT Power Energy Meter - Development Code";
                   

// App USER
String LocationName = "Local Host"; // Enter Location of Device such as House, Solar etc.  Used for Serial Monitor and LCD.

//Define Sketch Revision and Firmware Version
#define SKETCH_SM_GE3222M_REV                 "SM-GE3222M(0324-1.1)"
String FirmwareVersion = "SM-GE3222M 0324-1.1V";

String HardwareRev = "SM-GE3222M 1.1V";

#define FORMAT_SPIFFS_IF_FAILED true

// define filename to store config file
const String config_filename = "/config.txt";
const String Cali1_filename = "/Calibration_1.txt";
const String Cali2_filename = "/Calibration_2.txt";
const String Cali3_filename = "/Calibration_3.txt";
const String Cali4_filename = "/Calibration_4.txt";

// Constants Application
uint64_t chipid = ESP.getEfuseMac(); // Get ChipID (essentially the MAC address)



static int getNodeId()
{
    unsigned long chip_id = ESP.getEfuseMac();
    DBUGVAR(chip_id);
    int chip_tmp = chip_id / 10000;
    chip_tmp = chip_tmp * 10000;
    DBUGVAR(chip_tmp);
    return (chip_id - chip_tmp);
}

const String node_type = NODE_TYPE;
const String node_description = NODE_DESCRIPTION;

uint16_t node_id = getNodeId();
char id[41];

String node_name_default = node_type + String(node_id);
String node_name = "SM-GE3222M(WiFi)";
String node_describe = "";


//===================================================================
String Vendor_Name = "Microcode Eng";
String Product_Code = "GE3222M";
String Revision_Number = "1.1";

String meter_name = "SM Energy";
String model = "SM-GE3222M";
String manufacturer = "Microcode Engineering";
String serial_no = "";
String hardware_rev = HardwareRev;
String firmware_ver = "SM-GE3222M 0324-1.1V";
String mac_add = getMacAddress();
String node = node_name;
uint16_t node_ID = node_id;

//===================================================================


//======================================================================
//  CONST STRING
const char STR_SKETCH_REV[]               PROGMEM = "SM-GE3222M Sketch Rev: ";
const char STR_REV[]                      PROGMEM = "SM-GE3222M Device Rev: ";

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

// A LiquidLine object can be used more that once.
LiquidLine back_line(11, 3, "/BACK");

LiquidLine Start_Line1(0, 0, "SM-GE3222M Init...");
LiquidLine Start_Line2(0, 1, AppAcronym + "/" + AppVersion);
LiquidScreen welcome_screen(Start_Line1, Start_Line2);

LiquidLine System_Start1(4, 0, AppAcronym + " Starting...");
LiquidLine System_Start2(8, 1, "SM_GE3222M");
LiquidScreen System_Start(System_Start1, System_Start2);

// This is the first menu.
LiquidMenu main_menu(lcd, welcome_screen, System_Start, 1);

LiquidSystem menu_system(main_menu=1);



// Constants USER
int VoltageRawFactor = 0;                // ADC Raw Adjustment for 2048 @ 1.65V Default 0
float VoltageFactor = 15.9;                // Adjust as needed for Voltage Calibration. Default 39
const int AverageSamples = 25;           // Average Multi-Samples.  Default 25
const int AverageDelay = 20;             // Average Multi-Sample Delay. Default 20
boolean EnableAveraging = true;          // Set to true to enable averaging (ESP32 DCV).  Default true
boolean DisableHardwareTest = false;     // Set to true to speed up booting.  Default false
boolean EnableNoiseFilterSquelch = true; // This realtes to NoiseFilterSquelch Threshold.  false returns raw values.  Default true.


// Constants
const int LoopDelay = 1;                        // Loop Delay in Seconds.  Default 1.
boolean EnableBasicLoop = false;                // Set to true to display, in Serial Monitor, loop readings and displaying only one per reset cycle.  Default false.
boolean EnableDisplayBoardConfiguration = true; // Set to true to display, in Serial Monitor, board software configuration Information if DisplayFull is true. Default true.
boolean EnableLCDLoop = true;                  // Set to true to enable LCD Display in Loop.  Over-ride via I2C Scan.  Check OLED Instance in IPEM_Hardware, for OLED Selection.  Default true.
boolean LCD_Enabled = true;
boolean MODBUS_TCPIPEnabled = false;
boolean MODBUS_RTUEnabled = true;
boolean pageChanged = false;

uint16_t LCDCount=0;                // LCD Information Count

boolean EnableMODBUS_ETH = false;


boolean  WiFiState = false;
boolean  WiFi_APMode = false;
boolean  WiFi_StaMode = false;

extern boolean Config_State, WiFiAP_Mode, WIFi_Mode, WIFiClient_Mode, ETH_Mode, MDBUS_Mode;
extern int SerialBaud, ETH_TCPPort, WiFiServerPort;
extern int   MDBUS_BAUD, MDBus_TCPPort;
extern String WIFiSSID, WiFiPassword, WIFi_State;
extern String Server_IP, ETH_HostIP, MDBus_TCPHostIP, WiFiHost_IP, WiFiGateway, WiFiSubnet, DNS1, DNS2;
extern String WiFiSoftAP_IP, SoftAP_SSID, SoftAP_Password;
extern uint8_t ModSlave_ID;
extern String Config_Mode, COM_MODE;
extern String Cal_Page;
extern float  PublishInterval;


//Enable/Disable Ethernet Interface
// =============================================================
//#define Enable_ETH



//Define TimeBase 1msec
#define TimerTicks 1000 
#define TIMEBASE 1

volatile int Ticks;
int TotalTicks;

String ActiveFlag = "";

#define T_1MSEC (1/TIMEBASE)
#define T_2MSEC (2/TIMEBASE)
#define T_5MSEC (5/TIMEBASE)
#define T_10MSEC (10/TIMEBASE)
#define T_20MSEC (20/TIMEBASE)
#define T_50MSEC (50/TIMEBASE)
#define T_100MSEC (100/TIMEBASE)
#define T_200MSEC (200/TIMEBASE)
#define T_500MSEC (500/TIMEBASE)

#define T_1SEC (1000/TIMEBASE)
#define T_2SEC (2000/TIMEBASE)
#define T_3SEC (3000/TIMEBASE)
#define T_4SEC (4000/TIMEBASE)
#define T_5SEC (5000/TIMEBASE)
#define T_6SEC (6000/TIMEBASE)
#define T_7SEC (7000/TIMEBASE)
#define T_8SEC (8000/TIMEBASE)
#define T_9SEC (9000/TIMEBASE)
#define T_10SEC (10000/TIMEBASE)
#define T_15SEC (15000/TIMEBASE)

hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


static uint32_t last_mem = 0;
static uint32_t start_mem = 0;
static unsigned long mem_info_update = 0;



//======================================================================
//  Timer  variables
uint16_t  DebouncingInput[2];
uint16_t  TimeOutInputPin[2];

uint16_t  TimeOutReadData;
uint16_t  TimeOutPrintData;
uint16_t  TimeOutLCDData;
uint16_t  TimeOutLCDClear;

uint16_t  TimeOutLEDPulse;
uint16_t  TimeOutLEDPulseOn;
uint16_t  TimeOutLEDPulseOff;
//======================================================================



//======================================================================
uint8_t OutputPin[6];
uint8_t LastStateOutputPin[6];
uint8_t InputPin[10];

union DigitalInput {
    struct {
        uint8_t  InputStatus : 1;
        uint8_t  InputReaded : 1;
        uint8_t  InputVar : 1;
        uint8_t  LastInputStatus : 1;
        uint8_t  Free : 4;
    } Bit;
} DigitalInput[10];


struct Switch {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;
//======================================================================



#define MCP_ADDR (0x20)

const uint8_t RELAY_2 = 0;      // GPA0 (21) of the MCP23017
const uint8_t RELAY_1 = 1;      // GPA1 (22) of the MCP23017
const uint8_t WIFI_LED = 2;      // GPA2 (23) of the MCP23017
const uint8_t RUN_LED = 3;      // GPA3 (24) of the MCP23017
const uint8_t FLT_LED = 4;      // GPA4 (25) of the MCP23017
const uint8_t MODBUS_LED = 5;      // GPA5 (26) of the MCP23017

const uint8_t IRQ1 = 8;   // GPB0 (1) of the MCP23017
const uint8_t IRQ0 = 9;   // GPB1 (2) of the MCP23017
const uint8_t ATM_WO = 10;  // GPB2 (3) of the MCP23017
const uint8_t CF4 = 11;  // GPB3 (4) of the MCP23017
const uint8_t CF3 = 12;  // GPB4 (5) of the MCP23017
const uint8_t CF2 = 13;  // GPB5 (6) of the MCP23017
const uint8_t CF1 = 14;  // GPB6 (7) of the MCP23017




#define LED_OFF                 1
#define LED_ON                  0


#define OUTPUT_STATE_OFF           0
#define OUTPUT_STATE_ON            1

// Define Logic Types
#define TRUE  0
#define FALSE 1


#ifdef LEDRun_Mode
#ifndef RUN_LED_ON_STATE
#define RUN_LED_ON_STATE LOW

#ifndef RUN_LED_OFF_STATE
#define RUN_LED_OFF_STATE HIGH

#endif
#endif
#endif



#ifndef MODBUS_LED_ON_STATE
#define MODBUS_LED_ON_STATE LOW

#ifndef MODBUS_LED_OFF_STATE
#define MODBUS_LED_OFF_STATE HIGH

#endif


#endif
#ifndef MODBUS_LED_STATE
#define MODBUS_LED_STATE LOW

#endif








#define DHTPIN 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);





// CT4 Energy Monitor Library Configuration with ESP32 ADC as the forth input
// Initialize EmonLib (111.1 = EmonCalibration value, adjust as needed)
// NB.  EmonCalibration set to 260 for low current bring-up testing < 1.5A with Burden NOT connected.  Update Burden and Values as required.
#if CT4_CONFIG == CT4_ESP || ATM90DEVICE == ATM90E32_DEVICE
float EmonCalibration = 260; // Used for bring-up EMON calibration value.  To be calibrated.  Default 1.
float EmonCalcIrms = 7400;   // Calculate EMON Irms. Default 1480
float EmonThreshold = 0.2;   // Used to squelch low values. Default 0.2
#endif




// Create an Energy Monitor Library Instance (Used ONLY for CT4).  Ignore if CT4 Isolated or used for GPIO
#if CT4_CONFIG == CT4_ESP || ATM90DEVICE == ATM90E32_DEVICE
EnergyMonitor emon1;
#endif

// **************** ATM90Ex CALIBRATION SETTINGS GUIDE ****************
// LineFreq = 389 for 50 Hz (World)  4485 for (North America)
// VoltageGain = Depends on Transformer used for Voltage Isolation.  Below values are based on Greenbrook DAT01A (TC TR7) Transformer, set to 12V AC.  https://www.tlc-direct.co.uk/Products/TCTR7.html
// CurrentGainCTx = Depends on CT Clamps Used.  You can sue different on each channel is needed.  Below based on SCT-013-000 100A/50mA

#if ATM90DEVICE == ATM90E32_DEVICE

// The below related to calibration of Voltage inputs (V1, V2 and V3), also Current Clamp inputs (CT1, CT2 and CT3 only).

// Mains Frequency
unsigned short LineFreq = 389; // MMode0 0x33 _lineFreq | Default 389

// Voltage and Current - Overall Gain
// PMPGA 0x17              DDV3V2V1I4I3I2I1  // D = DPGA       4/3/2/1 = V1-4 = I1-4
unsigned short PGAGain = 0b0101010101111111; // PMPGA 0x17  | DPGA Gain = 2 and PGA Gain = 1

// Voltage.
// This is calculated based on the Bell Transformer DAT01 on 12V setting @ ~19V RMS.  Need to allow for +/- ~ 1% Tolerance.
// Calculations: Base value for 240V is 38800.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
// Calculations: Base value for 120V is 20200.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
#if ATM_SINGLEVOLTAGE == true
unsigned short VoltageGain1 = 38800;        // uGain = UgainA | 0x61	0x0002 40500 20000 42620 (10000 = ~60V)
unsigned short VoltageGain2 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
unsigned short VoltageGain3 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
#else
unsigned short VoltageGain1 = 38800; // uGain = UgainA | 38800 Default Starting Value
unsigned short VoltageGain2 = 38800; // uGain = UgainB | 38800 Default Starting Value
unsigned short VoltageGain3 = 38800; // uGain = UgainC | 38800 Default Starting Value
#endif

// Current
// This is calculated based on the YDH? 100A/50mA  Need to allow for +/- ~ 1% Tolerance.  Trim values are fine!
// Calculations: Base value for CT100/50 Mid Range 38500.  To increase/decrease change by Approx. ~500 per 0.01A RMS
unsigned short CurrentGainCT1 = 38500; // IgainA 0x62	| CT100/50 Mid Range 38500
unsigned short CurrentGainCT2 = 38500; // IgainA 0x62	| CT100/50 Mid Range 38500
unsigned short CurrentGainCT3 = 38500; // IgainA 0x62	| CT100/50 Mid Range 38500
#endif

#if ATM90DEVICE == ATM90E36_DEVICE

// The below related to calibration of Voltage inputs (V1, V2 and V3), also Current Clamp inputs (CT1, CT2 and CT3 only).

// Mains Frequency
unsigned short LineFreq = 389; // MMode0 0x33 _lineFreq | 389

// Voltage and Current - Overall Gain
// PMPGA 0x17              DDV3V2V1I4I3I2I1  // D = DPGA       4/3/2/1 = V1-4 = I1-4
unsigned short PGAGain = 0b0101010101010101; // PMPGA 0x17  | DPGA Gain = 2 and PGA Gain = 1

// Voltage.
// This is calculated based on the DAT01 on 12V setting @ ~19V RMS.  Need to allow for +/- ~ 1% Tolerance.
// Calculations: Base value for 240V is 20200.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
// Calculations: Base value for 120V is 9700.  To increase/decrease change by Approx. ~100 per 1.0 V RMS.
#if ATM_SINGLEVOLTAGE == true
unsigned short VoltageGain1 = 20200;        // uGain = UgainA | 20200 Default Starting Value
unsigned short VoltageGain2 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
unsigned short VoltageGain3 = VoltageGain1; // Duplicate V1 Values to V2 and V3.
#else
unsigned short VoltageGain1 = 7143; // uGain = UgainA | 20200 Default Starting Value
unsigned short VoltageGain2 = 7171; // uGain = UgainB | 20200 Default Starting Value
unsigned short VoltageGain3 = 7180; // uGain = UgainC | 20200 Default Starting Value
#endif

// Current
// This is calculated based on the YDH? 100A/50mA  Need to allow for +/- ~ 1% Tolerance.  Trim values are fine!
// Calculations: Base value for CT100/50 Mid Range 33500.  To increase/decrease change by Approx. ~500 per 0.01A RMS
unsigned short CurrentGainCT1 = 45578; // IgainA 0x62
unsigned short CurrentGainCT2 = 45578; // IgainA 0x65
unsigned short CurrentGainCT3 = 45578; // IgainA 0x6A
unsigned short CurrentGainCTN = 33500; // IgainA 0x6E
#endif


// Variables
float TemperatureC;           // Temperature
float TemperatureF;           // Temperature
int VoltageSensorRaw;         // ADC Raw Voltage Value
float VoltageCalculated;      // Calculated Voltage Value
int VoltagePercentage;        // Voltage Percentage


// Variables ATM
float LineVoltage1, LineVoltage2, LineVoltage3, LineVoltageTotal, LineVoltageAverage;
float LineCurrentCT1, LineCurrentCT2, LineCurrentCT3, LineCurrentCT4, LineCurrentCTN, LineCurrentTotal;
float CalculatedPowerCT1, CalculatedPowerCT2, CalculatedPowerCT3, CalculatedPowerCT4, CalculatedPowerCTN, CalculatedTotalPower;
float ActivePowerCT1, ActivePowerCT2, ActivePowerCT3, TotalActivePower, CalculatedTotalActivePower;
float ActivePowerImportCT1, ActivePowerImportCT2, ActivePowerImportCT3, TotalActivePowerImport;
float ActivePowerExportCT1, ActivePowerExportCT2, ActivePowerExportCT3, TotalActivePowerExport;
float ReactivePowerCT1, ReactivePowerCT2, ReactivePowerCT3, TotalReactivePower, CalculatedTotalReactivePower;
float ApparentPowerCT1, ApparentPowerCT2, ApparentPowerCT3, TotalApparentPower, CalculatedTotalApparentPower;
float TotalActiveFundPower, TotalActiveHarPower;
float PowerFactorCT1, PowerFactorCT2, PowerFactorCT3, TotalPowerFactor;
float PhaseAngleCT1, PhaseAngleCT2, PhaseAngleCT3;
float ChipTemperature, LineFrequency;

// Variables PCB
float DCVoltage;      // DCV Input Voltage
float Hum, Temp_C, Temp_F, hif, hic;

// **************** ESP32 INPUTS ****************

#define DCV_IN 36      // GPIO 36 (Analog VP / ADC 1 CH0)
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define Set_Button 33 // GPIO 33 
#define Mode_Button 32 // GPIO 32 
#define INT_A 25 // GPIO 25 - MCP23017 PORT A Interrupt
#define INT_B 26 // GPIO 26 - MCP23017 PORT B Interrupt

Switch Mode_SW = { Mode_Button, 0, false };
Switch Set_SW = { Set_Button, 0, false };

const bool pullup = true;
Button ModeSW(Mode_Button, pullup);
Button SetSW(Set_Button, pullup);

const uint8_t MCP23017_INTB = INT_B;  // connected to MCP23017 INTB (19) pin
volatile bool switchDidChange = false;  // change status of MCP23017 input pins


// Define I2C (Expansion Port)
#define I2C_SDA 21
#define I2C_SCL 22

/*
  References to ESP32 SPI Expansion Port to ATM90E3x
  * CS = 5
  * MISO = 19
  * MOSI = 23
  * SCK = 18
 */




 //======================================================================
typedef void State;
typedef State(*Pstate)();
Pstate ComRequest1;             //  State Machine used to manage PC Communcation
Pstate ComRequest2;             //  State Machine used to manage PC Communcation
Pstate SetBoardConfigure;      //  State Machine used to manage to set board configurations 
Pstate Power_Management;       //  State Machine used to read data from MCP39F511
Pstate InputP1_Management;     //  State Machine used to manage P1 Button
Pstate InputP2_Management;     //  State Machine used to manage P2 Button
Pstate Peripheral_Management;  //  State Machine used to check if active power exceeds the active power Threshold
//======================================================================


union Settings {
    struct {
        uint8_t  ReadDataInProgress : 1;  //  If "1" reads data in progress
        uint8_t  SettingInProgress : 1;
        uint8_t  DisplayInProgress : 1;
        uint8_t  WiFiState : 1;
        uint8_t  WiFi_APMode : 1;
        uint8_t  WiFi_StaMode : 1;
        uint8_t  PcUartRequest     :1;  // If "1" communication request from PC UART
        uint8_t  TimerFlag1 : 1;
        uint8_t  TimerPCTimeout : 1;
        uint8_t  Free : 2;
        
    } Bit;
} Settings;
//======================================================================

union MCP23017_INT {
    struct {
        uint8_t  MCP23017_INTB : 1;  //  If "1" MCP23017 Port A Interrupt Occurs
        uint8_t  IRQ1_Flag : 1;
        uint8_t  IRQ0_Flag : 1;   
        uint8_t  WO_Flag : 1;
        uint8_t  CF1_Flag : 1;
        uint8_t  CF2_Flag : 1;
        uint8_t  CF3_Flag : 1;
        uint8_t  CF4_Flag : 1;
        
    } Bit;
} MCP23017_INT;
//======================================================================

union Display {
    struct {
        uint8_t  Voltage : 1;  //  If "1" reads data in progress
        uint8_t  Current : 1;
        uint8_t  CalPower : 1;
        uint8_t  ActPower : 1;
        uint8_t  ReactPower : 1;  // If "1" communication request from PC UART
        uint8_t  AppPower : 1;
        uint8_t  PFactor : 1;
        uint8_t  PAngle : 1;
        uint8_t  LineFreq : 1;
        uint8_t  THData : 1;
        uint8_t  ActEnergy_Imp : 1;
        uint8_t  ActEnergy_Exp : 1;
        uint8_t  ReactEnergy_Imp : 1;
        uint8_t  ReactEnergy_Exp : 1;
        uint8_t  ApptEnergy : 1;
        uint8_t  WiFiState : 1;

    } Flag;
} Display;


String TextForSms;
String humidity = " Humidity: %";
String temperature = "   Temperature";
String sign = " °C";

char fromc; // character from computer

String readString, SerialIn;



