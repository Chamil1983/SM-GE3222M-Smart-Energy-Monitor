//extern void DisplayRegisters(boolean DisplayFull);


//======================================================================
void ReadDataIdle(void) { }
//======================================================================

//Initialize Energy IC
void DeviceInit() {

    // LineFreq = lineFreq, PGAGain = pgagain, VoltageGainX = ugain, CurrentGainCT1 = igainA, CurrentGainCT2 = igainB, CurrentGainCT3 = igainC, CurrentGainCTN = igainN
#if ATM90DEVICE == ATM90E32_DEVICE && ATM_SPLITPHASE == true // Not Fully Tested.
// the 2nd (B) current channel is not used with the split phase meter
#if ATM_SINGLEVOLTAGE == true
    eic.begin(LineFreq, PGAGain, VoltageGain1, 0, 0, CurrentGainCT1, 0, CurrentGainCT2);
#else
    eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain2, VoltageGain3, CurrentGainCT1, 0, CurrentGainCT2);
#endif
#endif

#if ATM90DEVICE == ATM90E32_DEVICE && ATM_SPLITPHASE == false
#if ATM_SINGLEVOLTAGE == true
    // eic.begin(LineFreq, PGAGain, VoltageGain1, 0, 0, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3);
    eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain1, VoltageGain1, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3);
#else
    eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain2, VoltageGain3, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3);
#endif
#endif

#if ATM90DEVICE == ATM90E36_DEVICE && ATM_SPLITPHASE == false
#if ATM_SINGLEVOLTAGE == true
    // eic.begin(LineFreq, PGAGain, VoltageGain1, 0, 0, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN);
    eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain1, VoltageGain1, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN);
#else

    eic.InitEnergy();

    //eic.begin(LineFreq, PGAGain, VoltageGain1, VoltageGain1, VoltageGain1, CurrentGainCT1, CurrentGainCT2, CurrentGainCT3, CurrentGainCTN);
#endif
#endif

    DBUGS.println("SM-GE32222M Hardware Setup and Power-Up Test - Board Initialized");

    //delay(1000);
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("System Initialized");
    //DBUGS.println("");

    


} // End DeviceInit

//=====================================================================

// Read Energy Data and Display Registers
void DisplayPower_Data(boolean DisplayFull= true)
{
    

    // Display Board Configuration
    if (EnableDisplayBoardConfiguration == true && DisplayFull == true)
        DisplayBoardConfiguration();

    // **************** VOLTAGE ****************

    // Get Mains Voltages
    LineVoltage1 = NoiseFilterSquelch(eic.GetLineVoltage1(), 1);
    LineVoltage2 = NoiseFilterSquelch(eic.GetLineVoltage2(), 1);
    LineVoltage3 = NoiseFilterSquelch(eic.GetLineVoltage3(), 1);
    LineVoltageAverage = 0;
    LineVoltageTotal = 0;

    // Calculate Voltage
#if ATM_SINGLEVOLTAGE == true && ATM_SPLITPHASE == true
    DBUGS.println("* SOFTWARE CONFIGURATION ERROR *\n* You cannot have ATM_SINGLEVOLTAGE and ATM_SPLITPHASE, both Enabled.\n");
#else
#if ATM_SPLITPHASE == true
    LineVoltageTotal = (LineVoltage1 + LineVoltage3) / 2; // Split Single Phase configured, so only 120V per leg
#else
    LineVoltageAverage = (LineVoltage1 + LineVoltage2 + LineVoltage3) / 3; // Voltage should be around 110V, or 220-240, at the AC transformer primary.
#endif
#endif

    if (DisplayFull == true) // Display Expanded Information
    {

        // Mains RMS Voltage
        PrintUnderline("Mains RMS Voltage");

#if ATM_SINGLEVOLTAGE == false

        DBUGS.println("Line Voltage 1\t\t: \t\t" + String(LineVoltage1) + " V");

#if ATM_SPLITPHASE == false
        DBUGS.println("Line Voltage 2\t\t: \t\t" + String(LineVoltage2) + " V");
#endif

        DBUGS.println("Line Voltage 3\t\t: \t\t" + String(LineVoltage3) + " V");

#if ATM_SPLITPHASE == true
        PrintSeparator("Total Split Voltage: " + String(LineVoltageTotal) + " V (V1~X~V3)");
#else
        PrintSeparator("Average Voltage\t\t: \t\t" + String(LineVoltageAverage) + " V (V1~V2~V3)");
#endif

#else
        PrintSeparator("Voltage 1: " + String(LineVoltage1) + " V (Single Line Voltage Selected)");
#endif
    }


    // **************** CURRENT ****************

    // Get Total Current.  Ignore CT2 if Split Phase
    LineCurrentCT1 = NoiseFilterSquelch(eic.GetLineCurrentCT1());
#if ATM_SPLITPHASE == false
    LineCurrentCT2 = NoiseFilterSquelch(eic.GetLineCurrentCT2());
#else
    LineCurrentCT2 = 0;
#endif
    LineCurrentCT3 = NoiseFilterSquelch(eic.GetLineCurrentCT3());

    // Using ESP32 ADC for CT4 Current Input

#if ATM90DEVICE == ATM90E32_DEVICE || CT4_CONFIG == CT4_ESP
    ReadCT4Current();
#if CT4_ENABLED == true
    LineCurrentTotal = NoiseFilterSquelch(LineCurrentCT1 + LineCurrentCT2 + LineCurrentCT3 + LineCurrentCT4);
#else
    LineCurrentTotal = NoiseFilterSquelch(LineCurrentCT1 + LineCurrentCT2 + LineCurrentCT3);
#endif
#endif

    // E36 Using I4N Input for Current
#if ATM90DEVICE == ATM90E36_DEVICE && CT4_CONFIG == CT4_ATM
    LineCurrentCTN = NoiseFilterSquelch(eic.GetLineCurrentCTN());
    LineCurrentTotal = NoiseFilterSquelch(LineCurrentCT1 + LineCurrentCT2 + LineCurrentCT3);
#endif

    if (DisplayFull == true) // Display Expanded Information
    {
        DBUGS.println("\n\n");

        // Mains RMS Current
        PrintUnderline("Mains RMS Current");
        DBUGS.println("Current CT1\t\t: \t\t" + String(LineCurrentCT1) + " A");

#if ATM_SPLITPHASE == false
        DBUGS.println("Current CT2\t\t: \t\t" + String(LineCurrentCT2) + " A");
#endif

        DBUGS.println("Current CT3\t\t: \t\t" + String(LineCurrentCT3) + " A");

#if ATM90DEVICE == ATM90E32_DEVICE && CT4_ENABLED == true || CT4_CONFIG == CT4_ESP && CT4_ENABLED == true
        DBUGS.println("Current CT4\t\t: \t\t" + String(LineCurrentCT4) + " A (ESP ADC1 CH7)");
#endif

#if ATM90DEVICE == ATM90E36_DEVICE && CT4_CONFIG == CT4_ATM
        DBUGS.println("Current CTN\t\t: \t\t" + String(LineCurrentCTN) + " A");
#endif

#if ATM_SPLITPHASE == true
        DBUGS.println("Actual Total Current: " + String(LineCurrentTotal) + " A (CT1~X~CT3)");
#else
#if CT4_ENABLED == true
        PrintSeparator("Actual Total Current\t\t: \t\t" + String(LineCurrentTotal) + " A (CT1~CT2~CT3~CT4)");
#else
        PrintSeparator("Actual Total Current\t\t: \t\t" + String(LineCurrentTotal) + " A (CT1~CT2~CT3)");
#endif
#endif

        DBUGS.println("\n\n");

 
    }

    // **************** POWER ****************

    // Calculated Power CT2ased on V*I
#if ATM_SINGLEVOLTAGE == true // Single Mains Voltage for all Current Measurements
    CalculatedPowerCT1 = LineVoltage1 * LineCurrentCT1;
    CalculatedPowerCT2 = LineVoltage1 * LineCurrentCT2;
    CalculatedPowerCT3 = LineVoltage1 * LineCurrentCT3;

#if ATM90DEVICE == ATM90E36_DEVICE
    CalculatedPowerCT4 = LineVoltage1 * LineCurrentCT4;
#endif

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Calculated RMS Power");
        DBUGS.println("Power V1*I1: " + String(CalculatedPowerCT1) + " W");
        DBUGS.println("Power V1*I2: " + String(CalculatedPowerCT2) + " W");
        DBUGS.println("Power V1*I3: " + String(CalculatedPowerCT3) + " W");

#if ATM90DEVICE == ATM90E36_DEVICE
        DBUGS.println("Power V1*I4: " + String(CalculatedPowerCT4) + " W");
#endif
    }
