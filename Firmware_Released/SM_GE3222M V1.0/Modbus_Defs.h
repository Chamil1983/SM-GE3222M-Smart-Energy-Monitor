#pragma once
//Modbus_Defs.h+

//#include <ModbusSerial.h>
//#include <Modbus.h>
#include "HardwareSerial.h"
#include <ModbusRTU.h>


#if defined(Enable_ETH)
#include <SPI.h>
#include <ModbusEthernet.h>
#include <Ethernet.h> 
#endif

ModbusRTU MODBUS;

#define Modbus_Port Serial2 // define serial port used, Serial most of the time, or Serial1, Serial2 ... if available

extern int   MDBUS_BAUD, MDBus_TCPPort;
extern String MDBus_TCPHostIP;
extern uint8_t ModSlave_ID;
extern boolean Config_State, ETH_Mode, MDBUS_Mode;

const uint8_t TxenPin = 27;
const uint8_t SLAVE_ID = 1;
const uint32_t Baudrate = MDBUS_BAUD;



const uint8_t rxPin = 16;
const uint8_t txPin = 17;

byte mac[] = {
	0x64, 0xB7,0x08, 0x58, 0xDC, 0x28
	
};

//ModbusSerial MODBUS(Modbus_Port, SlaveId, TxenPin);
/*


*/


long ts;






//==============================================================================================================
// Hardware Variables Digital Outputs
uint16_t LEDCoils[4], RelayCoils[8];
uint16_t Soft_Reset, CF1, CF2, ZX, IRQ, WO;
uint16_t WiFi_State, Fault_State, EEPROM_Connected_State, EEPROM_Bussy_State, DHT_State;

#define Soft_Reset	0x0100 //Read/Write Coil

#define LEDCoils[0] 0x0200
#define LEDCoils[1] 0x0201
#define LEDCoils[2] 0x0202
#define LEDCoils[3] 0x0203


#define RelayCoils[0] 0x0220
#define RelayCoils[1] 0x0221
#define RelayCoils[2] 0x0222
#define RelayCoils[3] 0x0223
#define RelayCoils[4] 0x0224
#define RelayCoils[5] 0x0225
#define RelayCoils[6] 0x0226
#define RelayCoils[7] 0x0227

#define CF1 0x0300
#define CF2 0x0301
#define ZX 0x0302
#define IRQ 0x0303
#define WO 0x0304

#define WiFi_State 0x0305
#define Fault_State 0x0306
#define EEPROM_Connected_State 0x0307
#define EEPROM_Bussy_State 0x0308
#define DHT_State 0x0309

//==============================================================================================================

// Hardware Variables Discrete Inputs

uint16_t Mode_Switch, Set_Switch;
uint16_t External_Inputs[10];




//Read Device Identification
#define VendorNameAdd           0x000  //10	R
#define ProductCodeAdd          0x01E  //10	R
#define RevisionNumberAdd		0x02D  //5	R



#define meter_nameAdd		0x082	//Iregister 130
#define modelAdd			0x096	//Iregister
#define manufacturerAdd		0x0AA	//Iregister	
#define serial_noAdd		0x0C8	//Iregister		
#define hardware_revAdd		0x0E6	//Iregister
#define firmware_verAdd		0x0EE	//Iregister	
#define mac_addAdd			0x102	//Iregister
#define nodeAdd				0x120	//Iregister
#define node_IDAdd			0x13E	//Iregister



#define meterDate_TimeAdd       240  //40


#define EnergyPulseDurationAdd				0x0850  //uint16_t 1 reg
#define ActiveEPulseOutputEnableAdd			0x0852  //uint16_t 1 reg
#define ReactiveEPulseOutputEnableAdd       0x0854  //uint16_t 1 reg

//Communications

#define ProtocolAdd					0x1901 //
#define Protocol_AddressAdd			0x1902 //
#define BaudRateAdd					0x1903 //
#define ParityAdd					0x1904 //

//Digital Inputs

#define DigitalInput_1CtrlModeAdd	0x1C69 //
#define DigitalInput_2CtrlModeAdd	0x1C81 //
#define DigitalInputStatusAdd		0x22C8 //

//Current, voltage, power, power factor and frequency (Holding Register Locations)

