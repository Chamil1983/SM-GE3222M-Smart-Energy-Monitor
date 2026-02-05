 
#include "Modbus_Serial.h" 
#include "Modbus_Defs.h" 
 


// Enter a MAC address and IP address for your controller below.
IPAddress mod_ip(169, 254, 33, 100); // The IP address will be dependent on your local network:
ModbusEthernet MODEthernet;



void Setup_Modbus(void) { 
 
	if (MDBUS_Mode) {

	}


	Modbus_Port.begin(Baudrate, SERIAL_8E1);
 
 
 
#if defined(ESP32) || defined(ESP8266) 
	MODBUS.begin(&Modbus_Port, TxenPin); 
#else 
	// mb.begin(&Serial); 
	mb.begin(&Serial2, RXD2, TXD2); //or use RX/TX direction control pin (if required) 
	mb.setBaudrate(9600); 
#endif 
 
 
 
	//MODBUS.begin(&Modbus_Port,TxenPin); 
	MODBUS.setBaudrate(Baudrate); 
	MODBUS.slave(SLAVE_ID);

	if (MDBUS_Mode) {
#define Enable_MDBUS

#if defined(Enable_MDBUS)
		Ethernet.init(14);        // SS pin
		Ethernet.begin(mac, mod_ip);  // start the Ethernet connection
		delay(1000);              // give the Ethernet shield a second to initialize
		MODEthernet.server();
#endif 

	}



	
            // Act as Modbus TCP server

	

	Configure_MODBUS_Registers();
	//InitModbus();

		 
} 
 
 
 
 
void Configure_MODBUS_Registers(void) { 
 
	uint8_t regadd = 0;
	Load_Reg_Callibration();
	//Load_Power_Registers(); 
	 
	 	
	do {
		MODBUS.addHreg((CalRegistersAdd + (2 * regadd)), CalRegisters[regadd]);
		
	} while (++regadd < sizeof(CalRegisters));

	regadd = 0;

	do {
		MODBUS.addHreg((HarCalRegistersAdd + (2 * regadd)), HarCalRegisters[regadd]);
		
	} while (++regadd < sizeof(HarCalRegisters));

	regadd = 0;

	do {
		MODBUS.addHreg((MeasurCalRegistersAdd + (2 * regadd)), MeasurCalRegisters[regadd]);
	} while (++regadd < sizeof(MeasurCalRegisters));

#if defined(Enable_ETH)
	if (MDBUS_Mode == true) {

		do {
			MODEthernet.addHreg((CalRegistersAdd + (2 * regadd)), CalRegisters[regadd]);

		} while (++regadd < sizeof(CalRegisters));

		regadd = 0;

		do {
			MODEthernet.addHreg((HarCalRegistersAdd + (2 * regadd)), HarCalRegisters[regadd]);

		} while (++regadd < sizeof(HarCalRegisters));

		regadd = 0;

		do {
			MODEthernet.addHreg((MeasurCalRegistersAdd + (2 * regadd)), MeasurCalRegisters[regadd]);
		} while (++regadd < sizeof(MeasurCalRegisters));

	}
#endif 
  

	//================================================================================
	// Add Energy Register on MODBUS
	//================================================================================
MODBUS.addIreg(TotalForwardActiveEnergyAdd);         //  (Float) Total Forward Active Energy
MODBUS.addIreg(PhaA_ForwardActiveEnergyAdd);	       //  (Float) Phase A Forward Active Energy
MODBUS.addIreg(PhaB_ForwardActiveEnergyAdd);         //  (Float) Phase B Forward Active Energy
MODBUS.addIreg(PhaC_ForwardActiveEnergyAdd);         //  (Float) Phase C Forward Active Energy
MODBUS.addIreg(TotalReverseActiveEnergyAdd);         //  (Float) Total Reverse Active Energy
MODBUS.addIreg(PhaA_ReverseActiveEnergyAdd);         //  (Float) Phase A Reverse Active Energy
MODBUS.addIreg(PhaB_ReverseActiveEnergyAdd);         //  (Float) Phase B Reverse Active Energy
MODBUS.addIreg(PhaC_ReverseActiveEnergyAdd);         //  (Float) Phase C Reverse Active Energy
MODBUS.addIreg(TotalForwardReactiveEnergyAdd);         //  (Float) Total Forward Reactive Energy
MODBUS.addIreg(PhaA_ForwardReactiveEnergyAdd);         //  (Float) Phase A Forward Reactive Energy
MODBUS.addIreg(PhaB_ForwardReactiveEnergyAdd);         //  (Float) Phase B Forward Reactive Energy
MODBUS.addIreg(PhaC_ForwardReactiveEnergyAdd);         //  (Float) Phase C Forward Reactive Energy
MODBUS.addIreg(TotalReverseReactiveEnergyAdd);         //  (Float) Total Reverse Reactive Energy
MODBUS.addIreg(PhaA_ReverseReactivEEnergyAdd);         //  (Float) Phase A Reverse Reactive Energy
MODBUS.addIreg(PhaB_ReverseReactivEEnergyAdd);         //  (Float) Phase B Reverse Reactive Energy
MODBUS.addIreg(PhaC_ReverseReactivEEnergyAdd);         //  (Float) Phase C Reverse Reactive Energy
MODBUS.addIreg(Total_ArithmeticSum_ApparentEnergyAdd);         //  (Float) Total (Arithmetic Sum) Apparent Energy

MODBUS.addIreg(PhaA_ApparentEnergyAdd);         //  (Float) Phase A Apparent Energy
MODBUS.addIreg(PhaB_ApparentEnergyAdd);         //  (Float) Phase B Apparent Energy
MODBUS.addIreg(PhaC_ApparentEnergyAdd);         //  (Float) Phase C Apparent Energy

MODBUS.addIreg(VectorSum_TotalApparentEnergyAdd);         //  (Float) (Vector Sum) Total Apparent Energy

MODBUS.addIreg(MeteringStatus0Add);            //  Metering Status 0
MODBUS.addIreg(MeteringStatus1Add);            //  Metering Status 1


MODBUS.addIreg(VectorSum_TotalApparentPowerAdd);         //  (Float) (Vector Sum) Total Apparent Power


//Power and Power Factor Register VARIABLES
//================================================================================
MODBUS.addIreg(Total_PhasesumActivePowerAdd);            //  (Float) Total (all-phase-sum) Active Power
MODBUS.addIreg(PhaA_ActivePowerAdd);                     //  Phase A Active Power
MODBUS.addIreg(PhaB_ActivePowerAdd);                     //  Phase B Active Power
MODBUS.addIreg(PhaC_ActivePowerAdd);                     //  Phase C Active Power
MODBUS.addIreg(Total_PhasesumReactivePowerAdd);          //  Total (all-phase-sum) Reactive Power
MODBUS.addIreg(PhaA_ReactivePowerAdd);                   //   Phase A Reactive Power   
MODBUS.addIreg(PhaB_ReactivePowerAdd);                  //   Phase B Reactive Power
MODBUS.addIreg(PhaC_ReactivePowerAdd);                   //   Phase C Reactive Power
MODBUS.addIreg(Total_ArithmeticSumapparentpowerAdd);     //  Total (Arithmetic Sum) apparent power
MODBUS.addIreg(PhaA_apparentpowerAdd);                   //  Phase A apparent power
MODBUS.addIreg(PhaB_apparentpowerAdd);                   //  Phase B apparent power
MODBUS.addIreg(PhaC_apparentpowerAdd);                  //  Phase C apparent power
MODBUS.addIreg(TotalpowerfactorAdd);                     //   Total power factor
MODBUS.addIreg(PhaA_powerfactorAdd);                     //  Phase A power factor
MODBUS.addIreg(PhaB_powerfactorAdd);                     //  Phase B power factor
MODBUS.addIreg(PhaC_powerfactorAdd);                     //  Phase C power factor

//==============================================================================
//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
MODBUS.addIreg(TotalactivefundamentalpowerAdd);         //  (Float) Total active fundamental power
MODBUS.addIreg(PhaA_activefundamentalpowerAdd);         //  (Float)  Phase A active fundamental power
MODBUS.addIreg(PhaB_activefundamentalpowerAdd);         //  (Float) Phase B active fundamental power
MODBUS.addIreg(PhaC_activefundamentalpowerAdd);         //  (Float) Phase C active fundamental power
MODBUS.addIreg(TotalactiveharmonicpowerAdd);			//  (Float) Total active harmonic power
MODBUS.addIreg(PhaA_activeharmonicpowerAdd);            //  (Float) phase A active harmonic power
MODBUS.addIreg(PhaB_activeharmonicpowerAdd);            //  (Float) phase B active harmonic power
MODBUS.addIreg(PhaC_activeharmonicpowerAdd);            //  (Float) phase C active harmonic power
MODBUS.addIreg(N_Line_SampledcurrentRMSAdd);            //  (Float) N Line Sampled current RMS
MODBUS.addIreg(PhaA_voltageRMSAdd);                     //  (Float) phase A voltage RMS
MODBUS.addIreg(PhaB_voltageRMSAdd);						//  (Float) phase B voltage RMS
MODBUS.addIreg(PhaC_voltageRMSAdd);						//  (Float) phase C voltage RMS
MODBUS.addIreg(N_Line_calculatedcurrentRMSAdd);         //  (Float) N Line calculated current RMS
MODBUS.addIreg(PhaA_currentRMSAdd);						//  (Float) phase A current RMS
MODBUS.addIreg(PhaB_currentRMSAdd);						//  (Float) phase B current RMS
MODBUS.addIreg(PhaC_currentRMSAdd);						//  (Float) phase C current RMS
MODBUS.addIreg(PhaA_voltageTHDNAdd);					//  (Float) phase A voltage THD+N
MODBUS.addIreg(PhaB_voltageTHDNAdd);					//  (Float) phase B voltage THD+N
MODBUS.addIreg(PhaC_voltageTHDNAdd);					//  (Float) phase C voltage THD+N
MODBUS.addIreg(PhaA_currentTHDNAdd);					//  (Float) phase A current THD+N
MODBUS.addIreg(PhaB_currentTHDNAdd);					//  (Float) phase B current THD+N
MODBUS.addIreg(PhaC_currentTHDNAdd);					//  (Float) phase C current THD+N

MODBUS.addIreg(FrequencyAdd);							//  (Float) Frequency
MODBUS.addIreg(PhaA_meanphaseangleAdd);					//  (Float)  phase A mean phase angle
MODBUS.addIreg(PhaB_meanphaseangleAdd);					//  (Float) phase B mean phase angle
MODBUS.addIreg(PhaC_meanphaseangleAdd);					//  (Float)  phase C mean phase angle
MODBUS.addIreg(MeasuredtemperatureAdd);					//  (Float)  Measured temperature
MODBUS.addIreg(PhaA_voltagephaseangleAdd);				//  (Float)  phase A voltage phase angle
MODBUS.addIreg(PhaB_voltagephaseangleAdd);				//  (Float)  phase B voltage phase angle 
MODBUS.addIreg(PhaC_voltagephaseangleAdd);				//  (Float)  phase C voltage phase angle 


//=============================================================================================================

MODBUS.addIreg(TotalForwardActiveEnergyAdd+1);         //  (Float) Total Forward Active Energy
MODBUS.addIreg(PhaA_ForwardActiveEnergyAdd + 1);	       //  (Float) Phase A Forward Active Energy
MODBUS.addIreg(PhaB_ForwardActiveEnergyAdd + 1);         //  (Float) Phase B Forward Active Energy
MODBUS.addIreg(PhaC_ForwardActiveEnergyAdd + 1);         //  (Float) Phase C Forward Active Energy
MODBUS.addIreg(TotalReverseActiveEnergyAdd + 1);         //  (Float) Total Reverse Active Energy
MODBUS.addIreg(PhaA_ReverseActiveEnergyAdd + 1);         //  (Float) Phase A Reverse Active Energy
MODBUS.addIreg(PhaB_ReverseActiveEnergyAdd + 1);         //  (Float) Phase B Reverse Active Energy
MODBUS.addIreg(PhaC_ReverseActiveEnergyAdd + 1);         //  (Float) Phase C Reverse Active Energy
MODBUS.addIreg(TotalForwardReactiveEnergyAdd + 1);         //  (Float) Total Forward Reactive Energy
MODBUS.addIreg(PhaA_ForwardReactiveEnergyAdd + 1);         //  (Float) Phase A Forward Reactive Energy
MODBUS.addIreg(PhaB_ForwardReactiveEnergyAdd + 1);         //  (Float) Phase B Forward Reactive Energy
MODBUS.addIreg(PhaC_ForwardReactiveEnergyAdd + 1);         //  (Float) Phase C Forward Reactive Energy
MODBUS.addIreg(TotalReverseReactiveEnergyAdd + 1);         //  (Float) Total Reverse Reactive Energy
MODBUS.addIreg(PhaA_ReverseReactivEEnergyAdd + 1);         //  (Float) Phase A Reverse Reactive Energy
MODBUS.addIreg(PhaB_ReverseReactivEEnergyAdd + 1);         //  (Float) Phase B Reverse Reactive Energy
MODBUS.addIreg(PhaC_ReverseReactivEEnergyAdd + 1);         //  (Float) Phase C Reverse Reactive Energy
MODBUS.addIreg(Total_ArithmeticSum_ApparentEnergyAdd + 1);         //  (Float) Total (Arithmetic Sum) Apparent Energy

MODBUS.addIreg(PhaA_ApparentEnergyAdd + 1);         //  (Float) Phase A Apparent Energy
MODBUS.addIreg(PhaB_ApparentEnergyAdd + 1);         //  (Float) Phase B Apparent Energy
MODBUS.addIreg(PhaC_ApparentEnergyAdd + 1);         //  (Float) Phase C Apparent Energy

MODBUS.addIreg(VectorSum_TotalApparentEnergyAdd + 1);         //  (Float) (Vector Sum) Total Apparent Energy


MODBUS.addIreg(VectorSum_TotalApparentPowerAdd + 1);         //  (Float) (Vector Sum) Total Apparent Power


//Power and Power Factor Register VARIABLES
//================================================================================
MODBUS.addIreg(Total_PhasesumActivePowerAdd + 1);            //  (Float) Total (all-phase-sum) Active Power
MODBUS.addIreg(PhaA_ActivePowerAdd + 1);                     //  Phase A Active Power
MODBUS.addIreg(PhaB_ActivePowerAdd + 1);                     //  Phase B Active Power
MODBUS.addIreg(PhaC_ActivePowerAdd + 1);                     //  Phase C Active Power
MODBUS.addIreg(Total_PhasesumReactivePowerAdd + 1);          //  Total (all-phase-sum) Reactive Power
MODBUS.addIreg(PhaA_ReactivePowerAdd + 1);                   //   Phase A Reactive Power   
MODBUS.addIreg(PhaB_ReactivePowerAdd + 1);                  //   Phase B Reactive Power
MODBUS.addIreg(PhaC_ReactivePowerAdd + 1);                   //   Phase C Reactive Power
MODBUS.addIreg(Total_ArithmeticSumapparentpowerAdd + 1);     //  Total (Arithmetic Sum) apparent power
MODBUS.addIreg(PhaA_apparentpowerAdd + 1);                   //  Phase A apparent power
MODBUS.addIreg(PhaB_apparentpowerAdd + 1);                   //  Phase B apparent power
MODBUS.addIreg(PhaC_apparentpowerAdd + 1);                  //  Phase C apparent power
MODBUS.addIreg(TotalpowerfactorAdd + 1);                     //   Total power factor
MODBUS.addIreg(PhaA_powerfactorAdd + 1);                     //  Phase A power factor
MODBUS.addIreg(PhaB_powerfactorAdd + 1);                     //  Phase B power factor
MODBUS.addIreg(PhaC_powerfactorAdd + 1);                     //  Phase C power factor

//==============================================================================
//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
MODBUS.addIreg(TotalactivefundamentalpowerAdd + 1);         //  (Float) Total active fundamental power
MODBUS.addIreg(PhaA_activefundamentalpowerAdd + 1);         //  (Float)  Phase A active fundamental power
MODBUS.addIreg(PhaB_activefundamentalpowerAdd + 1);         //  (Float) Phase B active fundamental power
MODBUS.addIreg(PhaC_activefundamentalpowerAdd + 1);         //  (Float) Phase C active fundamental power
MODBUS.addIreg(TotalactiveharmonicpowerAdd + 1);			//  (Float) Total active harmonic power
MODBUS.addIreg(PhaA_activeharmonicpowerAdd + 1);            //  (Float) phase A active harmonic power
MODBUS.addIreg(PhaB_activeharmonicpowerAdd + 1);            //  (Float) phase B active harmonic power
MODBUS.addIreg(PhaC_activeharmonicpowerAdd + 1);            //  (Float) phase C active harmonic power
MODBUS.addIreg(N_Line_SampledcurrentRMSAdd + 1);            //  (Float) N Line Sampled current RMS
MODBUS.addIreg(PhaA_voltageRMSAdd + 1);                     //  (Float) phase A voltage RMS
MODBUS.addIreg(PhaB_voltageRMSAdd + 1);						//  (Float) phase B voltage RMS
MODBUS.addIreg(PhaC_voltageRMSAdd + 1);						//  (Float) phase C voltage RMS
MODBUS.addIreg(N_Line_calculatedcurrentRMSAdd + 1);         //  (Float) N Line calculated current RMS
MODBUS.addIreg(PhaA_currentRMSAdd + 1);						//  (Float) phase A current RMS
MODBUS.addIreg(PhaB_currentRMSAdd + 1);						//  (Float) phase B current RMS
MODBUS.addIreg(PhaC_currentRMSAdd + 1);						//  (Float) phase C current RMS
MODBUS.addIreg(PhaA_voltageTHDNAdd + 1);					//  (Float) phase A voltage THD+N
MODBUS.addIreg(PhaB_voltageTHDNAdd + 1);					//  (Float) phase B voltage THD+N
MODBUS.addIreg(PhaC_voltageTHDNAdd + 1);					//  (Float) phase C voltage THD+N
MODBUS.addIreg(PhaA_currentTHDNAdd + 1);					//  (Float) phase A current THD+N
MODBUS.addIreg(PhaB_currentTHDNAdd + 1);					//  (Float) phase B current THD+N
MODBUS.addIreg(PhaC_currentTHDNAdd + 1);					//  (Float) phase C current THD+N

MODBUS.addIreg(FrequencyAdd + 1);							//  (Float) Frequency
MODBUS.addIreg(PhaA_meanphaseangleAdd + 1);					//  (Float)  phase A mean phase angle
MODBUS.addIreg(PhaB_meanphaseangleAdd + 1);					//  (Float) phase B mean phase angle
MODBUS.addIreg(PhaC_meanphaseangleAdd + 1);					//  (Float)  phase C mean phase angle
MODBUS.addIreg(MeasuredtemperatureAdd + 1);					//  (Float)  Measured temperature
MODBUS.addIreg(PhaA_voltagephaseangleAdd + 1);				//  (Float)  phase A voltage phase angle
MODBUS.addIreg(PhaB_voltagephaseangleAdd + 1);				//  (Float)  phase B voltage phase angle 
MODBUS.addIreg(PhaC_voltagephaseangleAdd + 1);				//  (Float)  phase C voltage phase angle


	/// <summary> 
	/// /////////////// 
	/// </summary> 
	/// <param name=""></param> 



	floatconv(Temp_C, &InputReg[TempA], &InputReg[TempB]); 
	floatconv(Temp_F, &InputReg[TempFA], &InputReg[TempFB]); 
	floatconv(Hum, &InputReg[HumA], &InputReg[HumB]); 
 
	MODBUS.addIreg(TempA); 
	MODBUS.addIreg(TempB); 
	MODBUS.addIreg(TempFA); 
	MODBUS.addIreg(TempFB); 
	MODBUS.addIreg(HumA); 
	MODBUS.addIreg(HumB); 


#if defined(Enable_ETH)
	if (MDBUS_Mode == true) {

		//================================================================================
// Add Energy Register on MODBUS
//================================================================================
		MODEthernet.addIreg(TotalForwardActiveEnergyAdd);         //  (Float) Total Forward Active Energy
		MODEthernet.addIreg(PhaA_ForwardActiveEnergyAdd);	       //  (Float) Phase A Forward Active Energy
		MODEthernet.addIreg(PhaB_ForwardActiveEnergyAdd);         //  (Float) Phase B Forward Active Energy
		MODEthernet.addIreg(PhaC_ForwardActiveEnergyAdd);         //  (Float) Phase C Forward Active Energy
		MODEthernet.addIreg(TotalReverseActiveEnergyAdd);         //  (Float) Total Reverse Active Energy
		MODEthernet.addIreg(PhaA_ReverseActiveEnergyAdd);         //  (Float) Phase A Reverse Active Energy
		MODEthernet.addIreg(PhaB_ReverseActiveEnergyAdd);         //  (Float) Phase B Reverse Active Energy
		MODEthernet.addIreg(PhaC_ReverseActiveEnergyAdd);         //  (Float) Phase C Reverse Active Energy
		MODEthernet.addIreg(TotalForwardReactiveEnergyAdd);         //  (Float) Total Forward Reactive Energy
		MODEthernet.addIreg(PhaA_ForwardReactiveEnergyAdd);         //  (Float) Phase A Forward Reactive Energy
		MODEthernet.addIreg(PhaB_ForwardReactiveEnergyAdd);         //  (Float) Phase B Forward Reactive Energy
		MODEthernet.addIreg(PhaC_ForwardReactiveEnergyAdd);         //  (Float) Phase C Forward Reactive Energy
		MODEthernet.addIreg(TotalReverseReactiveEnergyAdd);         //  (Float) Total Reverse Reactive Energy
		MODEthernet.addIreg(PhaA_ReverseReactivEEnergyAdd);         //  (Float) Phase A Reverse Reactive Energy
		MODEthernet.addIreg(PhaB_ReverseReactivEEnergyAdd);         //  (Float) Phase B Reverse Reactive Energy
		MODEthernet.addIreg(PhaC_ReverseReactivEEnergyAdd);         //  (Float) Phase C Reverse Reactive Energy
		MODEthernet.addIreg(Total_ArithmeticSum_ApparentEnergyAdd);         //  (Float) Total (Arithmetic Sum) Apparent Energy

		MODEthernet.addIreg(PhaA_ApparentEnergyAdd);         //  (Float) Phase A Apparent Energy
		MODEthernet.addIreg(PhaB_ApparentEnergyAdd);         //  (Float) Phase B Apparent Energy
		MODEthernet.addIreg(PhaC_ApparentEnergyAdd);         //  (Float) Phase C Apparent Energy

		MODEthernet.addIreg(VectorSum_TotalApparentEnergyAdd);         //  (Float) (Vector Sum) Total Apparent Energy

		MODEthernet.addIreg(MeteringStatus0Add);            //  Metering Status 0
		MODEthernet.addIreg(MeteringStatus1Add);            //  Metering Status 1


		MODEthernet.addIreg(VectorSum_TotalApparentPowerAdd);         //  (Float) (Vector Sum) Total Apparent Power


		//Power and Power Factor Register VARIABLES
		//================================================================================
		MODEthernet.addIreg(Total_PhasesumActivePowerAdd);            //  (Float) Total (all-phase-sum) Active Power
		MODEthernet.addIreg(PhaA_ActivePowerAdd);                     //  Phase A Active Power
		MODEthernet.addIreg(PhaB_ActivePowerAdd);                     //  Phase B Active Power
		MODEthernet.addIreg(PhaC_ActivePowerAdd);                     //  Phase C Active Power
		MODEthernet.addIreg(Total_PhasesumReactivePowerAdd);          //  Total (all-phase-sum) Reactive Power
		MODEthernet.addIreg(PhaA_ReactivePowerAdd);                   //   Phase A Reactive Power   
		MODEthernet.addIreg(PhaB_ReactivePowerAdd);                  //   Phase B Reactive Power
		MODEthernet.addIreg(PhaC_ReactivePowerAdd);                   //   Phase C Reactive Power
		MODEthernet.addIreg(Total_ArithmeticSumapparentpowerAdd);     //  Total (Arithmetic Sum) apparent power
		MODEthernet.addIreg(PhaA_apparentpowerAdd);                   //  Phase A apparent power
		MODEthernet.addIreg(PhaB_apparentpowerAdd);                   //  Phase B apparent power
		MODEthernet.addIreg(PhaC_apparentpowerAdd);                  //  Phase C apparent power
		MODEthernet.addIreg(TotalpowerfactorAdd);                     //   Total power factor
		MODEthernet.addIreg(PhaA_powerfactorAdd);                     //  Phase A power factor
		MODEthernet.addIreg(PhaB_powerfactorAdd);                     //  Phase B power factor
		MODEthernet.addIreg(PhaC_powerfactorAdd);                     //  Phase C power factor

		//==============================================================================
		//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
		MODEthernet.addIreg(TotalactivefundamentalpowerAdd);         //  (Float) Total active fundamental power
		MODEthernet.addIreg(PhaA_activefundamentalpowerAdd);         //  (Float)  Phase A active fundamental power
		MODEthernet.addIreg(PhaB_activefundamentalpowerAdd);         //  (Float) Phase B active fundamental power
		MODEthernet.addIreg(PhaC_activefundamentalpowerAdd);         //  (Float) Phase C active fundamental power
		MODEthernet.addIreg(TotalactiveharmonicpowerAdd);			//  (Float) Total active harmonic power
		MODEthernet.addIreg(PhaA_activeharmonicpowerAdd);            //  (Float) phase A active harmonic power
		MODEthernet.addIreg(PhaB_activeharmonicpowerAdd);            //  (Float) phase B active harmonic power
		MODEthernet.addIreg(PhaC_activeharmonicpowerAdd);            //  (Float) phase C active harmonic power
		MODEthernet.addIreg(N_Line_SampledcurrentRMSAdd);            //  (Float) N Line Sampled current RMS
		MODEthernet.addIreg(PhaA_voltageRMSAdd);                     //  (Float) phase A voltage RMS
		MODEthernet.addIreg(PhaB_voltageRMSAdd);						//  (Float) phase B voltage RMS
		MODEthernet.addIreg(PhaC_voltageRMSAdd);						//  (Float) phase C voltage RMS
		MODEthernet.addIreg(N_Line_calculatedcurrentRMSAdd);         //  (Float) N Line calculated current RMS
		MODEthernet.addIreg(PhaA_currentRMSAdd);						//  (Float) phase A current RMS
		MODEthernet.addIreg(PhaB_currentRMSAdd);						//  (Float) phase B current RMS
		MODEthernet.addIreg(PhaC_currentRMSAdd);						//  (Float) phase C current RMS
		MODEthernet.addIreg(PhaA_voltageTHDNAdd);					//  (Float) phase A voltage THD+N
		MODEthernet.addIreg(PhaB_voltageTHDNAdd);					//  (Float) phase B voltage THD+N
		MODEthernet.addIreg(PhaC_voltageTHDNAdd);					//  (Float) phase C voltage THD+N
		MODEthernet.addIreg(PhaA_currentTHDNAdd);					//  (Float) phase A current THD+N
		MODEthernet.addIreg(PhaB_currentTHDNAdd);					//  (Float) phase B current THD+N
		MODEthernet.addIreg(PhaC_currentTHDNAdd);					//  (Float) phase C current THD+N

		MODEthernet.addIreg(FrequencyAdd);							//  (Float) Frequency
		MODEthernet.addIreg(PhaA_meanphaseangleAdd);					//  (Float)  phase A mean phase angle
		MODEthernet.addIreg(PhaB_meanphaseangleAdd);					//  (Float) phase B mean phase angle
		MODEthernet.addIreg(PhaC_meanphaseangleAdd);					//  (Float)  phase C mean phase angle
		MODEthernet.addIreg(MeasuredtemperatureAdd);					//  (Float)  Measured temperature
		MODEthernet.addIreg(PhaA_voltagephaseangleAdd);				//  (Float)  phase A voltage phase angle
		MODEthernet.addIreg(PhaB_voltagephaseangleAdd);				//  (Float)  phase B voltage phase angle 
		MODEthernet.addIreg(PhaC_voltagephaseangleAdd);				//  (Float)  phase C voltage phase angle 

		//=========================================================================================================

		MODEthernet.addIreg(TotalForwardActiveEnergyAdd + 1);         //  (Float) Total Forward Active Energy
		MODEthernet.addIreg(PhaA_ForwardActiveEnergyAdd + 1);	       //  (Float) Phase A Forward Active Energy
		MODEthernet.addIreg(PhaB_ForwardActiveEnergyAdd + 1);         //  (Float) Phase B Forward Active Energy
		MODEthernet.addIreg(PhaC_ForwardActiveEnergyAdd + 1);         //  (Float) Phase C Forward Active Energy
		MODEthernet.addIreg(TotalReverseActiveEnergyAdd + 1);         //  (Float) Total Reverse Active Energy
		MODEthernet.addIreg(PhaA_ReverseActiveEnergyAdd + 1);         //  (Float) Phase A Reverse Active Energy
		MODEthernet.addIreg(PhaB_ReverseActiveEnergyAdd + 1);         //  (Float) Phase B Reverse Active Energy
		MODEthernet.addIreg(PhaC_ReverseActiveEnergyAdd + 1);         //  (Float) Phase C Reverse Active Energy
		MODEthernet.addIreg(TotalForwardReactiveEnergyAdd + 1);         //  (Float) Total Forward Reactive Energy
		MODEthernet.addIreg(PhaA_ForwardReactiveEnergyAdd + 1);         //  (Float) Phase A Forward Reactive Energy
		MODEthernet.addIreg(PhaB_ForwardReactiveEnergyAdd + 1);         //  (Float) Phase B Forward Reactive Energy
		MODEthernet.addIreg(PhaC_ForwardReactiveEnergyAdd + 1);         //  (Float) Phase C Forward Reactive Energy
		MODEthernet.addIreg(TotalReverseReactiveEnergyAdd + 1);         //  (Float) Total Reverse Reactive Energy
		MODEthernet.addIreg(PhaA_ReverseReactivEEnergyAdd + 1);         //  (Float) Phase A Reverse Reactive Energy
		MODEthernet.addIreg(PhaB_ReverseReactivEEnergyAdd + 1);         //  (Float) Phase B Reverse Reactive Energy
		MODEthernet.addIreg(PhaC_ReverseReactivEEnergyAdd + 1);         //  (Float) Phase C Reverse Reactive Energy
		MODEthernet.addIreg(Total_ArithmeticSum_ApparentEnergyAdd + 1);         //  (Float) Total (Arithmetic Sum) Apparent Energy

		MODEthernet.addIreg(PhaA_ApparentEnergyAdd + 1);         //  (Float) Phase A Apparent Energy
		MODEthernet.addIreg(PhaB_ApparentEnergyAdd + 1);         //  (Float) Phase B Apparent Energy
		MODEthernet.addIreg(PhaC_ApparentEnergyAdd + 1);         //  (Float) Phase C Apparent Energy

		MODEthernet.addIreg(VectorSum_TotalApparentEnergyAdd + 1);         //  (Float) (Vector Sum) Total Apparent Energy


		MODEthernet.addIreg(VectorSum_TotalApparentPowerAdd + 1);         //  (Float) (Vector Sum) Total Apparent Power


		//Power and Power Factor Register VARIABLES
		//================================================================================
		MODEthernet.addIreg(Total_PhasesumActivePowerAdd+1);            //  (Float) Total (all-phase-sum) Active Power
		MODEthernet.addIreg(PhaA_ActivePowerAdd + 1);                     //  Phase A Active Power
		MODEthernet.addIreg(PhaB_ActivePowerAdd + 1);                     //  Phase B Active Power
		MODEthernet.addIreg(PhaC_ActivePowerAdd + 1);                     //  Phase C Active Power
		MODEthernet.addIreg(Total_PhasesumReactivePowerAdd + 1);          //  Total (all-phase-sum) Reactive Power
		MODEthernet.addIreg(PhaA_ReactivePowerAdd + 1);                   //   Phase A Reactive Power   
		MODEthernet.addIreg(PhaB_ReactivePowerAdd + 1);                  //   Phase B Reactive Power
		MODEthernet.addIreg(PhaC_ReactivePowerAdd + 1);                   //   Phase C Reactive Power
		MODEthernet.addIreg(Total_ArithmeticSumapparentpowerAdd + 1);     //  Total (Arithmetic Sum) apparent power
		MODEthernet.addIreg(PhaA_apparentpowerAdd + 1);                   //  Phase A apparent power
		MODEthernet.addIreg(PhaB_apparentpowerAdd + 1);                   //  Phase B apparent power
		MODEthernet.addIreg(PhaC_apparentpowerAdd + 1);                  //  Phase C apparent power
		MODEthernet.addIreg(TotalpowerfactorAdd + 1);                     //   Total power factor
		MODEthernet.addIreg(PhaA_powerfactorAdd + 1);                     //  Phase A power factor
		MODEthernet.addIreg(PhaB_powerfactorAdd + 1);                     //  Phase B power factor
		MODEthernet.addIreg(PhaC_powerfactorAdd + 1);                     //  Phase C power factor

		//==============================================================================
		//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
		MODEthernet.addIreg(TotalactivefundamentalpowerAdd + 1);         //  (Float) Total active fundamental power
		MODEthernet.addIreg(PhaA_activefundamentalpowerAdd + 1);         //  (Float)  Phase A active fundamental power
		MODEthernet.addIreg(PhaB_activefundamentalpowerAdd + 1);         //  (Float) Phase B active fundamental power
		MODEthernet.addIreg(PhaC_activefundamentalpowerAdd + 1);         //  (Float) Phase C active fundamental power
		MODEthernet.addIreg(TotalactiveharmonicpowerAdd + 1);			//  (Float) Total active harmonic power
		MODEthernet.addIreg(PhaA_activeharmonicpowerAdd + 1);            //  (Float) phase A active harmonic power
		MODEthernet.addIreg(PhaB_activeharmonicpowerAdd + 1);            //  (Float) phase B active harmonic power
		MODEthernet.addIreg(PhaC_activeharmonicpowerAdd + 1);            //  (Float) phase C active harmonic power
		MODEthernet.addIreg(N_Line_SampledcurrentRMSAdd + 1);            //  (Float) N Line Sampled current RMS
		MODEthernet.addIreg(PhaA_voltageRMSAdd + 1);                     //  (Float) phase A voltage RMS
		MODEthernet.addIreg(PhaB_voltageRMSAdd + 1);						//  (Float) phase B voltage RMS
		MODEthernet.addIreg(PhaC_voltageRMSAdd + 1);						//  (Float) phase C voltage RMS
		MODEthernet.addIreg(N_Line_calculatedcurrentRMSAdd + 1);         //  (Float) N Line calculated current RMS
		MODEthernet.addIreg(PhaA_currentRMSAdd + 1);						//  (Float) phase A current RMS
		MODEthernet.addIreg(PhaB_currentRMSAdd + 1);						//  (Float) phase B current RMS
		MODEthernet.addIreg(PhaC_currentRMSAdd + 1);						//  (Float) phase C current RMS
		MODEthernet.addIreg(PhaA_voltageTHDNAdd + 1);					//  (Float) phase A voltage THD+N
		MODEthernet.addIreg(PhaB_voltageTHDNAdd + 1);					//  (Float) phase B voltage THD+N
		MODEthernet.addIreg(PhaC_voltageTHDNAdd + 1);					//  (Float) phase C voltage THD+N
		MODEthernet.addIreg(PhaA_currentTHDNAdd + 1);					//  (Float) phase A current THD+N
		MODEthernet.addIreg(PhaB_currentTHDNAdd + 1);					//  (Float) phase B current THD+N
		MODEthernet.addIreg(PhaC_currentTHDNAdd + 1);					//  (Float) phase C current THD+N

		MODEthernet.addIreg(FrequencyAdd + 1);							//  (Float) Frequency
		MODEthernet.addIreg(PhaA_meanphaseangleAdd + 1);					//  (Float)  phase A mean phase angle
		MODEthernet.addIreg(PhaB_meanphaseangleAdd + 1);					//  (Float) phase B mean phase angle
		MODEthernet.addIreg(PhaC_meanphaseangleAdd + 1);					//  (Float)  phase C mean phase angle
		MODEthernet.addIreg(MeasuredtemperatureAdd + 1);					//  (Float)  Measured temperature
		MODEthernet.addIreg(PhaA_voltagephaseangleAdd + 1);				//  (Float)  phase A voltage phase angle
		MODEthernet.addIreg(PhaB_voltagephaseangleAdd + 1);				//  (Float)  phase B voltage phase angle 
		MODEthernet.addIreg(PhaC_voltagephaseangleAdd + 1);				//  (Float)  phase C voltage phase angle 


			/// <summary> 
			/// /////////////// 
			/// </summary> 
			/// <param name=""></param> 



		floatconv(Temp_C, &InputReg[TempA], &InputReg[TempB]);
		floatconv(Temp_F, &InputReg[TempFA], &InputReg[TempFB]);
		floatconv(Hum, &InputReg[HumA], &InputReg[HumB]);

		MODEthernet.addIreg(TempA);
		MODEthernet.addIreg(TempB);
		MODEthernet.addIreg(TempFA);
		MODEthernet.addIreg(TempFB);
		MODEthernet.addIreg(HumA);
		MODEthernet.addIreg(HumB);


	}

#endif
	 
} 
 
 
 