#else // Seperate Mains Voltages for Current Measurements
    CalculatedPowerCT1 = LineVoltage1 * LineCurrentCT1;
    CalculatedPowerCT2 = LineVoltage2 * LineCurrentCT2;
    CalculatedPowerCT3 = LineVoltage3 * LineCurrentCT3;

#if ATM90DEVICE == ATM90E36_DEVICE
    CalculatedPowerCT4 = LineVoltage1 * LineCurrentCTN; 
#endif

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Calculated RMS Power");
        DBUGS.println("Power V1*I1\t\t: \t\t " + String(CalculatedPowerCT1) + " W");
        DBUGS.println("Power V2*I2\t\t: \t\t " + String(CalculatedPowerCT2) + " W");
        DBUGS.println("Power V3*I3\t\t: \t\t " + String(CalculatedPowerCT3) + " W");
    }
#if CT4_ENABLED == true && ATM90DEVICE == ATM90E36_DEVICE
    if (DisplayFull == true) // Display Expanded Information
    {
        DBUGS.println("Power V1*I4: " + String(CalculatedPowerCT4) + " W");
    }
#endif
#endif

    // Calculate Total Watts
#if ATM_SPLITPHASE == true // USA 120+120
    CalculatedTotalPower = (LineVoltage1 * LineCurrentCT1) + (LineVoltage3 * LineCurrentCT3);

    if (DisplayFull == true) // Display Expanded Information
        PrintSeparator("Calculated Total Power: " + String(CalculatedTotalPower) + " W (CT1~X~CT3)");
#else // World
#if CT4_CONFIG == CT4_ESP && CT4_ENABLED == true
    CalculatedTotalPower = (LineVoltage1 * LineCurrentCT1) + (LineVoltage2 * LineCurrentCT2) + (LineVoltage3 * LineCurrentCT3) + (LineVoltage1 * LineCurrentCT4);

    if (DisplayFull == true) // Display Expanded Information
        PrintSeparator("Calculated Total Power: " + String(CalculatedTotalPower) + " W (CT1~CT2~CT3~CT4)");
#else
    CalculatedTotalPower = (LineVoltage1 * LineCurrentCT1) + (LineVoltage2 * LineCurrentCT2) + (LineVoltage3 * LineCurrentCT3);

    if (DisplayFull == true) // Display Expanded Information

        PrintSeparator("Calculated Total Power\t: \t\t" + String(CalculatedTotalPower) + " W (CT1~CT2~CT3)");

  