//================================================================================
// Energy Register VARIABLE
//================================================================================
#define TotalForwardActiveEnergyAdd					0         //  (Float) Total Forward Active Energy
#define PhaA_ForwardActiveEnergyAdd					2         //  (Float) Phase A Forward Active Energy
#define PhaB_ForwardActiveEnergyAdd					4         //  (Float) Phase B Forward Active Energy
#define PhaC_ForwardActiveEnergyAdd					6         //  (Float) Phase C Forward Active Energy
#define TotalReverseActiveEnergyAdd					8         //  (Float) Total Reverse Active Energy
#define PhaA_ReverseActiveEnergyAdd					10         //  (Float) Phase A Reverse Active Energy
#define PhaB_ReverseActiveEnergyAdd					12         //  (Float) Phase B Reverse Active Energy
#define PhaC_ReverseActiveEnergyAdd					14         //  (Float) Phase C Reverse Active Energy
#define TotalForwardReactiveEnergyAdd				16         //  (Float) Total Forward Reactive Energy
#define PhaA_ForwardReactiveEnergyAdd				18         //  (Float) Phase A Forward Reactive Energy
#define PhaB_ForwardReactiveEnergyAdd				20         //  (Float) Phase B Forward Reactive Energy
#define PhaC_ForwardReactiveEnergyAdd				22         //  (Float) Phase C Forward Reactive Energy
#define TotalReverseReactiveEnergyAdd				24         //  (Float) Total Reverse Reactive Energy
#define PhaA_ReverseReactivEEnergyAdd				26         //  (Float) Phase A Reverse Reactive Energy
#define PhaB_ReverseReactivEEnergyAdd				28         //  (Float) Phase B Reverse Reactive Energy
#define PhaC_ReverseReactivEEnergyAdd				30         //  (Float) Phase C Reverse Reactive Energy
#define Total_ArithmeticSum_ApparentEnergyAdd		32         //  (Float) Total (Arithmetic Sum) Apparent Energy

#define PhaA_ApparentEnergyAdd						34         //  (Float) Phase A Apparent Energy
#define PhaB_ApparentEnergyAdd						36         //  (Float) Phase B Apparent Energy
#define PhaC_ApparentEnergyAdd						38         //  (Float) Phase C Apparent Energy

#define VectorSum_TotalApparentEnergyAdd			40         //  (Float) (Vector Sum) Total Apparent Energy

#define MeteringStatus0Add							50            //  Metering Status 0
#define MeteringStatus1Add							52            //  Metering Status 1


#define VectorSum_TotalApparentPowerAdd				42         //  (Float) (Vector Sum) Total Apparent Power


//Power and Power Factor Register VARIABLES
//================================================================================
#define Total_PhasesumActivePowerAdd				60            //  (Float) Total (all-phase-sum) Active Power
#define PhaA_ActivePowerAdd							62                     //  Phase A Active Power
#define PhaB_ActivePowerAdd							64                     //  Phase B Active Power
#define PhaC_ActivePowerAdd							66                     //  Phase C Active Power
#define Total_PhasesumReactivePowerAdd				68          //  Total (all-phase-sum) Reactive Power
#define PhaA_ReactivePowerAdd						70                   //   Phase A Reactive Power   
#define PhaB_ReactivePowerAdd						72                  //   Phase B Reactive Power
#define PhaC_ReactivePowerAdd						74                   //   Phase C Reactive Power
#define Total_ArithmeticSumapparentpowerAdd			76     //  Total (Arithmetic Sum) apparent power
#define PhaA_apparentpowerAdd						78                   //  Phase A apparent power
#define PhaB_apparentpowerAdd						80                   //  Phase B apparent power
#define PhaC_apparentpowerAdd						82                   //  Phase C apparent power
#define TotalpowerfactorAdd							84                     //   Total power factor
#define PhaA_powerfactorAdd							86                     //  Phase A power factor
#define PhaB_powerfactorAdd							88                     //  Phase B power factor
#define PhaC_powerfactorAdd							90                     //  Phase C power factor