void InitModbus(void) { 
	Load_Reg_Callibration();
	Load_Power_Registers();

	uint8_t regadd = 0;


	do {
		MODBUS.Hreg((CalRegistersAdd + (2 * regadd)), CalRegisters[regadd]);

	} while (++regadd < sizeof(CalRegisters));

	regadd = 0;

	do {
		MODBUS.Hreg((HarCalRegistersAdd + (2 * regadd)), HarCalRegisters[regadd]);

	} while (++regadd < sizeof(HarCalRegisters));

	regadd = 0;

	do {
		MODBUS.Hreg((MeasurCalRegistersAdd + (2 * regadd)), MeasurCalRegisters[regadd]);
	} while (++regadd < sizeof(MeasurCalRegisters));

	
	APEnergyT.IntVal_A = lround(TotalForwardActiveEnergy);
	APEnergyT.DecVal_B = lround((TotalForwardActiveEnergy - APEnergyT.IntVal_A) * 100);

	APEnergyA.IntVal_A = lround(PhaA_ForwardActiveEnergy);
	APEnergyA.DecVal_B = lround((PhaA_ForwardActiveEnergy - APEnergyA.IntVal_A) * 100);

	APEnergyB.IntVal_A = lround(PhaB_ForwardActiveEnergy);
	APEnergyB.DecVal_B = lround((PhaB_ForwardActiveEnergy - APEnergyB.IntVal_A) * 100);

	APEnergyC.IntVal_A = lround(PhaC_ForwardActiveEnergy);
	APEnergyC.DecVal_B = lround((PhaC_ForwardActiveEnergy - APEnergyC.IntVal_A) * 100);

	ANEnergyT.IntVal_A = lround(TotalReverseActiveEnergy);
	ANEnergyT.DecVal_B = lround((TotalReverseActiveEnergy - ANEnergyT.IntVal_A) * 100);

	ANEnergyA.IntVal_A = lround(PhaA_ReverseActiveEnergy);
	ANEnergyA.DecVal_B = lround((PhaA_ReverseActiveEnergy - ANEnergyA.IntVal_A) * 100);

	ANEnergyB.IntVal_A = lround(PhaB_ReverseActiveEnergy);
	ANEnergyB.DecVal_B = lround((PhaB_ReverseActiveEnergy - ANEnergyB.IntVal_A) * 100);

	ANEnergyC.IntVal_A = lround(PhaC_ReverseActiveEnergy);
	ANEnergyC.DecVal_B = lround((PhaC_ReverseActiveEnergy - ANEnergyC.IntVal_A) * 100);

	RPEnergyT.IntVal_A = lround(TotalForwardReactiveEnergy);
	RPEnergyT.DecVal_B = lround((TotalForwardReactiveEnergy - RPEnergyT.IntVal_A) * 100);

	RPEnergyA.IntVal_A = lround(PhaA_ForwardReactiveEnergy);
	RPEnergyA.DecVal_B = lround((PhaA_ForwardReactiveEnergy - RPEnergyA.IntVal_A) * 100);

	RPEnergyB.IntVal_A = lround(PhaB_ForwardReactiveEnergy);
	RPEnergyB.DecVal_B = lround((PhaB_ForwardReactiveEnergy - RPEnergyB.IntVal_A) * 100);

	RPEnergyC.IntVal_A = lround(PhaC_ForwardReactiveEnergy);
	RPEnergyC.DecVal_B = lround((PhaC_ForwardReactiveEnergy - RPEnergyC.IntVal_A) * 100);

	RNEnergyT.IntVal_A = lround(TotalReverseReactiveEnergy);
	RNEnergyT.DecVal_B = lround((TotalReverseReactiveEnergy - RNEnergyT.IntVal_A) * 100);

	RNEnergyA.IntVal_A = lround(PhaA_ReverseReactiveEnergy);
	RNEnergyA.DecVal_B = lround((PhaA_ReverseReactiveEnergy - RNEnergyA.IntVal_A) * 100);

	RNEnergyB.IntVal_A = lround(PhaB_ReverseReactiveEnergy);
	RNEnergyB.DecVal_B = lround((PhaB_ReverseReactiveEnergy - RNEnergyB.IntVal_A) * 100);

	RNEnergyC.IntVal_A = lround(PhaC_ReverseReactiveEnergy);
	RNEnergyC.DecVal_B = lround((PhaC_ReverseReactiveEnergy - RNEnergyC.IntVal_A) * 100);

	SAEnergyT.IntVal_A = lround(Total_ArithmeticSum_ApparentEnergy);
	SAEnergyT.DecVal_B = lround((Total_ArithmeticSum_ApparentEnergy - SAEnergyT.IntVal_A) * 100);

	SEnergyA.IntVal_A = lround(PhaA_ApparentEnergy);
	SEnergyA.DecVal_B = lround((PhaA_ApparentEnergy - SEnergyA.IntVal_A) * 100);

	SEnergyB.IntVal_A = lround(PhaB_ApparentEnergy);
	SEnergyB.DecVal_B = lround((PhaB_ApparentEnergy - SEnergyB.IntVal_A) * 100);

	SEnergyC.IntVal_A = lround(PhaC_ApparentEnergy);
	SEnergyC.DecVal_B = lround((PhaC_ApparentEnergy - SEnergyC.IntVal_A) * 100);

	SVEnergyT.IntVal_A = lround(VectorSum_TotalApparentEnergy);
	SVEnergyT.DecVal_B = lround((VectorSum_TotalApparentEnergy - SVEnergyT.IntVal_A) * 100);

	SVMeanT.IntVal_A = lround(VectorSum_TotalApparentPower);
	SVMeanT.DecVal_B = lround((VectorSum_TotalApparentPower - SVMeanT.IntVal_A) * 1000);

	PMeanT.IntVal_A = lround(Total_PhasesumActivePower);
	PMeanT.DecVal_B = lround((Total_PhasesumActivePower - PMeanT.IntVal_A) * 1000);

	PMeanA.IntVal_A = lround(PhaA_ActivePower);
	PMeanA.DecVal_B = lround((PhaA_ActivePower - PMeanA.IntVal_A) * 1000);

	PMeanB.IntVal_A = lround(PhaB_ActivePower);
	PMeanB.DecVal_B = lround((PhaB_ActivePower - PMeanB.IntVal_A) * 1000);

	PMeanC.IntVal_A = lround(PhaC_ActivePower);
	PMeanC.DecVal_B = lround((PhaC_ActivePower - PMeanC.IntVal_A) * 1000);

	QMeanT.IntVal_A = lround(Total_PhasesumReactivePower);
	QMeanT.DecVal_B = lround((Total_PhasesumReactivePower - QMeanT.IntVal_A) * 1000);

	QMeanA.IntVal_A = lround(PhaA_ReactivePower);
	QMeanA.DecVal_B = lround((PhaA_ReactivePower - QMeanA.IntVal_A) * 1000);

	QMeanB.IntVal_A = lround(PhaB_ReactivePower);
	QMeanB.DecVal_B = lround((PhaB_ReactivePower - QMeanB.IntVal_A) * 1000);

	QMeanC.IntVal_A = lround(PhaC_ReactivePower);
	QMeanC.DecVal_B = lround((PhaC_ReactivePower - QMeanC.IntVal_A) * 1000);

	SAMeanT.IntVal_A = lround(Total_ArithmeticSumapparentpower);
	SAMeanT.DecVal_B = lround((Total_ArithmeticSumapparentpower - SAMeanT.IntVal_A) * 1000);

	SMeanA.IntVal_A = lround(PhaA_apparentpower);
	SMeanA.DecVal_B = lround((PhaA_apparentpower - SMeanA.IntVal_A) * 1000);

	SMeanB.IntVal_A = lround(PhaB_apparentpower);
	SMeanB.DecVal_B = lround((PhaB_apparentpower - SMeanB.IntVal_A) * 1000);

	SMeanC.IntVal_A = lround(PhaC_apparentpower);
	SMeanC.DecVal_B = lround((PhaC_apparentpower - SMeanC.IntVal_A) * 1000);

	PFMeanT.IntVal_A = lround(Totalpowerfactor);
	PFMeanT.DecVal_B = lround((Totalpowerfactor - PFMeanT.IntVal_A) * 1000);

	PFMeanA.IntVal_A = lround(PhaA_powerfactor);
	PFMeanA.DecVal_B = lround((PhaA_powerfactor - PFMeanA.IntVal_A) * 1000);

	PFMeanB.IntVal_A = lround(PhaB_powerfactor);
	PFMeanB.DecVal_B = lround((PhaB_powerfactor - PFMeanB.IntVal_A) * 1000);

	PFMeanC.IntVal_A = lround(PhaC_powerfactor);
	PFMeanC.DecVal_B = lround((PhaC_powerfactor - PFMeanC.IntVal_A) * 1000);

	PMeanTF.IntVal_A = lround(Totalactivefundamentalpower);
	PMeanTF.DecVal_B = lround((Totalactivefundamentalpower - PMeanTF.IntVal_A) * 1000);

	PMeanAF.IntVal_A = lround(PhaA_activefundamentalpower);
	PMeanAF.DecVal_B = lround((PhaA_activefundamentalpower - PMeanAF.IntVal_A) * 1000);

	PMeanBF.IntVal_A = lround(PhaB_activefundamentalpower);
	PMeanBF.DecVal_B = lround((PhaB_activefundamentalpower - PMeanBF.IntVal_A) * 1000);

	PMeanCF.IntVal_A = lround(PhaC_activefundamentalpower);
	PMeanCF.DecVal_B = lround((PhaC_activefundamentalpower - PMeanAF.IntVal_A) * 1000);

	PMeanTH.IntVal_A = lround(Totalactiveharmonicpower);
	PMeanTH.DecVal_B = lround((Totalactiveharmonicpower - PMeanTH.IntVal_A) * 1000);

	PMeanAH.IntVal_A = lround(PhaA_activeharmonicpower);
	PMeanAH.DecVal_B = lround((PhaA_activeharmonicpower - PMeanAH.IntVal_A) * 1000);

	PMeanBH.IntVal_A = lround(PhaB_activeharmonicpower);
	PMeanBH.DecVal_B = lround((PhaB_activeharmonicpower - PMeanBH.IntVal_A) * 1000);

	PMeanCH.IntVal_A = lround(PhaC_activeharmonicpower);
	PMeanCH.DecVal_B = lround((PhaC_activeharmonicpower - PMeanCH.IntVal_A) * 1000);

	N1Irms.IntVal_A = lround(N_Line_SampledcurrentRMS);
	N1Irms.DecVal_B = lround((N_Line_SampledcurrentRMS - N1Irms.IntVal_A) * 1000);

	N0Irms.IntVal_A = lround(N_Line_calculatedcurrentRMS);
	N0Irms.DecVal_B = lround((N_Line_calculatedcurrentRMS - N0Irms.IntVal_A) * 1000);

	Urms_A.IntVal_A = lround(PhaA_voltageRMS);
	Urms_A.DecVal_B = lround((PhaA_voltageRMS - Urms_A.IntVal_A) * 100);

	Urms_B.IntVal_A = lround(PhaB_voltageRMS);
	Urms_B.DecVal_B = lround((PhaB_voltageRMS - Urms_B.IntVal_A) * 100);

	Urms_C.IntVal_A = lround(PhaC_voltageRMS);
	Urms_C.DecVal_B = lround((PhaC_voltageRMS - Urms_C.IntVal_A) * 100);

	Irms_A.IntVal_A = lround(PhaA_currentRMS);
	Irms_A.DecVal_B = lround((PhaA_currentRMS - Irms_A.IntVal_A) * 1000);

	Irms_B.IntVal_A = lround(PhaB_currentRMS);
	Irms_B.DecVal_B = lround((PhaB_currentRMS - Irms_B.IntVal_A) * 1000);

	Irms_C.IntVal_A = lround(PhaC_currentRMS);
	Irms_C.DecVal_B = lround((PhaC_currentRMS - Irms_C.IntVal_A) * 1000);

	THDNU_A.IntVal_A = lround(PhaA_voltageTHDN);
	THDNU_A.DecVal_B = lround((PhaA_voltageTHDN - THDNU_A.IntVal_A) * 100);

	THDNU_B.IntVal_A = lround(PhaB_voltageTHDN);
	THDNU_B.DecVal_B = lround((PhaB_voltageTHDN - THDNU_B.IntVal_A) * 100);

	THDNU_C.IntVal_A = lround(PhaC_voltageTHDN);
	THDNU_C.DecVal_B = lround((PhaC_voltageTHDN - THDNU_C.IntVal_A) * 100);

	THDNI_A.IntVal_A = lround(PhaA_currentTHDN);
	THDNI_A.DecVal_B = lround((PhaA_currentTHDN - THDNI_A.IntVal_A) * 1000);

	THDNI_B.IntVal_A = lround(PhaB_currentTHDN);
	THDNI_B.DecVal_B = lround((PhaB_currentTHDN - THDNI_B.IntVal_A) * 1000);

	THDNI_C.IntVal_A = lround(PhaC_currentTHDN);
	THDNI_C.DecVal_B = lround((PhaC_currentTHDN - THDNI_C.IntVal_A) * 1000);

	FREQ.IntVal_A = lround(Frequency);
	FREQ.DecVal_B = lround((Frequency - FREQ.IntVal_A) * 100);

	PAngle_A.IntVal_A = lround(PhaA_meanphaseangle);
	PAngle_A.DecVal_B = lround((PhaA_meanphaseangle - PAngle_A.IntVal_A) * 10);

	PAngle_B.IntVal_A = lround(PhaB_meanphaseangle);
	PAngle_B.DecVal_B = lround((PhaB_meanphaseangle - PAngle_B.IntVal_A) * 10);

	PAngle_C.IntVal_A = lround(PhaC_meanphaseangle);
	PAngle_C.DecVal_B = lround((PhaC_meanphaseangle - PAngle_C.IntVal_A) * 10);

	BoardTemp.IntVal_A = lround(Measuredtemperature);
	BoardTemp.DecVal_B = lround((Measuredtemperature - BoardTemp.IntVal_A) * 10);

	Uangle_A.IntVal_A = lround(PhaA_voltagephaseangle);
	Uangle_A.DecVal_B = lround((PhaA_voltagephaseangle - Uangle_A.IntVal_A) * 10);

	Uangle_B.IntVal_A = lround(PhaB_voltagephaseangle);
	Uangle_B.DecVal_B = lround((PhaB_voltagephaseangle - Uangle_B.IntVal_A) * 10);

	Uangle_C.IntVal_A = lround(PhaC_voltagephaseangle);
	Uangle_C.DecVal_B = lround((PhaC_voltagephaseangle - Uangle_C.IntVal_A) * 10);



//================================================================================
// Energy Register VARIABLE
//================================================================================
	MODBUS.Ireg(TotalForwardActiveEnergyAdd, APEnergyT.IntVal_A);         //  (Float) Total Forward Active Energy
	MODBUS.Ireg(TotalForwardActiveEnergyAdd+1, APEnergyT.DecVal_B);         //  (Float) Total Forward Active Energy
	MODBUS.Ireg(PhaA_ForwardActiveEnergyAdd, APEnergyA.IntVal_A);	       //  (Float) Phase A Forward Active Energy
	MODBUS.Ireg(PhaA_ForwardActiveEnergyAdd+1, APEnergyA.DecVal_B);	       //  (Float) Phase A Forward Active Energy
	MODBUS.Ireg(PhaB_ForwardActiveEnergyAdd, APEnergyB.IntVal_A);         //  (Float) Phase B Forward Active Energy
	MODBUS.Ireg(PhaB_ForwardActiveEnergyAdd+1, APEnergyB.DecVal_B);         //  (Float) Phase B Forward Active Energy
	MODBUS.Ireg(PhaC_ForwardActiveEnergyAdd, APEnergyC.IntVal_A);         //  (Float) Phase C Forward Active Energy
	MODBUS.Ireg(PhaC_ForwardActiveEnergyAdd+1, APEnergyC.DecVal_B);         //  (Float) Phase C Forward Active Energy
	MODBUS.Ireg(TotalReverseActiveEnergyAdd, ANEnergyT.IntVal_A);         //  (Float) Total Reverse Active Energy
	MODBUS.Ireg(TotalReverseActiveEnergyAdd+1, ANEnergyT.DecVal_B);         //  (Float) Total Reverse Active Energy
	MODBUS.Ireg(PhaA_ReverseActiveEnergyAdd, ANEnergyA.IntVal_A);         //  (Float) Phase A Reverse Active Energy
	MODBUS.Ireg(PhaA_ReverseActiveEnergyAdd+1, ANEnergyA.DecVal_B);         //  (Float) Phase A Reverse Active Energy
	MODBUS.Ireg(PhaB_ReverseActiveEnergyAdd, ANEnergyB.IntVal_A);         //  (Float) Phase B Reverse Active Energy
	MODBUS.Ireg(PhaB_ReverseActiveEnergyAdd+1, ANEnergyB.DecVal_B);         //  (Float) Phase B Reverse Active Energy
	MODBUS.Ireg(PhaC_ReverseActiveEnergyAdd, ANEnergyC.IntVal_A);         //  (Float) Phase C Reverse Active Energy
	MODBUS.Ireg(PhaC_ReverseActiveEnergyAdd+1, ANEnergyC.DecVal_B);         //  (Float) Phase C Reverse Active Energy
	MODBUS.Ireg(TotalForwardReactiveEnergyAdd, RPEnergyT.IntVal_A);         //  (Float) Total Forward Reactive Energy
	MODBUS.Ireg(TotalForwardReactiveEnergyAdd+1, RPEnergyT.DecVal_B);         //  (Float) Total Forward Reactive Energy
	MODBUS.Ireg(PhaA_ForwardReactiveEnergyAdd, RPEnergyA.IntVal_A);         //  (Float) Phase A Forward Reactive Energy
	MODBUS.Ireg(PhaA_ForwardReactiveEnergyAdd+1, RPEnergyA.DecVal_B);         //  (Float) Phase A Forward Reactive Energy
	MODBUS.Ireg(PhaB_ForwardReactiveEnergyAdd, RPEnergyB.IntVal_A);         //  (Float) Phase B Forward Reactive Energy
	MODBUS.Ireg(PhaB_ForwardReactiveEnergyAdd+1, RPEnergyB.DecVal_B);         //  (Float) Phase B Forward Reactive Energ
	MODBUS.Ireg(PhaC_ForwardReactiveEnergyAdd, RPEnergyC.IntVal_A);         //  (Float) Phase C Forward Reactive Energy
	MODBUS.Ireg(PhaC_ForwardReactiveEnergyAdd+1, RPEnergyC.DecVal_B);         //  (Float) Phase C Forward Reactive Energy
	MODBUS.Ireg(TotalReverseReactiveEnergyAdd, RNEnergyT.IntVal_A);         //  (Float) Total Reverse Reactive Energy
	MODBUS.Ireg(TotalReverseReactiveEnergyAdd+1, RNEnergyT.DecVal_B);         //  (Float) Total Reverse Reactive Energy
	MODBUS.Ireg(PhaA_ReverseReactivEEnergyAdd, RNEnergyA.IntVal_A);         //  (Float) Phase A Reverse Reactive Energy
	MODBUS.Ireg(PhaA_ReverseReactivEEnergyAdd+1, RNEnergyA.DecVal_B);         //  (Float) Phase A Reverse Reactive Energy
	MODBUS.Ireg(PhaB_ReverseReactivEEnergyAdd, RNEnergyB.IntVal_A);         //  (Float) Phase B Reverse Reactive Energy
	MODBUS.Ireg(PhaB_ReverseReactivEEnergyAdd+1, RNEnergyB.DecVal_B);         //  (Float) Phase B Reverse Reactive Energy
	MODBUS.Ireg(PhaC_ReverseReactivEEnergyAdd, RNEnergyC.IntVal_A);         //  (Float) Phase C Reverse Reactive Energy
	MODBUS.Ireg(PhaC_ReverseReactivEEnergyAdd+1, RNEnergyC.DecVal_B);         //  (Float) Phase C Reverse Reactive Energy
	MODBUS.Ireg(Total_ArithmeticSum_ApparentEnergyAdd, SAEnergyT.IntVal_A);         //  (Float) Total (Arithmetic Sum) Apparent Energy
	MODBUS.Ireg(Total_ArithmeticSum_ApparentEnergyAdd+1, SAEnergyT.DecVal_B);         //  (Float) Total (Arithmetic Sum) Apparent Energy
	MODBUS.Ireg(PhaA_ApparentEnergyAdd, SEnergyA.IntVal_A);         //  (Float) Phase A Apparent Energy
	MODBUS.Ireg(PhaA_ApparentEnergyAdd+1, SEnergyA.DecVal_B);         //  (Float) Phase A Apparent Energy
	MODBUS.Ireg(PhaB_ApparentEnergyAdd, SEnergyB.IntVal_A);         //  (Float) Phase B Apparent Energy
	MODBUS.Ireg(PhaB_ApparentEnergyAdd+1, SEnergyB.DecVal_B);         //  (Float) Phase B Apparent Energy
	MODBUS.Ireg(PhaC_ApparentEnergyAdd, SEnergyC.IntVal_A);         //  (Float) Phase C Apparent Energy
	MODBUS.Ireg(PhaC_ApparentEnergyAdd+1, SEnergyC.DecVal_B);         //  (Float) Phase C Apparent Energy
	MODBUS.Ireg(VectorSum_TotalApparentEnergyAdd, SVEnergyT.IntVal_A);         //  (Float) (Vector Sum) Total Apparent Energy
	MODBUS.Ireg(VectorSum_TotalApparentEnergyAdd+1, SVEnergyT.DecVal_B);         //  (Float) (Vector Sum) Total Apparent Energy
	MODBUS.Ireg(VectorSum_TotalApparentPowerAdd, SVMeanT.IntVal_A);         //  (Float) (Vector Sum) Total Apparent Power
	MODBUS.Ireg(VectorSum_TotalApparentPowerAdd + 1, SVMeanT.DecVal_B);         //  (Float) (Vector Sum) Total Apparent Power

	MODBUS.Ireg(MeteringStatus0Add, MeteringStatus0);            //  Metering Status 0
	MODBUS.Ireg(MeteringStatus1Add, MeteringStatus1);            //  Metering Status 1



	//Power and Power Factor Register VARIABLES
	//================================================================================
	MODBUS.Ireg(Total_PhasesumActivePowerAdd, PMeanT.IntVal_A);            //  (Float) Total (all-phase-sum) Active Power
	MODBUS.Ireg(Total_PhasesumActivePowerAdd+1, PMeanT.DecVal_B);            //  (Float) Total (all-phase-sum) Active Power
	MODBUS.Ireg(PhaA_ActivePowerAdd, PMeanA.IntVal_A);                     //  Phase A Active Power
	MODBUS.Ireg(PhaA_ActivePowerAdd+1, PMeanA.DecVal_B);                     //  Phase A Active Power
	MODBUS.Ireg(PhaB_ActivePowerAdd, PMeanB.IntVal_A);                     //  Phase B Active Power
	MODBUS.Ireg(PhaB_ActivePowerAdd+1, PMeanB.DecVal_B);                     //  Phase B Active Power
	MODBUS.Ireg(PhaC_ActivePowerAdd, PMeanC.IntVal_A);                     //  Phase C Active Power
	MODBUS.Ireg(PhaC_ActivePowerAdd+1, PMeanC.DecVal_B);                     //  Phase C Active Power
	MODBUS.Ireg(Total_PhasesumReactivePowerAdd, QMeanT.IntVal_A);          //  Total (all-phase-sum) Reactive Power
	MODBUS.Ireg(Total_PhasesumReactivePowerAdd+1, QMeanT.DecVal_B);          //  Total (all-phase-sum) Reactive Power
	MODBUS.Ireg(PhaA_ReactivePowerAdd, QMeanA.IntVal_A);                   //   Phase A Reactive Power   
	MODBUS.Ireg(PhaA_ReactivePowerAdd+1, QMeanA.DecVal_B);                   //   Phase A Reactive Power 
	MODBUS.Ireg(PhaB_ReactivePowerAdd, QMeanB.IntVal_A);                  //   Phase B Reactive Power
	MODBUS.Ireg(PhaB_ReactivePowerAdd+1, QMeanB.DecVal_B);                  //   Phase B Reactive Power
	MODBUS.Ireg(PhaC_ReactivePowerAdd, QMeanC.IntVal_A);                   //   Phase C Reactive Power
	MODBUS.Ireg(PhaC_ReactivePowerAdd+1, QMeanC.DecVal_B);                   //   Phase C Reactive Power
	MODBUS.Ireg(Total_ArithmeticSumapparentpowerAdd, SAMeanT.IntVal_A);     //  Total (Arithmetic Sum) apparent power
	MODBUS.Ireg(Total_ArithmeticSumapparentpowerAdd+1, SAMeanT.DecVal_B);     //  Total (Arithmetic Sum) apparent power
	MODBUS.Ireg(PhaA_apparentpowerAdd, SMeanA.IntVal_A);                   //  Phase A apparent power
	MODBUS.Ireg(PhaA_apparentpowerAdd+1, SMeanA.DecVal_B);                   //  Phase A apparent power
	MODBUS.Ireg(PhaB_apparentpowerAdd, SMeanB.IntVal_A);                   //  Phase B apparent power
	MODBUS.Ireg(PhaB_apparentpowerAdd+1, SMeanB.DecVal_B);                   //  Phase B apparent power
	MODBUS.Ireg(PhaC_apparentpowerAdd, SMeanC.IntVal_A);                  //  Phase C apparent power
	MODBUS.Ireg(PhaC_apparentpowerAdd+1, SMeanC.DecVal_B);                  //  Phase C apparent power
	MODBUS.Ireg(TotalpowerfactorAdd, PFMeanT.IntVal_A);                     //   Total power factor
	MODBUS.Ireg(TotalpowerfactorAdd+1, PFMeanT.DecVal_B);                     //   Total power factor
	MODBUS.Ireg(PhaA_powerfactorAdd, PFMeanA.IntVal_A);                     //  Phase A power factor
	MODBUS.Ireg(PhaA_powerfactorAdd+1, PFMeanA.DecVal_B);                     //  Phase A power factor
	MODBUS.Ireg(PhaB_powerfactorAdd, PFMeanB.IntVal_A);                     //  Phase B power factor
	MODBUS.Ireg(PhaB_powerfactorAdd+1, PFMeanB.DecVal_B);                     //  Phase B power factor
	MODBUS.Ireg(PhaC_powerfactorAdd, PFMeanC.IntVal_A);                     //  Phase C power factor
	MODBUS.Ireg(PhaC_powerfactorAdd+1, PFMeanC.DecVal_B);                     //  Phase C power factor



	//==============================================================================
	//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
	MODBUS.Ireg(TotalactivefundamentalpowerAdd, PMeanTF.IntVal_A);         //  (Float) Total active fundamental power
	MODBUS.Ireg(TotalactivefundamentalpowerAdd+1, PMeanTF.DecVal_B);         //  (Float) Total active fundamental power
	MODBUS.Ireg(PhaA_activefundamentalpowerAdd, PMeanAF.IntVal_A);         //  (Float)  Phase A active fundamental power
	MODBUS.Ireg(PhaA_activefundamentalpowerAdd+1, PMeanAF.DecVal_B);         //  (Float)  Phase A active fundamental power
	MODBUS.Ireg(PhaB_activefundamentalpowerAdd, PMeanBF.IntVal_A);         //  (Float) Phase B active fundamental power
	MODBUS.Ireg(PhaB_activefundamentalpowerAdd+1, PMeanBF.DecVal_B);         //  (Float) Phase B active fundamental power
	MODBUS.Ireg(PhaC_activefundamentalpowerAdd, PMeanCF.IntVal_A);         //  (Float) Phase C active fundamental power
	MODBUS.Ireg(PhaC_activefundamentalpowerAdd+1, PMeanCF.DecVal_B);         //  (Float) Phase C active fundamental power
	MODBUS.Ireg(TotalactiveharmonicpowerAdd, PMeanTH.IntVal_A);			//  (Float) Total active harmonic power
	MODBUS.Ireg(TotalactiveharmonicpowerAdd+1, PMeanTH.DecVal_B);			//  (Float) Total active harmonic power
	MODBUS.Ireg(PhaA_activeharmonicpowerAdd, PMeanAH.IntVal_A);            //  (Float) phase A active harmonic power
	MODBUS.Ireg(PhaA_activeharmonicpowerAdd+1, PMeanAH.DecVal_B);            //  (Float) phase A active harmonic power
	MODBUS.Ireg(PhaB_activeharmonicpowerAdd, PMeanBH.IntVal_A);            //  (Float) phase B active harmonic power
	MODBUS.Ireg(PhaB_activeharmonicpowerAdd+1, PMeanBH.DecVal_B);            //  (Float) phase B active harmonic power
	MODBUS.Ireg(PhaC_activeharmonicpowerAdd, PMeanCH.IntVal_A);            //  (Float) phase C active harmonic power
	MODBUS.Ireg(PhaC_activeharmonicpowerAdd+1, PMeanCH.DecVal_B);            //  (Float) phase C active harmonic power
	MODBUS.Ireg(N_Line_SampledcurrentRMSAdd, N1Irms.IntVal_A);            //  (Float) N Line Sampled current RMS
	MODBUS.Ireg(N_Line_SampledcurrentRMSAdd+1, N1Irms.DecVal_B);            //  (Float) N Line Sampled current RMS
	MODBUS.Ireg(PhaA_voltageRMSAdd, Urms_A.IntVal_A);                     //  (Float) phase A voltage RMS
	MODBUS.Ireg(PhaA_voltageRMSAdd+1, Urms_A.DecVal_B);                     //  (Float) phase A voltage RMS
	MODBUS.Ireg(PhaB_voltageRMSAdd, Urms_B.IntVal_A);						//  (Float) phase B voltage RMS
	MODBUS.Ireg(PhaB_voltageRMSAdd+1, Urms_B.DecVal_B);						//  (Float) phase B voltage RMS
	MODBUS.Ireg(PhaC_voltageRMSAdd, Urms_C.IntVal_A);						//  (Float) phase C voltage RMS
	MODBUS.Ireg(PhaC_voltageRMSAdd+1, Urms_C.DecVal_B);						//  (Float) phase C voltage RMS
	MODBUS.Ireg(N_Line_calculatedcurrentRMSAdd, N0Irms.IntVal_A);         //  (Float) N Line calculated current RMS
	MODBUS.Ireg(N_Line_calculatedcurrentRMSAdd+1, N0Irms.DecVal_B);         //  (Float) N Line calculated current RMS
	MODBUS.Ireg(PhaA_currentRMSAdd, Irms_A.IntVal_A);						//  (Float) phase A current RMS
	MODBUS.Ireg(PhaA_currentRMSAdd+1, Irms_A.DecVal_B);						//  (Float) phase A current RMS
	MODBUS.Ireg(PhaB_currentRMSAdd, Irms_B.IntVal_A);						//  (Float) phase B current RMS
	MODBUS.Ireg(PhaB_currentRMSAdd+1, Irms_B.DecVal_B);						//  (Float) phase B current RMS
	MODBUS.Ireg(PhaC_currentRMSAdd, Irms_C.IntVal_A);						//  (Float) phase C current RMS
	MODBUS.Ireg(PhaC_currentRMSAdd+1, Irms_C.DecVal_B);						//  (Float) phase C current RMS
	MODBUS.Ireg(PhaA_voltageTHDNAdd, THDNU_A.IntVal_A);					//  (Float) phase A voltage THD+N
	MODBUS.Ireg(PhaA_voltageTHDNAdd+1, THDNU_A.DecVal_B);					//  (Float) phase A voltage THD+N
	MODBUS.Ireg(PhaB_voltageTHDNAdd, THDNU_B.IntVal_A);					//  (Float) phase B voltage THD+N
	MODBUS.Ireg(PhaB_voltageTHDNAdd+1, THDNU_B.DecVal_B);					//  (Float) phase B voltage THD+N
	MODBUS.Ireg(PhaC_voltageTHDNAdd, THDNU_C.IntVal_A);					//  (Float) phase C voltage THD+N
	MODBUS.Ireg(PhaC_voltageTHDNAdd+1, THDNU_C.DecVal_B);					//  (Float) phase C voltage THD+N
	MODBUS.Ireg(PhaA_currentTHDNAdd, THDNI_A.IntVal_A);					//  (Float) phase A current THD+N
	MODBUS.Ireg(PhaA_currentTHDNAdd+1, THDNI_A.DecVal_B);					//  (Float) phase A current THD+N
	MODBUS.Ireg(PhaB_currentTHDNAdd, THDNI_B.IntVal_A);					//  (Float) phase B current THD+N
	MODBUS.Ireg(PhaB_currentTHDNAdd+1, THDNI_B.DecVal_B);					//  (Float) phase B current THD+N
	MODBUS.Ireg(PhaC_currentTHDNAdd, THDNI_C.IntVal_A);					//  (Float) phase C current THD+N
	MODBUS.Ireg(PhaC_currentTHDNAdd+1, THDNI_C.DecVal_B);					//  (Float) phase C current THD+N
	MODBUS.Ireg(FrequencyAdd, FREQ.IntVal_A);							//  (Float) Frequency
	MODBUS.Ireg(FrequencyAdd+1, FREQ.DecVal_B);							//  (Float) Frequency
	MODBUS.Ireg(PhaA_meanphaseangleAdd, PAngle_A.IntVal_A);					//  (Float)  phase A mean phase angle
	MODBUS.Ireg(PhaA_meanphaseangleAdd+1, PAngle_A.DecVal_B);					//  (Float)  phase A mean phase angle
	MODBUS.Ireg(PhaB_meanphaseangleAdd, PAngle_B.IntVal_A);					//  (Float) phase B mean phase angle
	MODBUS.Ireg(PhaB_meanphaseangleAdd+1, PAngle_B.DecVal_B);					//  (Float) phase B mean phase angle
	MODBUS.Ireg(PhaC_meanphaseangleAdd, PAngle_C.IntVal_A);					//  (Float)  phase C mean phase angle
	MODBUS.Ireg(PhaC_meanphaseangleAdd+1, PAngle_C.DecVal_B);					//  (Float)  phase C mean phase angle
	MODBUS.Ireg(MeasuredtemperatureAdd, BoardTemp.IntVal_A);					//  (Float)  Measured temperature
	MODBUS.Ireg(MeasuredtemperatureAdd+1, BoardTemp.DecVal_B);					//  (Float)  Measured temperature
	MODBUS.Ireg(PhaA_voltagephaseangleAdd, Uangle_A.IntVal_A);				//  (Float)  phase A voltage phase angle
	MODBUS.Ireg(PhaA_voltagephaseangleAdd+1, Uangle_A.DecVal_B);				//  (Float)  phase A voltage phase angle
	MODBUS.Ireg(PhaB_voltagephaseangleAdd, Uangle_B.IntVal_A);				//  (Float)  phase B voltage phase angle 
	MODBUS.Ireg(PhaB_voltagephaseangleAdd+1, Uangle_B.DecVal_B);				//  (Float)  phase B voltage phase angle 
	MODBUS.Ireg(PhaC_voltagephaseangleAdd, Uangle_C.IntVal_A);				//  (Float)  phase C voltage phase angle 
	MODBUS.Ireg(PhaC_voltagephaseangleAdd+1, Uangle_C.DecVal_B);				//  (Float)  phase C voltage phase angle 


	 
  
 
	floatconv(Temp_C, &InputReg[TempA], &InputReg[TempB]); 
	floatconv(Temp_F, &InputReg[TempFA], &InputReg[TempFB]); 
	floatconv(Hum, &InputReg[HumA], &InputReg[HumB]); 
 
	MODBUS.Ireg(TempA, InputReg[TempA]); 
	MODBUS.onSetIreg(TempA, SetIreg);
	MODBUS.onGetIreg(TempA, GetIreg);
	MODBUS.Ireg(TempB, InputReg[TempB]); 
	MODBUS.onSetIreg(TempB, SetIreg);
	MODBUS.onGetIreg(TempB, GetIreg);
	MODBUS.Ireg(TempFA, InputReg[TempFA]); 
	MODBUS.onSetIreg(TempFA, SetIreg);
	MODBUS.onGetIreg(TempFA, GetIreg);
	MODBUS.Ireg(TempFB, InputReg[TempFB]); 
	MODBUS.onSetIreg(TempFB, SetIreg);
	MODBUS.onGetIreg(TempFB, GetIreg);
	MODBUS.Ireg(HumA, InputReg[HumA]); 
	MODBUS.onSetIreg(HumA, SetIreg);
	MODBUS.onGetIreg(HumA, GetIreg);
	MODBUS.Ireg(HumB, InputReg[HumB]); 
	MODBUS.onSetIreg(HumB, SetIreg);
	MODBUS.onGetIreg(HumB, GetIreg);

	/*
		MODBUS.Ireg(TemperatureFAdd, Temp);
	MODBUS.onSetIreg(TemperatureFAdd, SetIreg);
	MODBUS.onGetIreg(TemperatureFAdd, GetIreg);

	MODBUS.Ireg(TemperatureFAFAdd, Temp_F);
	MODBUS.onSetIreg(TemperatureFAFAdd, SetIreg);
	MODBUS.onGetIreg(TemperatureFAFAdd, GetIreg);

	MODBUS.Ireg(HumidityAdd, Hum);
	MODBUS.onSetIreg(HumidityAdd, SetIreg);
	MODBUS.onGetIreg(HumidityAdd, GetIreg);
	*/
#if defined(Enable_ETH)
	if (MDBUS_Mode == true) {

		do {
			MODEthernet.Hreg((CalRegistersAdd + (2 * regadd)), CalRegisters[regadd]);

		} while (++regadd < sizeof(CalRegisters));

		regadd = 0;

		do {
			MODEthernet.Hreg((HarCalRegistersAdd + (2 * regadd)), HarCalRegisters[regadd]);

		} while (++regadd < sizeof(HarCalRegisters));

		regadd = 0;

		do {
			MODEthernet.Hreg((MeasurCalRegistersAdd + (2 * regadd)), MeasurCalRegisters[regadd]);
		} while (++regadd < sizeof(MeasurCalRegisters));




		//================================================================================
		// Energy Register VARIABLE
		//================================================================================
		MODEthernet.Ireg(TotalForwardActiveEnergyAdd, APEnergyT.IntVal_A);         //  (Float) Total Forward Active Energy
		MODEthernet.Ireg(TotalForwardActiveEnergyAdd + 1, APEnergyT.DecVal_B);         //  (Float) Total Forward Active Energy
		MODEthernet.Ireg(PhaA_ForwardActiveEnergyAdd, APEnergyA.IntVal_A);	       //  (Float) Phase A Forward Active Energy
		MODEthernet.Ireg(PhaA_ForwardActiveEnergyAdd + 1, APEnergyA.DecVal_B);	       //  (Float) Phase A Forward Active Energy
		MODEthernet.Ireg(PhaB_ForwardActiveEnergyAdd, APEnergyB.IntVal_A);         //  (Float) Phase B Forward Active Energy
		MODEthernet.Ireg(PhaB_ForwardActiveEnergyAdd + 1, APEnergyB.DecVal_B);         //  (Float) Phase B Forward Active Energy
		MODEthernet.Ireg(PhaC_ForwardActiveEnergyAdd, APEnergyC.IntVal_A);         //  (Float) Phase C Forward Active Energy
		MODEthernet.Ireg(PhaC_ForwardActiveEnergyAdd + 1, APEnergyC.DecVal_B);         //  (Float) Phase C Forward Active Energy
		MODEthernet.Ireg(TotalReverseActiveEnergyAdd, ANEnergyT.IntVal_A);         //  (Float) Total Reverse Active Energy
		MODEthernet.Ireg(TotalReverseActiveEnergyAdd + 1, ANEnergyT.DecVal_B);         //  (Float) Total Reverse Active Energy
		MODEthernet.Ireg(PhaA_ReverseActiveEnergyAdd, ANEnergyA.IntVal_A);         //  (Float) Phase A Reverse Active Energy
		MODEthernet.Ireg(PhaA_ReverseActiveEnergyAdd + 1, ANEnergyA.DecVal_B);         //  (Float) Phase A Reverse Active Energy
		MODEthernet.Ireg(PhaB_ReverseActiveEnergyAdd, ANEnergyB.IntVal_A);         //  (Float) Phase B Reverse Active Energy
		MODEthernet.Ireg(PhaB_ReverseActiveEnergyAdd + 1, ANEnergyB.DecVal_B);         //  (Float) Phase B Reverse Active Energy
		MODEthernet.Ireg(PhaC_ReverseActiveEnergyAdd, ANEnergyC.IntVal_A);         //  (Float) Phase C Reverse Active Energy
		MODEthernet.Ireg(PhaC_ReverseActiveEnergyAdd + 1, ANEnergyC.DecVal_B);         //  (Float) Phase C Reverse Active Energy
		MODEthernet.Ireg(TotalForwardReactiveEnergyAdd, RPEnergyT.IntVal_A);         //  (Float) Total Forward Reactive Energy
		MODEthernet.Ireg(TotalForwardReactiveEnergyAdd + 1, RPEnergyT.DecVal_B);         //  (Float) Total Forward Reactive Energy
		MODEthernet.Ireg(PhaA_ForwardReactiveEnergyAdd, RPEnergyA.IntVal_A);         //  (Float) Phase A Forward Reactive Energy
		MODEthernet.Ireg(PhaA_ForwardReactiveEnergyAdd + 1, RPEnergyA.DecVal_B);         //  (Float) Phase A Forward Reactive Energy
		MODEthernet.Ireg(PhaB_ForwardReactiveEnergyAdd, RPEnergyB.IntVal_A);         //  (Float) Phase B Forward Reactive Energy
		MODEthernet.Ireg(PhaB_ForwardReactiveEnergyAdd + 1, RPEnergyB.DecVal_B);         //  (Float) Phase B Forward Reactive Energ
		MODEthernet.Ireg(PhaC_ForwardReactiveEnergyAdd, RPEnergyC.IntVal_A);         //  (Float) Phase C Forward Reactive Energy
		MODEthernet.Ireg(PhaC_ForwardReactiveEnergyAdd + 1, RPEnergyC.DecVal_B);         //  (Float) Phase C Forward Reactive Energy
		MODEthernet.Ireg(TotalReverseReactiveEnergyAdd, RNEnergyT.IntVal_A);         //  (Float) Total Reverse Reactive Energy
		MODEthernet.Ireg(TotalReverseReactiveEnergyAdd + 1, RNEnergyT.DecVal_B);         //  (Float) Total Reverse Reactive Energy
		MODEthernet.Ireg(PhaA_ReverseReactivEEnergyAdd, RNEnergyA.IntVal_A);         //  (Float) Phase A Reverse Reactive Energy
		MODEthernet.Ireg(PhaA_ReverseReactivEEnergyAdd + 1, RNEnergyA.DecVal_B);         //  (Float) Phase A Reverse Reactive Energy
		MODEthernet.Ireg(PhaB_ReverseReactivEEnergyAdd, RNEnergyB.IntVal_A);         //  (Float) Phase B Reverse Reactive Energy
		MODEthernet.Ireg(PhaB_ReverseReactivEEnergyAdd + 1, RNEnergyB.DecVal_B);         //  (Float) Phase B Reverse Reactive Energy
		MODEthernet.Ireg(PhaC_ReverseReactivEEnergyAdd, RNEnergyC.IntVal_A);         //  (Float) Phase C Reverse Reactive Energy
		MODEthernet.Ireg(PhaC_ReverseReactivEEnergyAdd + 1, RNEnergyC.DecVal_B);         //  (Float) Phase C Reverse Reactive Energy
		MODEthernet.Ireg(Total_ArithmeticSum_ApparentEnergyAdd, SAEnergyT.IntVal_A);         //  (Float) Total (Arithmetic Sum) Apparent Energy
		MODEthernet.Ireg(Total_ArithmeticSum_ApparentEnergyAdd + 1, SAEnergyT.DecVal_B);         //  (Float) Total (Arithmetic Sum) Apparent Energy
		MODEthernet.Ireg(PhaA_ApparentEnergyAdd, SEnergyA.IntVal_A);         //  (Float) Phase A Apparent Energy
		MODEthernet.Ireg(PhaA_ApparentEnergyAdd + 1, SEnergyA.DecVal_B);         //  (Float) Phase A Apparent Energy
		MODEthernet.Ireg(PhaB_ApparentEnergyAdd, SEnergyB.IntVal_A);         //  (Float) Phase B Apparent Energy
		MODEthernet.Ireg(PhaB_ApparentEnergyAdd + 1, SEnergyB.DecVal_B);         //  (Float) Phase B Apparent Energy
		MODEthernet.Ireg(PhaC_ApparentEnergyAdd, SEnergyC.IntVal_A);         //  (Float) Phase C Apparent Energy
		MODEthernet.Ireg(PhaC_ApparentEnergyAdd + 1, SEnergyC.DecVal_B);         //  (Float) Phase C Apparent Energy
		MODEthernet.Ireg(VectorSum_TotalApparentEnergyAdd, SVEnergyT.IntVal_A);         //  (Float) (Vector Sum) Total Apparent Energy
		MODEthernet.Ireg(VectorSum_TotalApparentEnergyAdd + 1, SVEnergyT.DecVal_B);         //  (Float) (Vector Sum) Total Apparent Energy
		MODEthernet.Ireg(VectorSum_TotalApparentPowerAdd, SVMeanT.IntVal_A);         //  (Float) (Vector Sum) Total Apparent Power
		MODEthernet.Ireg(VectorSum_TotalApparentPowerAdd + 1, SVMeanT.DecVal_B);         //  (Float) (Vector Sum) Total Apparent Power

		MODEthernet.Ireg(MeteringStatus0Add, MeteringStatus0);            //  Metering Status 0
		MODEthernet.Ireg(MeteringStatus1Add, MeteringStatus1);            //  Metering Status 1



		//Power and Power Factor Register VARIABLES
		//================================================================================
		MODEthernet.Ireg(Total_PhasesumActivePowerAdd, PMeanT.IntVal_A);            //  (Float) Total (all-phase-sum) Active Power
		MODEthernet.Ireg(Total_PhasesumActivePowerAdd + 1, PMeanT.DecVal_B);            //  (Float) Total (all-phase-sum) Active Power
		MODEthernet.Ireg(PhaA_ActivePowerAdd, PMeanA.IntVal_A);                     //  Phase A Active Power
		MODEthernet.Ireg(PhaA_ActivePowerAdd + 1, PMeanA.DecVal_B);                     //  Phase A Active Power
		MODEthernet.Ireg(PhaB_ActivePowerAdd, PMeanB.IntVal_A);                     //  Phase B Active Power
		MODEthernet.Ireg(PhaB_ActivePowerAdd + 1, PMeanB.DecVal_B);                     //  Phase B Active Power
		MODEthernet.Ireg(PhaC_ActivePowerAdd, PMeanC.IntVal_A);                     //  Phase C Active Power
		MODEthernet.Ireg(PhaC_ActivePowerAdd + 1, PMeanC.DecVal_B);                     //  Phase C Active Power
		MODEthernet.Ireg(Total_PhasesumReactivePowerAdd, QMeanT.IntVal_A);          //  Total (all-phase-sum) Reactive Power
		MODEthernet.Ireg(Total_PhasesumReactivePowerAdd + 1, QMeanT.DecVal_B);          //  Total (all-phase-sum) Reactive Power
		MODEthernet.Ireg(PhaA_ReactivePowerAdd, QMeanA.IntVal_A);                   //   Phase A Reactive Power   
		MODEthernet.Ireg(PhaA_ReactivePowerAdd + 1, QMeanA.DecVal_B);                   //   Phase A Reactive Power 
		MODEthernet.Ireg(PhaB_ReactivePowerAdd, QMeanB.IntVal_A);                  //   Phase B Reactive Power
		MODEthernet.Ireg(PhaB_ReactivePowerAdd + 1, QMeanB.DecVal_B);                  //   Phase B Reactive Power
		MODEthernet.Ireg(PhaC_ReactivePowerAdd, QMeanC.IntVal_A);                   //   Phase C Reactive Power
		MODEthernet.Ireg(PhaC_ReactivePowerAdd + 1, QMeanC.DecVal_B);                   //   Phase C Reactive Power
		MODEthernet.Ireg(Total_ArithmeticSumapparentpowerAdd, SAMeanT.IntVal_A);     //  Total (Arithmetic Sum) apparent power
		MODEthernet.Ireg(Total_ArithmeticSumapparentpowerAdd + 1, SAMeanT.DecVal_B);     //  Total (Arithmetic Sum) apparent power
		MODEthernet.Ireg(PhaA_apparentpowerAdd, SMeanA.IntVal_A);                   //  Phase A apparent power
		MODEthernet.Ireg(PhaA_apparentpowerAdd + 1, SMeanA.DecVal_B);                   //  Phase A apparent power
		MODEthernet.Ireg(PhaB_apparentpowerAdd, SMeanB.IntVal_A);                   //  Phase B apparent power
		MODEthernet.Ireg(PhaB_apparentpowerAdd + 1, SMeanB.DecVal_B);                   //  Phase B apparent power
		MODEthernet.Ireg(PhaC_apparentpowerAdd, SMeanC.IntVal_A);                  //  Phase C apparent power
		MODEthernet.Ireg(PhaC_apparentpowerAdd + 1, SMeanC.DecVal_B);                  //  Phase C apparent power
		MODEthernet.Ireg(TotalpowerfactorAdd, PFMeanT.IntVal_A);                     //   Total power factor
		MODEthernet.Ireg(TotalpowerfactorAdd + 1, PFMeanT.DecVal_B);                     //   Total power factor
		MODEthernet.Ireg(PhaA_powerfactorAdd, PFMeanA.IntVal_A);                     //  Phase A power factor
		MODEthernet.Ireg(PhaA_powerfactorAdd + 1, PFMeanA.DecVal_B);                     //  Phase A power factor
		MODEthernet.Ireg(PhaB_powerfactorAdd, PFMeanB.IntVal_A);                     //  Phase B power factor
		MODEthernet.Ireg(PhaB_powerfactorAdd + 1, PFMeanB.DecVal_B);                     //  Phase B power factor
		MODEthernet.Ireg(PhaC_powerfactorAdd, PFMeanC.IntVal_A);                     //  Phase C power factor
		MODEthernet.Ireg(PhaC_powerfactorAdd + 1, PFMeanC.DecVal_B);                     //  Phase C power factor



		//==============================================================================
		//Fundamental/ Harmonic Power and Voltage/ Current RMS Registers VARIABLES
		MODEthernet.Ireg(TotalactivefundamentalpowerAdd, PMeanTF.IntVal_A);         //  (Float) Total active fundamental power
		MODEthernet.Ireg(TotalactivefundamentalpowerAdd + 1, PMeanTF.DecVal_B);         //  (Float) Total active fundamental power
		MODEthernet.Ireg(PhaA_activefundamentalpowerAdd, PMeanAF.IntVal_A);         //  (Float)  Phase A active fundamental power
		MODEthernet.Ireg(PhaA_activefundamentalpowerAdd + 1, PMeanAF.DecVal_B);         //  (Float)  Phase A active fundamental power
		MODEthernet.Ireg(PhaB_activefundamentalpowerAdd, PMeanBF.IntVal_A);         //  (Float) Phase B active fundamental power
		MODEthernet.Ireg(PhaB_activefundamentalpowerAdd + 1, PMeanBF.DecVal_B);         //  (Float) Phase B active fundamental power
		MODEthernet.Ireg(PhaC_activefundamentalpowerAdd, PMeanCF.IntVal_A);         //  (Float) Phase C active fundamental power
		MODEthernet.Ireg(PhaC_activefundamentalpowerAdd + 1, PMeanCF.DecVal_B);         //  (Float) Phase C active fundamental power
		MODEthernet.Ireg(TotalactiveharmonicpowerAdd, PMeanTH.IntVal_A);			//  (Float) Total active harmonic power
		MODEthernet.Ireg(TotalactiveharmonicpowerAdd + 1, PMeanTH.DecVal_B);			//  (Float) Total active harmonic power
		MODEthernet.Ireg(PhaA_activeharmonicpowerAdd, PMeanAH.IntVal_A);            //  (Float) phase A active harmonic power
		MODEthernet.Ireg(PhaA_activeharmonicpowerAdd + 1, PMeanAH.DecVal_B);            //  (Float) phase A active harmonic power
		MODEthernet.Ireg(PhaB_activeharmonicpowerAdd, PMeanBH.IntVal_A);            //  (Float) phase B active harmonic power
		MODEthernet.Ireg(PhaB_activeharmonicpowerAdd + 1, PMeanBH.DecVal_B);            //  (Float) phase B active harmonic power
		MODEthernet.Ireg(PhaC_activeharmonicpowerAdd, PMeanCH.IntVal_A);            //  (Float) phase C active harmonic power
		MODEthernet.Ireg(PhaC_activeharmonicpowerAdd + 1, PMeanCH.DecVal_B);            //  (Float) phase C active harmonic power
		MODEthernet.Ireg(N_Line_SampledcurrentRMSAdd, N1Irms.IntVal_A);            //  (Float) N Line Sampled current RMS
		MODEthernet.Ireg(N_Line_SampledcurrentRMSAdd + 1, N1Irms.DecVal_B);            //  (Float) N Line Sampled current RMS
		MODEthernet.Ireg(PhaA_voltageRMSAdd, Urms_A.IntVal_A);                     //  (Float) phase A voltage RMS
		MODEthernet.Ireg(PhaA_voltageRMSAdd + 1, Urms_A.DecVal_B);                     //  (Float) phase A voltage RMS
		MODEthernet.Ireg(PhaB_voltageRMSAdd, Urms_B.IntVal_A);						//  (Float) phase B voltage RMS
		MODEthernet.Ireg(PhaB_voltageRMSAdd + 1, Urms_B.DecVal_B);						//  (Float) phase B voltage RMS
		MODEthernet.Ireg(PhaC_voltageRMSAdd, Urms_C.IntVal_A);						//  (Float) phase C voltage RMS
		MODEthernet.Ireg(PhaC_voltageRMSAdd + 1, Urms_C.DecVal_B);						//  (Float) phase C voltage RMS
		MODEthernet.Ireg(N_Line_calculatedcurrentRMSAdd, N0Irms.IntVal_A);         //  (Float) N Line calculated current RMS
		MODEthernet.Ireg(N_Line_calculatedcurrentRMSAdd + 1, N0Irms.DecVal_B);         //  (Float) N Line calculated current RMS
		MODEthernet.Ireg(PhaA_currentRMSAdd, Irms_A.IntVal_A);						//  (Float) phase A current RMS
		MODEthernet.Ireg(PhaA_currentRMSAdd + 1, Irms_A.DecVal_B);						//  (Float) phase A current RMS
		MODEthernet.Ireg(PhaB_currentRMSAdd, Irms_B.IntVal_A);						//  (Float) phase B current RMS
		MODEthernet.Ireg(PhaB_currentRMSAdd + 1, Irms_B.DecVal_B);						//  (Float) phase B current RMS
		MODEthernet.Ireg(PhaC_currentRMSAdd, Irms_C.IntVal_A);						//  (Float) phase C current RMS
		MODEthernet.Ireg(PhaC_currentRMSAdd + 1, Irms_C.DecVal_B);						//  (Float) phase C current RMS
		MODEthernet.Ireg(PhaA_voltageTHDNAdd, THDNU_A.IntVal_A);					//  (Float) phase A voltage THD+N
		MODEthernet.Ireg(PhaA_voltageTHDNAdd + 1, THDNU_A.DecVal_B);					//  (Float) phase A voltage THD+N
		MODEthernet.Ireg(PhaB_voltageTHDNAdd, THDNU_B.IntVal_A);					//  (Float) phase B voltage THD+N
		MODEthernet.Ireg(PhaB_voltageTHDNAdd + 1, THDNU_B.DecVal_B);					//  (Float) phase B voltage THD+N
		MODEthernet.Ireg(PhaC_voltageTHDNAdd, THDNU_C.IntVal_A);					//  (Float) phase C voltage THD+N
		MODEthernet.Ireg(PhaC_voltageTHDNAdd + 1, THDNU_C.DecVal_B);					//  (Float) phase C voltage THD+N
		MODEthernet.Ireg(PhaA_currentTHDNAdd, THDNI_A.IntVal_A);					//  (Float) phase A current THD+N
		MODEthernet.Ireg(PhaA_currentTHDNAdd + 1, THDNI_A.DecVal_B);					//  (Float) phase A current THD+N
		MODEthernet.Ireg(PhaB_currentTHDNAdd, THDNI_B.IntVal_A);					//  (Float) phase B current THD+N
		MODEthernet.Ireg(PhaB_currentTHDNAdd + 1, THDNI_B.DecVal_B);					//  (Float) phase B current THD+N
		MODEthernet.Ireg(PhaC_currentTHDNAdd, THDNI_C.IntVal_A);					//  (Float) phase C current THD+N
		MODEthernet.Ireg(PhaC_currentTHDNAdd + 1, THDNI_C.DecVal_B);					//  (Float) phase C current THD+N
		MODEthernet.Ireg(FrequencyAdd, FREQ.IntVal_A);							//  (Float) Frequency
		MODEthernet.Ireg(FrequencyAdd + 1, FREQ.DecVal_B);							//  (Float) Frequency
		MODEthernet.Ireg(PhaA_meanphaseangleAdd, PAngle_A.IntVal_A);					//  (Float)  phase A mean phase angle
		MODEthernet.Ireg(PhaA_meanphaseangleAdd + 1, PAngle_A.DecVal_B);					//  (Float)  phase A mean phase angle
		MODEthernet.Ireg(PhaB_meanphaseangleAdd, PAngle_B.IntVal_A);					//  (Float) phase B mean phase angle
		MODEthernet.Ireg(PhaB_meanphaseangleAdd + 1, PAngle_B.DecVal_B);					//  (Float) phase B mean phase angle
		MODEthernet.Ireg(PhaC_meanphaseangleAdd, PAngle_C.IntVal_A);					//  (Float)  phase C mean phase angle
		MODEthernet.Ireg(PhaC_meanphaseangleAdd + 1, PAngle_C.DecVal_B);					//  (Float)  phase C mean phase angle
		MODEthernet.Ireg(MeasuredtemperatureAdd, BoardTemp.IntVal_A);					//  (Float)  Measured temperature
		MODEthernet.Ireg(MeasuredtemperatureAdd + 1, BoardTemp.DecVal_B);					//  (Float)  Measured temperature
		MODEthernet.Ireg(PhaA_voltagephaseangleAdd, Uangle_A.IntVal_A);				//  (Float)  phase A voltage phase angle
		MODEthernet.Ireg(PhaA_voltagephaseangleAdd + 1, Uangle_A.DecVal_B);				//  (Float)  phase A voltage phase angle
		MODEthernet.Ireg(PhaB_voltagephaseangleAdd, Uangle_B.IntVal_A);				//  (Float)  phase B voltage phase angle 
		MODEthernet.Ireg(PhaB_voltagephaseangleAdd + 1, Uangle_B.DecVal_B);				//  (Float)  phase B voltage phase angle 
		MODEthernet.Ireg(PhaC_voltagephaseangleAdd, Uangle_C.IntVal_A);				//  (Float)  phase C voltage phase angle 
		MODEthernet.Ireg(PhaC_voltagephaseangleAdd + 1, Uangle_C.DecVal_B);				//  (Float)  phase C voltage phase angle 




		floatconv(Temp_C, &InputReg[TempA], &InputReg[TempB]);
		floatconv(Temp_F, &InputReg[TempFA], &InputReg[TempFB]);
		floatconv(Hum, &InputReg[HumA], &InputReg[HumB]);

		MODEthernet.Ireg(TempA, InputReg[TempA]);
		//MODEthernet.onSetIreg(TempA, SetIreg);
		//MODEthernet.onGetIreg(TempA, GetIreg);
		MODEthernet.Ireg(TempB, InputReg[TempB]);
		//MODEthernet.onSetIreg(TempB, SetIreg);
		//MODEthernet.onGetIreg(TempB, GetIreg);
		MODEthernet.Ireg(TempFA, InputReg[TempFA]);
		//MODEthernet.onSetIreg(TempFA, SetIreg);
		//MODEthernet.onGetIreg(TempFA, GetIreg);
		MODEthernet.Ireg(TempFB, InputReg[TempFB]);
		//MODEthernet.onSetIreg(TempFB, SetIreg);
		//MODEthernet.onGetIreg(TempFB, GetIreg);
		MODEthernet.Ireg(HumA, InputReg[HumA]);
		//MODEthernet.onSetIreg(HumA, SetIreg);
		//MODEthernet.onGetIreg(HumA, GetIreg);
		MODEthernet.Ireg(HumB, InputReg[HumB]);
		//MODEthernet.onSetIreg(HumB, SetIreg);
		//MODEthernet.onGetIreg(HumB, GetIreg);





	}
#endif
		 
} 
 
 
 
void floatconv(float input, uint16_t* ha, uint16_t* hb) { 
	long lconv = lround(input * scale); 
	memcpy((uint8_t*)ha, (uint8_t*)&lconv, 2); 
	memcpy((uint8_t*)hb, (uint8_t*)&lconv + 2, 2); 
 
} 
 
 
void Modbus_Poling(void) { 
 
	MODBUS.task();
#if defined(Enable_ETH)
	MODEthernet.task();
#endif
	InitModbus();
	led_ModbusFlash(50, 10);
	//yield(); 
	 
 
} 
 
 
 
uint16_t SetHoldreg(TRegister* reg, uint16_t val) {


	return val;
}


//Holding register 0 อ่านฟังก์ชันเรียกกลับเหตุการณ์ข้อมูล Address 0
uint16_t GetHoldreg(TRegister* reg, uint16_t val) {

	return val;
}


float SetIreg(TRegister* reg, float val) {


	return val;
}


//Holding register 0 อ่านฟังก์ชันเรียกกลับเหตุการณ์ข้อมูล Address 0
float GetIreg(TRegister* reg, float val) {

	return val;
}
 
 
 

 
 
 