#endif
#endif

    if (DisplayFull == true) // Display Expanded Information
        DBUGS.println("");

    // Active Power.  Extra Import/Export values included to provide added detail for Home Automation logging
    

    if (DisplayFull == true) // Display Expanded Information
        PrintUnderline("Active Power (Absorbed or Used by the Load)");

    // Active Power CT1 - Always a Postive Number
    ActivePowerCT1 = NoiseFilterSquelch(eic.GetActivePowerCT1(), 0.2);
    ActivePowerImportCT1 = 0;
    ActivePowerExportCT1 = 0;
    ActiveFlag = "";

    if (ActivePowerCT1 > 0)
    {
        ActivePowerImportCT1 = ActivePowerCT1;
        ActiveFlag = "(Import)";
    }

    if (ActivePowerCT1 < 0)
    {
        ActivePowerExportCT1 = -ActivePowerCT1;
        ActiveFlag = "(Export)";
    }

    if (DisplayFull == true) // Display Expanded Information
        DBUGS.println("Active Power CT1\t\t: \t\t " + String(ActivePowerCT1) + " W \t" + ActiveFlag);



    // Active Power CT2 - Always a Postive Number
    ActivePowerCT2 = NoiseFilterSquelch(eic.GetActivePowerCT2(), 0.2);
    ActivePowerImportCT2 = 0;
    ActivePowerExportCT2 = 0;
    ActiveFlag = "";

    if (ActivePowerCT2 > 0)
    {
        ActivePowerImportCT2 = ActivePowerCT2;
        ActiveFlag = "(Import)";
    }

    if (ActivePowerCT2 < 0)
    {
        ActivePowerExportCT2 = -ActivePowerCT2;
        ActiveFlag = "(Export)";
    }
    if (DisplayFull == true) // Display Expanded Information
        DBUGS.println("Active Power CT2\t\t: \t\t " + String(ActivePowerCT2) + " W \t" + ActiveFlag);


    // Active Power CT3 - Always a Postive Number
    ActivePowerCT3 = NoiseFilterSquelch(eic.GetActivePowerCT3(), 0.2);
    ActivePowerImportCT3 = 0;
    ActivePowerExportCT3 = 0;
    ActiveFlag = "";

    if (ActivePowerCT3 > 0)
    {
        ActivePowerImportCT3 = ActivePowerCT3;
        ActiveFlag = "(Import)";
    }

    if (ActivePowerCT3 < 0)
    {
        ActivePowerExportCT3 = -ActivePowerCT3;
        ActiveFlag = "(Export)";
    }

    if (DisplayFull == true) // Display Expanded Information
        DBUGS.println("Active Power CT3\t\t: \t\t " + String(ActivePowerCT3) + " W \t" + ActiveFlag);



    // Total Active Power
    TotalActivePower = NoiseFilterSquelch(eic.GetTotalActivePower(), 0.2);
    ActiveFlag = "";

    if (TotalActivePower > 0)
    {
        TotalActivePowerImport = TotalActivePower;
        ActiveFlag = "(Import)";
    }

    if (TotalActivePower < 0)
    {
        TotalActivePowerExport = -TotalActivePower;
        ActiveFlag = "(Export)";
    }

    CalculatedTotalActivePower = (ActivePowerCT1 + ActivePowerCT2 + ActivePowerCT3);

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintSeparator("Total Active Power\t\t: \t\t " + String(TotalActivePower) + " W \t" + ActiveFlag);
        PrintSeparator("Calculated Total Active Power\t: \t\t " + String(CalculatedTotalActivePower) + " W (CT1~CT2~CT3)");


        DBUGS.println("\n\n");
    }

    // Re-Active Power
    ReactivePowerCT1 = NoiseFilterSquelch(eic.GetReactivePowerCT1());
    ReactivePowerCT2 = NoiseFilterSquelch(eic.GetReactivePowerCT2());
    ReactivePowerCT3 = NoiseFilterSquelch(eic.GetReactivePowerCT3());
    TotalReactivePower = NoiseFilterSquelch(eic.GetTotalReactivePower());
    CalculatedTotalReactivePower = (ReactivePowerCT1 + ReactivePowerCT2 + ReactivePowerCT3);

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Re-Active Power");
        DBUGS.println("ReActive Power CT1\t\t\t: \t\t" + String(ReactivePowerCT1) + " VAR");
        DBUGS.println("ReActive Power CT2\t\t\t: \t\t" + String(ReactivePowerCT2) + " VAR");
        DBUGS.println("ReActive Power CT3\t\t\t: \t\t" + String(ReactivePowerCT3) + " VAR");
        PrintSeparator("Total ReActive Power\t\t: \t\t" + String(TotalReactivePower) + " VAR (CT1~CT2~CT3)");

        PrintSeparator("Calculated Total Reactive Power\t: \t\t" + String(CalculatedTotalReactivePower) + " W (CT1~CT2~CT3)");
        DBUGS.println("\n\n");


    }

    // Apparent Power Flowing - Voltage and Current (V * I)
    ApparentPowerCT1 = NoiseFilterSquelch(eic.GetApparentPowerCT1());
    ApparentPowerCT2 = NoiseFilterSquelch(eic.GetApparentPowerCT2());
    ApparentPowerCT3 = NoiseFilterSquelch(eic.GetApparentPowerCT3());
    TotalApparentPower = NoiseFilterSquelch(eic.GetTotalApparentPower());
    CalculatedTotalApparentPower = (ApparentPowerCT1 + ApparentPowerCT2 + ApparentPowerCT3);

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Apparent Power (Total Amount of Power Flowing from Source to Load)");
        DBUGS.println("Apparent Power CT1\t\t\t: \t\t" + String(ApparentPowerCT1) + " VA");
        DBUGS.println("Apparent Power CT2\t\t\t: \t\t" + String(ApparentPowerCT2) + " VA");
        DBUGS.println("Apparent Power CT3\t\t\t: \t\t" + String(ApparentPowerCT3) + " VA");
        PrintSeparator("Total Apparent Power\t\t: \t\t" + String(TotalApparentPower) + " VA (CT1~CT2~CT3)");

        PrintSeparator("Calculated Total Apparent Power\t: \t\t" + String(CalculatedTotalApparentPower) + " W (CT1~CT2~CT3)");
        DBUGS.println("\n\n");


    }