//==============================================================================
//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
#define TotalactivefundamentalpowerAdd				100           //  (Float) Total active fundamental power
#define PhaA_activefundamentalpowerAdd				102            //  (Float)  Phase A active fundamental power
#define PhaB_activefundamentalpowerAdd				104            //  (Float) Phase B active fundamental power
#define PhaC_activefundamentalpowerAdd				106            //  (Float) Phase C active fundamental power
#define TotalactiveharmonicpowerAdd					108            //  (Float) Total active harmonic power
#define PhaA_activeharmonicpowerAdd					110            //  (Float) phase A active harmonic power
#define PhaB_activeharmonicpowerAdd					112            //  (Float) phase B active harmonic power
#define PhaC_activeharmonicpowerAdd					114            //  (Float) phase C active harmonic power
#define N_Line_SampledcurrentRMSAdd					116            //  (Float) N Line Sampled current RMS
#define PhaA_voltageRMSAdd							118                      //  (Float) phase A voltage RMS
#define PhaB_voltageRMSAdd							120            //  (Float) phase B voltage RMS
#define PhaC_voltageRMSAdd							122            //  (Float) phase C voltage RMS
#define N_Line_calculatedcurrentRMSAdd				124            //  (Float) N Line calculated current RMS
#define PhaA_currentRMSAdd							126            //  (Float) phase A current RMS
#define PhaB_currentRMSAdd							128            //  (Float) phase B current RMS
#define PhaC_currentRMSAdd							130            //  (Float) phase C current RMS
#define PhaA_voltageTHDNAdd							132            //  (Float) phase A voltage THD+N
#define PhaB_voltageTHDNAdd							134            //  (Float) phase B voltage THD+N
#define PhaC_voltageTHDNAdd							136            //  (Float) phase C voltage THD+N
#define PhaA_currentTHDNAdd							138            //  (Float) phase A current THD+N
#define PhaB_currentTHDNAdd							140            //  (Float) phase B current THD+N
#define PhaC_currentTHDNAdd							142            //  (Float) phase C current THD+N

#define FrequencyAdd								144            //  (Float) Frequency
#define PhaA_meanphaseangleAdd						146            //  (Float)  phase A mean phase angle
#define PhaB_meanphaseangleAdd						148            //  (Float) phase B mean phase angle
#define PhaC_meanphaseangleAdd						150            //  (Float)  phase C mean phase angle
#define MeasuredtemperatureAdd						152            //  (Float)  Measured temperature
#define PhaA_voltagephaseangleAdd					154            //  (Float)  phase A voltage phase angle
#define PhaB_voltagephaseangleAdd					156            //  (Float)  phase B voltage phase angle 
#define PhaC_voltagephaseangleAdd					158            //  (Float)  phase C voltage phase angle 




//#define TemperatureAdd			0x0835 // 
#define HumidityAdd				180 //
#define TemperatureFAdd			184 //
#define TemperatureFAFAdd		188 //
#define HicAdd					192 //
#define HifAdd					196 //


long scale = 10;
long lconv = 0;



enum
{
	TempA  = 20,      
	TempB,      
	HumA,      
	HumB,      
	TempFA,     
	TempFB,     

	INPUT_REGS_SIZE
};

uint16_t InputReg[INPUT_REGS_SIZE];


//Energy and input metering
#define EnergyRstDate_TimeAdd					0x0CB3 //






union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}APEnergyT;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}APEnergyA;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}APEnergyB;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}APEnergyC;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}ANEnergyT;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}ANEnergyA;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}ANEnergyB;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}ANEnergyC;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}RPEnergyT;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}RPEnergyA;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}RPEnergyB;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}RPEnergyC;

//======================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}RNEnergyT;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}RNEnergyA;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}RNEnergyB;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}RNEnergyC;

//=========================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SAEnergyT;


union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SEnergyA;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SEnergyB;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SEnergyC;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SVEnergyT;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SVMeanT;

//========================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanT;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanA;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanB;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanC;

//=================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}QMeanT;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}QMeanA;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}QMeanB;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}QMeanC;


//===================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SAMeanT;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SMeanA;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SMeanB;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}SMeanC;

//=======================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;
	};

}PFMeanT;

union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;
	};

}PFMeanA;

union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;
	};

}PFMeanB;

union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;
	};

}PFMeanC;

//========================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanTF;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanAF;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanBF;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanCF;

//===============================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanTH;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanAH;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanBH;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}PMeanCH;

//===============================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}N1Irms;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}N0Irms;

//=================================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}Urms_A;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}Urms_B;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}Urms_C;

//===================================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}Irms_A;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}Irms_B;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}Irms_C;

//========================================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}THDNU_A;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}THDNU_B;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}THDNU_C;

//===================================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}THDNI_A;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}THDNI_B;

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}THDNI_C;

//============================================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		uint16_t IntVal_A;
		uint16_t DecVal_B;
	};

}FREQ;

//============================================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;
		
	};

}PAngle_A;

union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;

	};

}PAngle_B;

union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;

	};

}PAngle_C;

//==========================================================================================================================================

union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;

	};

}BoardTemp;

//==========================================================================================================================================
union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;

	};

}Uangle_A;


union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;

	};

}Uangle_B;

union
{
	unsigned long Val;
	struct
	{
		int16_t IntVal_A;
		int16_t DecVal_B;

	};

}Uangle_C;


//Status,Metering Calibration and Configuration (Input Register Locations)

#define     CalRegistersAdd			0
#define     HarCalRegistersAdd		28
#define     MeasurCalRegistersAdd	44	