#if ATM90DEVICE == ATM90E32_DEVICE
    if (DisplayFull == true) // Display Expanded Information
        PrintUnderline("Other Power Information");

    // Total Fundamental Power
    TotalActiveFundPower = NoiseFilterSquelch(eic.GetTotalActiveFundPower());
    if (DisplayFull == true) // Display Expanded Information
        DBUGS.println("Total Fundamental Power: " + String(TotalActiveFundPower) + " pH(t)\n");

    // Total Harmonic Power
    TotalActiveHarPower = NoiseFilterSquelch(eic.GetTotalActiveHarPower());
    if (DisplayFull == true) // Display Expanded Information
        DBUGS.println("Total Harmonic Power: " + String(TotalActiveHarPower) + " ");

#endif

    if (DisplayFull == true)
        DBUGS.println("");

    // **************** OTHER ****************

    // Power Factor
    PowerFactorCT1 = NoiseFilterSquelch(eic.GetPowerFactorCT1(), 0.01, false);
    PowerFactorCT2 = NoiseFilterSquelch(eic.GetPowerFactorCT2(), 0.01, false);
    PowerFactorCT3 = NoiseFilterSquelch(eic.GetPowerFactorCT3(), 0.01, false);
    TotalPowerFactor = NoiseFilterSquelch(eic.GetTotalPowerFactor(), 0, false);

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Power Factor");
        DBUGS.println("Power Factor CT1\t\t: \t\t" + String(PowerFactorCT1) + " PF");
        DBUGS.println("Power Factor CT2\t\t: \t\t" + String(PowerFactorCT2) + " PF");
        DBUGS.println("Power Factor CT3\t\t: \t\t" + String(PowerFactorCT3) + " PF");
        PrintSeparator("Total Power Factor\t\t: \t\t" + String(TotalPowerFactor) + " PF");
        DBUGS.println("\n\n");



    }

    // Phase Angle
    PhaseAngleCT1 = NoiseFilterSquelch(eic.GetPhaseCT1(), 0, true, 180);
    PhaseAngleCT2 = NoiseFilterSquelch(eic.GetPhaseCT2(), 0, true, 180);
    PhaseAngleCT3 = NoiseFilterSquelch(eic.GetPhaseCT3(), 0, true, 180);

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Phase Angle");
        DBUGS.println("Phase Angle CT1\t\t: \t\t" + String(PhaseAngleCT1));
        DBUGS.println("Phase Angle CT2\t\t: \t\t" + String(PhaseAngleCT2));
        DBUGS.println("Phase Angle CT3\t\t: \t\t" + String(PhaseAngleCT3));
        DBUGS.println("");


    }

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Import(Forward) Active Energy");
        DBUGS.println("Import Active Energy CT1\t\t: \t\t" + String(PhaA_ForwardActiveEnergy) + " kWh");
        DBUGS.println("Import Active Energy CT2\t\t: \t\t" + String(PhaB_ForwardActiveEnergy) + " kWh");
        DBUGS.println("Import Active Energy CT3\t\t: \t\t" + String(PhaC_ForwardActiveEnergy) + " kWh");
        PrintSeparator("Total Import Active Energy\t\t: \t\t" + String(TotalForwardActiveEnergy) + " kWh");
        DBUGS.println("\n\n");
    }


    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Export(Reverse) Active Energy");
        DBUGS.println("Export Active Energy CT1\t\t: \t\t" + String(PhaA_ReverseActiveEnergy) + " kWh");
        DBUGS.println("Export Active Energy CT2\t\t: \t\t" + String(PhaB_ReverseActiveEnergy) + " kWh");
        DBUGS.println("Export Active Energy CT3\t\t: \t\t" + String(PhaC_ReverseActiveEnergy) + " kWh");
        PrintSeparator("Total Export Active Energy\t\t: \t\t" + String(TotalReverseActiveEnergy) + " kWh");
        DBUGS.println("\n\n");
    }


    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Import(Forward) Reactive Energy");
        DBUGS.println("Import Reactive Energy CT1\t\t: \t\t" + String(PhaA_ForwardReactiveEnergy) + " kVarh");
        DBUGS.println("Import Reactive Energy CT2\t\t: \t\t" + String(PhaB_ForwardReactiveEnergy) + " kVarh");
        DBUGS.println("Import Reactive Energy CT3\t\t: \t\t" + String(PhaC_ForwardReactiveEnergy) + " kVarh");
        PrintSeparator("Total Import Reactive Energy\t\t: \t\t" + String(TotalForwardReactiveEnergy) + " kVarh");
        DBUGS.println("\n\n");
    }

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Export(Reverse) Reactive Energy");
        DBUGS.println("Export Reactive Energy CT1\t\t: \t\t" + String(PhaA_ReverseReactiveEnergy) + " kVarh");
        DBUGS.println("Export Reactive Energy CT2\t\t: \t\t" + String(PhaB_ReverseReactiveEnergy) + " kVarh");
        DBUGS.println("Export Reactive Energy CT3\t\t: \t\t" + String(PhaC_ReverseReactiveEnergy) + " kVarh");
        PrintSeparator("Total Export Reactive Energy\t\t: \t\t" + String(TotalReverseReactiveEnergy) + " kVarh");
        DBUGS.println("\n\n");
    }

    if (DisplayFull == true) // Display Expanded Information
    {
        PrintUnderline("Apparent Energy");
        DBUGS.println("Apparent Energy CT1\t\t: \t\t" + String(PhaA_ApparentEnergy) + " kVAh");
        DBUGS.println("Apparent Energy CT2\t\t: \t\t" + String(PhaB_ApparentEnergy) + " kVAh");
        DBUGS.println("Apparent Energy CT3\t\t: \t\t" + String(PhaC_ApparentEnergy) + " kVAh");
        PrintSeparator("Total Apparent Energy (Arith. Sum) \t: \t\t" + String(Total_ArithmeticSum_ApparentEnergy) + " kVAh");
        DBUGS.println("Total (Vector Sum) Apparent Energy\t: \t\t" + String(VectorSum_TotalApparentEnergy) + " kVAh");
        DBUGS.println("\n\n");
    }


    if (DisplayFull == true) // Display Expanded Information
        PrintUnderline("Other Information");

    // Chip Temperature
    ChipTemperature = NoiseFilterSquelch(eic.GetTemperature());

    if (DisplayFull == true) // Display Expanded Information
        DBUGS.println("Chip Temperature\t\t: \t\t" + String(ChipTemperature) + " °C");

    // Line Frequency
    LineFrequency = NoiseFilterSquelch(eic.GetFrequency());

    if (DisplayFull == true) // Display Expanded Information
        DBUGS.println("Mains Frequency\t\t: \t\t" + String(LineFrequency) + " Hz\n");



    // Read PCB NTC Temperature
    if (DisplayFull == true)
        DBUGS.println("\n");
        ReadPCBTemperature(); // Read PCB NTC Temperature
        

    if (DisplayFull == true)
        DBUGS.println("\n");

    // Read PCB DCV_IN Voltage
    if (DisplayFull == true)
        ReadADCVoltage(); // Read AC>DC Input Voltage
    

    Settings.Bit.DisplayInProgress = 1;
}   //DisplayRegisters


//=====================================================================



void Load_Reg_Callibration(void) {

    Wire.begin();

    uint8_t regadd = 0;

    do {
        CalRegisters[regadd] = eic.GetRegisterValue(regadd + 65);
    } while (++regadd < sizeof(CalRegisters));

    regadd = 0;

    do {
        HarCalRegisters[regadd] = eic.GetRegisterValue(regadd + 81);
    } while (++regadd < sizeof(HarCalRegisters));

    regadd = 0;

    do {
        MeasurCalRegisters[regadd] = eic.GetRegisterValue(regadd + 97);
    } while (++regadd < sizeof(MeasurCalRegisters));

    


}



void Load_Power_Registers(void) {

    Settings.Bit.ReadDataInProgress = 1;

    TotalForwardActiveEnergy = NoiseFilterSquelch(eic.GetImportEnergy());
    PhaA_ForwardActiveEnergy = NoiseFilterSquelch(eic.GetImportEnergyPhaA());
    PhaB_ForwardActiveEnergy = NoiseFilterSquelch(eic.GetImportEnergyPhaB());
    PhaB_ForwardActiveEnergy = NoiseFilterSquelch(eic.GetImportEnergyPhaC());

    TotalReverseActiveEnergy = NoiseFilterSquelch(eic.GetExportEnergy());
    PhaA_ReverseActiveEnergy = NoiseFilterSquelch(eic.GetExportEnergyPhaA());
    PhaB_ReverseActiveEnergy = NoiseFilterSquelch(eic.GetExportEnergyPhaB());
    PhaC_ReverseActiveEnergy = NoiseFilterSquelch(eic.GetExportEnergyPhaC());

    TotalForwardReactiveEnergy = NoiseFilterSquelch(eic.GetImportReactiveEnergy());
    PhaA_ForwardReactiveEnergy = NoiseFilterSquelch(eic.GetImportReactiveEnergyPhaA());
    PhaB_ForwardReactiveEnergy = NoiseFilterSquelch(eic.GetImportReactiveEnergyPhaB());
    PhaC_ForwardReactiveEnergy = NoiseFilterSquelch(eic.GetImportReactiveEnergyPhaC());

    TotalReverseReactiveEnergy = NoiseFilterSquelch(eic.GetExportReactiveEnergy());
    PhaA_ReverseReactiveEnergy = NoiseFilterSquelch(eic.GetExportReactiveEnergyPhaA());
    PhaB_ReverseReactiveEnergy = NoiseFilterSquelch(eic.GetExportReactiveEnergyPhaB());
    PhaC_ReverseReactiveEnergy = NoiseFilterSquelch(eic.GetExportReactiveEnergyPhaC());

    Total_ArithmeticSum_ApparentEnergy = NoiseFilterSquelch(eic.GetImportApparentEnergyArt());
    PhaA_ApparentEnergy = NoiseFilterSquelch(eic.GetImportApparentEnergyPhaA());
    PhaB_ApparentEnergy = NoiseFilterSquelch(eic.GetImportApparentEnergyPhaB());
    PhaC_ApparentEnergy = NoiseFilterSquelch(eic.GetImportApparentEnergyPhaC());

    VectorSum_TotalApparentEnergy = NoiseFilterSquelch(eic.GetImportApparentEnergyVec());
    VectorSum_TotalApparentPower = NoiseFilterSquelch(eic.GetTotalVSumApparentPower());
    MeteringStatus0 = eic.GetMeterStatus0();
    MeteringStatus1 = eic.GetMeterStatus1();

    Total_PhasesumActivePower = NoiseFilterSquelch(eic.GetTotalActivePower());
    PhaA_ActivePower = NoiseFilterSquelch(eic.GetMeanActivePowerPhaA());
    PhaB_ActivePower = NoiseFilterSquelch(eic.GetMeanActivePowerPhaB());
    PhaC_ActivePower = NoiseFilterSquelch(eic.GetMeanActivePowerPhaC());

    Total_PhasesumReactivePower = NoiseFilterSquelch(eic.GetTotalReactivePower());
    PhaA_ReactivePower = NoiseFilterSquelch(eic.GetMeanReactivePowerPhaA());
    PhaB_ReactivePower = NoiseFilterSquelch(eic.GetMeanReactivePowerPhaB());
    PhaC_ReactivePower = NoiseFilterSquelch(eic.GetMeanReactivePowerPhaC());

    Total_ArithmeticSumapparentpower = NoiseFilterSquelch(eic.GetTotalApparentPower());
    PhaA_apparentpower = NoiseFilterSquelch(eic.GetMeanApparentPowerPhaA());
    PhaB_apparentpower = NoiseFilterSquelch(eic.GetMeanApparentPowerPhaB());
    PhaC_apparentpower = NoiseFilterSquelch(eic.GetMeanApparentPowerPhaC());

    Totalpowerfactor = NoiseFilterSquelch(eic.GetTotalPowerFactor());
    PhaA_powerfactor = NoiseFilterSquelch(eic.GetPowerFactorCT1());
    PhaB_powerfactor = NoiseFilterSquelch(eic.GetPowerFactorCT2());
    PhaC_powerfactor = NoiseFilterSquelch(eic.GetPowerFactorCT3());


    Totalactivefundamentalpower = NoiseFilterSquelch(eic.GetTotalActiveFundPower());
    PhaA_activefundamentalpower = NoiseFilterSquelch(eic.GetMeanActiveFundPowerPhaA());
    PhaB_activefundamentalpower = NoiseFilterSquelch(eic.GetMeanActiveFundPowerPhaB());
    PhaC_activefundamentalpower = NoiseFilterSquelch(eic.GetMeanActiveFundPowerPhaC());

    Totalactiveharmonicpower = NoiseFilterSquelch(eic.GetTotalActiveHarPower());
    PhaA_activeharmonicpower = NoiseFilterSquelch(eic.GetMeanActiveHarPowerPhaA());
    PhaB_activeharmonicpower = NoiseFilterSquelch(eic.GetMeanActiveHarPowerPhaB());
    PhaC_activeharmonicpower = NoiseFilterSquelch(eic.GetMeanActiveHarPowerPhaC());

    N_Line_SampledcurrentRMS = NoiseFilterSquelch(eic.GetLineCurrentCTNSamp());
    N_Line_calculatedcurrentRMS = NoiseFilterSquelch(eic.GetLineCurrentCTN());

    PhaA_voltageRMS = NoiseFilterSquelch(eic.GetLineVoltage1());
    PhaB_voltageRMS = NoiseFilterSquelch(eic.GetLineVoltage2());
    PhaC_voltageRMS = NoiseFilterSquelch(eic.GetLineVoltage3());

    PhaA_currentRMS = NoiseFilterSquelch(eic.GetLineCurrentCT1());
    PhaB_currentRMS = NoiseFilterSquelch(eic.GetLineCurrentCT2());
    PhaC_currentRMS = NoiseFilterSquelch(eic.GetLineCurrentCT3());

    PhaA_voltageTHDN = NoiseFilterSquelch(eic.GetLineVoltage1THDN());
    PhaB_voltageTHDN = NoiseFilterSquelch(eic.GetLineVoltage2THDN());
    PhaC_voltageTHDN = NoiseFilterSquelch(eic.GetLineVoltage3THDN());

    PhaA_currentTHDN = NoiseFilterSquelch(eic.GetLineCurrentCT1THDN());
    PhaB_currentTHDN = NoiseFilterSquelch(eic.GetLineCurrentCT2THDN());
    PhaC_currentTHDN = NoiseFilterSquelch(eic.GetLineCurrentCT3THDN());

    Frequency = NoiseFilterSquelch(eic.GetFrequency());

    PhaA_meanphaseangle = NoiseFilterSquelch(eic.GetPhaseCT1());
    PhaB_meanphaseangle = NoiseFilterSquelch(eic.GetPhaseCT2());
    PhaC_meanphaseangle = NoiseFilterSquelch(eic.GetPhaseCT3());

    Measuredtemperature = NoiseFilterSquelch(eic.GetTemperature());

    PhaA_voltagephaseangle = NoiseFilterSquelch(eic.GetPhaseangV1());
    PhaB_voltagephaseangle = NoiseFilterSquelch(eic.GetPhaseangV2());
    PhaC_voltagephaseangle = NoiseFilterSquelch(eic.GetPhaseangV3());

    Settings.Bit.ReadDataInProgress = 0;
    Power_Management = ReadDataIdle;

}

void LCD_PrintData(void) {

    char measurement[16];

    if (Display.Flag.Voltage == 1) {

        lcd.clear();
        dtostrf(LineVoltage1, -6, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("V1_Line : " + String(measurement) + "V");

        dtostrf(LineVoltage2, -6, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("V2_Line : " + String(measurement) + "V");

        dtostrf(LineVoltage3, -6, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("V3_Line : " + String(measurement) + "V");

        dtostrf(LineVoltageAverage, -6, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("Vavg : " + String(measurement) + "V");

        //Flash_LED(RUN_LED, 50, 1000, 1);
        Display.Flag.Voltage = 0;

    }

    if (Display.Flag.Current == 1) {

        lcd.clear();
        dtostrf(LineCurrentCT1, -6, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("I1_Line : " + String(measurement) + "A");

        dtostrf(LineCurrentCT2, -6, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("I2_Line : " + String(measurement) + "A");

        dtostrf(LineCurrentCT3, -6, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("I3_Line : " + String(measurement) + "A");

        dtostrf(LineCurrentCTN, -6, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("I_N : " + String(measurement) + "A");

        ClearLCD(T_3SEC);
       
        dtostrf(LineCurrentTotal, -6, 2, measurement);
        lcd.print("I_Tot : " + String(measurement) + "A");

        //Flash_LED(RUN_LED, 50, 1000, 1);
        Display.Flag.Current = 0;
    }

    if (Display.Flag.CalPower == 1) {

        lcd.clear();
        dtostrf(CalculatedPowerCT1, -7, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("P1 : " + String(measurement) + "W");

        dtostrf(CalculatedPowerCT2, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("P2 : " + String(measurement) + "W");

        dtostrf(CalculatedPowerCT3, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("P3 : " + String(measurement) + "W");

        dtostrf(CalculatedPowerCT4, -7, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("P_N : " + String(measurement) + "W");

        ClearLCD(T_3SEC);

        dtostrf(CalculatedTotalPower, -7, 2, measurement);
        lcd.print("P_Tot : " + String(measurement) + "W");

        //Flash_LED(RUN_LED, 50, 1000, 1);
        Display.Flag.CalPower = 0;

    }

    if (Display.Flag.ActPower == 1) {

        lcd.clear();
        dtostrf(ActivePowerCT1, -7, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("P1act : " + String(measurement) + "W " + ActiveFlag);

        dtostrf(ActivePowerCT2, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("P2act : " + String(measurement) + "W " + ActiveFlag);

        dtostrf(ActivePowerCT3, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("P3act : " + String(measurement) + "W " + ActiveFlag);

        ClearLCD(T_3SEC);
        dtostrf(TotalActivePower, -7, 2, measurement);
        lcd.print("P_t : " + String(measurement) + "W " + ActiveFlag);

        dtostrf(CalculatedTotalActivePower, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("P_tcal : " + String(measurement) + "W " + ActiveFlag);

        //Flash_LED(RUN_LED, 50, 1000, 1);
        Display.Flag.ActPower = 0;
    }

//=====================================================================================================================================
    
    if (Display.Flag.ReactPower == 1) {
        lcd.clear();
        dtostrf(ReactivePowerCT1, -7, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("Q1 : " + String(measurement) + "VAR");

        dtostrf(ReactivePowerCT2, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("Q2 : " + String(measurement) + "VAR");

        dtostrf(ReactivePowerCT3, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("Q3 : " + String(measurement) + "VAR");

        dtostrf(TotalReactivePower, -7, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("Qtot : " + String(measurement) + "VAR");

        //Flash_LED(RUN_LED, 50, 1000, 3);
        Display.Flag.ReactPower = 0;

    }

    if (Display.Flag.AppPower == 1) {
        lcd.clear();
        dtostrf(ApparentPowerCT1, -7, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("S1 : " + String(measurement) + "VA");

        dtostrf(ApparentPowerCT2, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("S2 : " + String(measurement) + "VA");

        dtostrf(ApparentPowerCT3, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("S3 : " + String(measurement) + "VA");

        dtostrf(TotalApparentPower, -7, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("Stot : " + String(measurement) + "VA");

        //Flash_LED(RUN_LED, 50, 1000, 3);
        Display.Flag.AppPower = 0;

    }

    if (Display.Flag.PFactor == 1) {
        lcd.clear();
        dtostrf(PowerFactorCT1, -5, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("PF1 : " + String(measurement));

        dtostrf(PowerFactorCT2, -5, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("PF2 : " + String(measurement));

        dtostrf(PowerFactorCT3, -5, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("PF3 : " + String(measurement));

        dtostrf(TotalPowerFactor, -5, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("PF : " + String(measurement));

        //Flash_LED(RUN_LED, 50, 1000, 3);
        Display.Flag.PFactor = 0;
    }

    if (Display.Flag.PAngle == 1) {
        lcd.clear();
        dtostrf(PhaseAngleCT1, -7, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("ang1 : " + String(measurement));

        dtostrf(PhaseAngleCT2, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("ang2 : " + String(measurement));

        dtostrf(PhaseAngleCT3, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("ang3 : " + String(measurement));

        //Flash_LED(RUN_LED, 50, 1000, 3);
        Display.Flag.PAngle = 0;
    }


    if (Display.Flag.LineFreq == 1) {
        lcd.clear();
        dtostrf(LineFrequency, -5, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("Freq : " + String(measurement) + "Hz");

        //Flash_LED(RUN_LED, 50, 1000, 3);
        Display.Flag.LineFreq = 0;
    }

    if (Display.Flag.THData == 1) {
        lcd.clear();
        dtostrf(Hum, -6, 2, measurement);
        lcd.setCursor(0, 0);
        lcd.print("HUM : " + String(measurement) + "%");

        dtostrf(Temp_C, -6, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("Tc : " + String(measurement) + (char)223 + "C");

        dtostrf(Temp_F, -6, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("Tf : " + String(measurement) + (char)223 + "F");

        //Flash_LED(RUN_LED, 50, 1000, 3);
        Display.Flag.THData = 0;

    }


    if (Display.Flag.ActEnergy_Imp == 1) {

        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Active Energy");
        dtostrf(PhaA_ForwardActiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("Pha A(Imp) : " + String(measurement) + "kWh");
        dtostrf(PhaB_ForwardActiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("Pha B(Imp) : " + String(measurement) + "kWh");
        dtostrf(PhaC_ForwardActiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("Pha C(Imp) : " + String(measurement) + "kWh");

        ClearLCD(T_3SEC);


        lcd.print("Total Energy");
        lcd.setCursor(0, 1);
        lcd.print("Imported : ");
        dtostrf(TotalForwardActiveEnergy, -7, 2, measurement);
        lcd.print(String(measurement) + " kWh");

        //Flash_LED(RUN_LED, 50, 1000, 1);
        Display.Flag.ActEnergy_Imp = 0;
    }



    //====================================================================================================

    if (Display.Flag.ActEnergy_Exp == 1) {
    
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Active Energy");
        dtostrf(PhaA_ReverseActiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("Pha A(Exp) : " + String(measurement) + "kWh");
        dtostrf(PhaB_ReverseActiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("Pha B(Exp) : " + String(measurement) + "kWh");
        dtostrf(PhaC_ReverseActiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("Pha C(Exp) : " + String(measurement) + "kWh");

        ClearLCD(T_3SEC);

        lcd.print("Total Energy");
        lcd.setCursor(0, 1);
        lcd.print("Exported: ");
        dtostrf(TotalReverseActiveEnergy, -7, 2, measurement);
        lcd.print(String(measurement) + "kWh");

        //Flash_LED(RUN_LED, 50, 1000, 1);
        Display.Flag.ActEnergy_Exp = 0;
    }

    


    //====================================================================================================

    if (Display.Flag.ReactEnergy_Imp == 1) {

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Reactive Eng kVarh");
        dtostrf(PhaA_ForwardReactiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("Pha A(Imp): " + String(measurement));
        dtostrf(PhaB_ForwardReactiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("Pha B(Imp): " + String(measurement));
        dtostrf(PhaC_ForwardReactiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("Pha C(Imp): " + String(measurement));

        ClearLCD(T_3SEC);

        lcd.print("Total Energy");
        lcd.setCursor(0, 1);
        lcd.print("Imported kVarh ");
        lcd.setCursor(0, 2);
        dtostrf(TotalForwardReactiveEnergy, -7, 2, measurement);
        lcd.print(String(measurement));

        //Flash_LED(RUN_LED, 50, 1000, 1);
        Display.Flag.ReactEnergy_Imp = 0;

    }

    //====================================================================================================

    if (Display.Flag.ReactEnergy_Exp == 1) {
    
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Reactive Eng kVarh");
        dtostrf(PhaA_ReverseReactiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("Pha A(Exp): " + String(measurement));
        dtostrf(PhaB_ReverseReactiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("Pha B(Exp): " + String(measurement));
        dtostrf(PhaC_ReverseReactiveEnergy, -7, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("Pha C(Exp): " + String(measurement));

        ClearLCD(T_3SEC);

        lcd.print("Total Energy");
        lcd.setCursor(0, 1);
        lcd.print("Exported kVarh");
        lcd.setCursor(0, 2);
        dtostrf(TotalReverseReactiveEnergy, -7, 2, measurement);
        lcd.print(String(measurement));

        //Flash_LED(RUN_LED, 50, 1000, 1);
        Display.Flag.ReactEnergy_Exp = 0;
    }




    //====================================================================================================

    if (Display.Flag.ApptEnergy == 1) {
    
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Apparent Energy");
        dtostrf(PhaA_ApparentEnergy, -7, 2, measurement);
        lcd.setCursor(0, 1);
        lcd.print("Pha A : " + String(measurement) + "kVAh");
        dtostrf(PhaB_ApparentEnergy, -7, 2, measurement);
        lcd.setCursor(0, 2);
        lcd.print("Pha B : " + String(measurement) + "kVAh");
        dtostrf(PhaC_ApparentEnergy, -7, 2, measurement);
        lcd.setCursor(0, 3);
        lcd.print("Pha C : " + String(measurement) + "kVAh");

        ClearLCD(T_3SEC);

        lcd.print("Total Energy");
        lcd.setCursor(0, 1);
        lcd.print("Arithmetic Sum");
        lcd.setCursor(0, 2);
        dtostrf(Total_ArithmeticSum_ApparentEnergy, -7, 2, measurement);
        lcd.print(String(measurement) + " kVAh");
        lcd.setCursor(0, 3);

        dtostrf(VectorSum_TotalApparentEnergy, -7, 2, measurement);
        lcd.print("Vector Sum: " + String(measurement) + "kVAh");

        //Flash_LED(RUN_LED, 50, 1000, 1);
        Display.Flag.ApptEnergy = 0;
    
    }



    if (Display.Flag.WiFiState == 1) {
        DisplayWiFiStatus();
        Peripheral_Management = PeripheralsIdle;
        Display.Flag.WiFiState = 0;
        Settings.Bit.DisplayInProgress = 0;

    }




}



