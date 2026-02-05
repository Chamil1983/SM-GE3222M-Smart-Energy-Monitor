extern void configureInterrupts();
extern void TestPeripherals();

// **************** FUNCTIONS AND ROUTINES ****************

void SetOutputPin(void) {

    uint8_t Count = 0;
    OutputPin[0] = RELAY_2;
    OutputPin[1] = RELAY_1;
    OutputPin[2] = WIFI_LED;
    OutputPin[3] = RUN_LED;
    OutputPin[4] = FLT_LED;
    OutputPin[5] = MODBUS_LED;

    // Initialize I2C
    Wire.begin(I2C_SDA, I2C_SCL);

    mcp23017.init();  // initialize MCP23017 IC

    // Configure MCP23017 I/O pins

    // Configure output pins
    do {
        mcp23017.pinMode(OutputPin[Count], OUTPUT);
        
    } while (++Count < sizeof(OutputPin));


    // LEDs Default Off State
    mcp23017.digitalWrite(RELAY_2, OUTPUT_STATE_OFF);
    mcp23017.digitalWrite(RELAY_1, OUTPUT_STATE_OFF);
    mcp23017.digitalWrite(WIFI_LED, LED_OFF);
    mcp23017.digitalWrite(RUN_LED, LED_OFF);
    mcp23017.digitalWrite(FLT_LED, LED_OFF);
    mcp23017.digitalWrite(MODBUS_LED, LED_OFF);

}   //SetOutputPin

//======================================================================
//  Sets digital inputs
void SetInputPin(void) {
    InputPin[0] = Mode_Button;
    InputPin[1] = Set_Button;
    InputPin[2] = INT_B;
    InputPin[3] = IRQ0;
    InputPin[4] = IRQ1;
    InputPin[5] = ATM_WO;
    InputPin[6] = CF1;
    InputPin[7] = CF2;
    InputPin[8] = CF3;
    InputPin[9] = CF4;

    pinMode(InputPin[0], INPUT);          // set pin to input
    pinMode(InputPin[1], INPUT);          // set pin to input
    pinMode(InputPin[2], INPUT);          // set pin to input

        // Configure input pins with internal 100K pull-up resistors
    // Third argument inverts the polarity of the input value when read
    digitalWrite(InputPin[0], LOW);      // turn Off pullup resistors
    digitalWrite(InputPin[1], LOW);      // turn Off pullup resistors
    digitalWrite(InputPin[2], HIGH);      // turn on pullup resistors


    mcp23017.pinMode(InputPin[3], INPUT_PULLUP, true);
    mcp23017.pinMode(InputPin[4], INPUT_PULLUP, true);
    mcp23017.pinMode(InputPin[5], INPUT_PULLUP, true);
    mcp23017.pinMode(InputPin[6], INPUT_PULLUP, true);
    mcp23017.pinMode(InputPin[7], INPUT_PULLUP, true);
    mcp23017.pinMode(InputPin[8], INPUT_PULLUP, true);
    mcp23017.pinMode(InputPin[9], INPUT_PULLUP, true);



    /*
    InputPin[0] = digitalRead(Mode_Button); //Mode_Button
    InputPin[1] = digitalRead(Set_Button); //Set_Button
    InputPin[2] = digitalRead(INT_B); //Set_Button
    InputPin[3] = mcp23017.digitalRead(IRQ0); //IRQ0 - Interrupt Output 0 This pin is asserted when one or more events in the SysStatus0 register(01H) occur.
    InputPin[4] = mcp23017.digitalRead(IRQ1); //IRQ1 - This pin is asserted when one or more events in the SysStatus1 register(02H) occur.
    InputPin[5] = mcp23017.digitalRead(ATM_WO); //ATM_WO - Fatal Error Warning
    InputPin[6] = mcp23017.digitalRead(CF1); //CF1 (all-phase-sum total) Active Energy Pulse Output
    InputPin[7] = mcp23017.digitalRead(CF2); //CF2 (all-phase-sum total) Reactive/ Apparent Energy Pulse Output
    InputPin[8] = mcp23017.digitalRead(CF3); //CF3 (all-phase-sum total) Active Fundamental Energy Pulse Output
    InputPin[9] = mcp23017.digitalRead(CF4); //CF4 (all-phase-sum total) Active Harmonic Energy Pulse Output
    
    
    */

    Mode_SW.numberKeyPresses = 0;
    Set_SW.numberKeyPresses = 0;

    //Settings.Bit.SettingInProgress = 0;

    Settings.Bit.PcUartRequest = 0;
    Settings.Bit.DisplayInProgress = 0;
    Settings.Bit.WiFiState = 0;
    Settings.Bit.WiFi_APMode = 0;
    Settings.Bit.WiFi_StaMode = 0;

    DigitalInput[0].Bit.InputStatus = 0;
    DigitalInput[1].Bit.InputStatus = 0;
    DigitalInput[2].Bit.InputStatus = 0;
    DigitalInput[3].Bit.InputStatus = 0;
    DigitalInput[4].Bit.InputStatus = 0;
    DigitalInput[5].Bit.InputStatus = 0;
    DigitalInput[6].Bit.InputStatus = 0;
    DigitalInput[7].Bit.InputStatus = 0;
    DigitalInput[8].Bit.InputStatus = 0;
    DigitalInput[9].Bit.InputStatus = 0;


}   //SetInputPin
//======================================================================

// Calculate Average Value and Reduce Jitter
int CalculateADCAverage(int SensorChannel)
{
    int AverageRAW = 0;
    if (EnableAveraging == true)
    {

        for (int i = 0; i < AverageSamples; i++)
        {
            AverageRAW = AverageRAW + analogRead(SensorChannel);
            delay(AverageDelay);
        }
        AverageRAW = AverageRAW / AverageSamples;
        if (AverageRAW < 2)
            AverageRAW = 0;
    }
    else
    {
        AverageRAW = analogRead(SensorChannel);
    }
    return AverageRAW;
} // CalculateADCAverage

// Rough and Ready Underline Text.
void PrintUnderline(String sText)
{
    int count = 0;
    DBUGS.println(sText);

    while (count <= sText.length())
    {
        DBUGS.print("─");
        count++;
    }
    DBUGS.println("");
} // PrintUnderline

// Equally Rough and Ready Dash Separator
void PrintSeparator(String sText)
{
    int count = 0;

    while (count <= (sText.length() / 2) + 1)
    {
        DBUGS.print("- ");
        count++;
    }
    DBUGS.println("");

    DBUGS.println(sText);
} // PrintSeparator


// Initialise OLED
void InitialiseLCD()
{
    if (LCD_Enabled == true || EnableLCDLoop == true)
    {
        DBUGS.println("Initialise LCD (If Plugged In)\n");

        lcd.begin();

        // turn on LCD backlight
        lcd.backlight();
        lcd.noCursor();
        lcd.clear();
        /*
        main_menu.init();

        back_line.set_focusPosition(Position::LEFT);

        back_line.attach_function(1, go_back);
        delay(2000);
        back_line.attach_function(2, go_back);
        */
        lcd.setCursor(0, 0);
        // print message
        lcd.print("SM-GE3222M Init...");

        lcd.setCursor(0, 1);
        lcd.print(AppAcronym);


        // App and Firmware Version
        lcd.setCursor(0, 2);
        lcd.print(AppVersion + " v");
        delay(2000);



        // App and Starting...
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(AppAcronym);
        lcd.print(" Starting..");
        lcd.setCursor(0, 1);
        lcd.print("SM_GE3222M");


       

        delay(3000);


        

        
    }
} // InitialiseOLED


void go_back() {
    // This function takes reference to the wanted menu.
    menu_system.change_menu(main_menu);
}


void ConfigureBoard()
{
    //Get Unique ID String
    MicroID.getUniqueIDString(id);
    serial_no = id;
       
    SetOutputPin();  //Setup Outputs

    SetInputPin();  //Setup Inputs



    configureInterrupts();

    // Reset MCP23017 ports
    mcp23017.writeRegister(MCP23017Register::GPIO_A, 0x3C);
    mcp23017.writeRegister(MCP23017Register::GPIO_B, 0x00);

    




} // ConfigureBoard

void ConfigIdle(void){}

//======================================================================
void DebouncingInputPin(void) {
    uint8_t n = 0;

    do {
        DigitalInput[n].Bit.InputReaded = digitalRead(InputPin[n]);

        if (DigitalInput[n].Bit.InputReaded != DigitalInput[n].Bit.InputStatus) {
            if (DigitalInput[n].Bit.InputReaded != DigitalInput[n].Bit.InputVar) {
                DigitalInput[n].Bit.InputVar = DigitalInput[n].Bit.InputReaded;
                DebouncingInput[n] = T_50MSEC;
            }
            else {
                if (DebouncingInput[n] == 0) {
                    DigitalInput[n].Bit.InputVar = DigitalInput[n].Bit.InputReaded;
                    DigitalInput[n].Bit.InputStatus = DigitalInput[n].Bit.InputReaded;
                }
            }
        }
        else {
            DigitalInput[n].Bit.InputVar = DigitalInput[n].Bit.InputStatus;
        }
    } while (++n < 2);
}
//======================================================================



void DigitalControl(uint8_t Pin, uint8_t state) {


    if (state == OUTPUT_STATE_ON) {
        mcp23017.digitalWrite(Pin, OUTPUT_STATE_ON);
    }
    else {
        mcp23017.digitalWrite(Pin, OUTPUT_STATE_OFF);
    }


}



void Flash_LED(uint8_t SelectOutput, int ton, int toff, uint16_t numFlash = 0) {

    for (int i = 0; i < numFlash; i++) {
        mcp23017.digitalWrite(OutputPin[SelectOutput], LED_ON);
        delay(ton);
        mcp23017.digitalWrite(OutputPin[SelectOutput], LED_OFF);
        delay(toff);

    }

}


// Display Board Configuration
void DisplayBoardConfiguration()
{

    // Header
    DBUGS.println("Register Status and Diagnostic Report");
    DBUGS.println("SM-GE3222M ATM90E36 IoT Power Energy Monitor");
    DBUGS.printf("Device Serial ID: %04X", (uint16_t)(chipid >> 32));
    DBUGS.printf("%08X", (uint32_t)chipid);
    DBUGS.print("   Firmware Version: ");
    DBUGS.print(AppVersion);
    DBUGS.print("  Hardware Version: ");
    DBUGS.println(HardwareRev);
    DBUGS.println("");

    // Stabalise
    delay(100);

    // Display Hardware Configuration for ATM90E32 ATM90E36
#if ATM90DEVICE == ATM90E32_DEVICE
    PrintUnderline("Hardware Configuration for ATM90E32");
#endif

#if ATM90DEVICE == ATM90E36_DEVICE
    PrintUnderline("Hardware Configuration for ATM90E36");
#endif

    // CT4 Configuration / ATM Status Values
#if CT4_CONFIG == CT4_ATM && ATM90DEVICE == ATM90E32_DEVICE || CT4_ATM == 90 && CT4_ENABLED == true
    DBUGS.println("* SOFTWARE CONFIGURATION ERROR *\n* You cannot have an ATM90E32 and I4N Input Selected (Change CT4_CONFIG) *\n");
#endif

#if CT4_ENABLED == true
    DBUGS.println("* SOFTWARE CONFIGURATION ERROR *\n* You cannot Use CT4 as Current Clamp Input and GPIO Set (Check CT4_ENABLED, CT4_CONFIG) *\n");
#endif

#if ATM_SINGLEVOLTAGE == true
    DBUGS.println("AC Voltage Inputs:\tSingle Input V1 will be used for Current Phase Calculations");
#else
#if ATM_SPLITPHASE == false // World Single, Three Phase or 3 x Single Phases
    DBUGS.println("AC Voltage Inputs:\tMulti Separated V1 V2 V3 - 3 Phase, or 3 x Single Phase Configuration.");
#else                       // USA Split Phase 120+120
    DBUGS.println("AC Voltage Inputs:\tMulti Dual V1 and V3 - USA Configuration");
#endif
#endif

#if ATM_SPLITPHASE == true
#if ATM_SINGLEVOLTAGE == false
#if CT4_CONFIG == CT4_ESP && ATM90DEVICE == ATM90E32_DEVICE || CT4_CONFIG == CT4_ESP
    // USA 120+120
    DBUGS.println("Split AC Voltage:\tDual Input V1 and V3 Enabled - USA Configuration");
    DBUGS.println("CT Current Clamps:\tPhase Configured for 1 and 3 + 1 Phase (ESP32)");
#else
    // USA 120+120
    DBUGS.println("Split AC Voltage:\tDual Input V1 + V3 Enabled - USA Configuration");
    DBUGS.println("CT Current Clamps:\tPhase Configured for 1 and 3 + 1 Phase (I4N)");
#endif
#else
    // Cannot have Split AC Voltage input with Single Voltage Input Selected
    DBUGS.println("Split AC Voltage:\tConfiguration Error (Check ATM_SINGLEVOLTAGE)");
#endif
#else
    // World
#if CT4_CONFIG == CT4_ESP && ATM90DEVICE == ATM90E32_DEVICE || CT4_CONFIG == CT4_ESP
    DBUGS.println("Split AC Voltage:\tDual Split-Phase (V1-x-V3) Disabled");
    DBUGS.println("CT Current Clamps:\tConfigured for 1, 2, 3 Phase + 1 Phase (ESP32)");
#else
    DBUGS.println("Split AC Voltage:\tDual or Split Voltage Input Disabled");

#if ATM90DEVICE == ATM90E36_DEVICE
    DBUGS.println("CT Current Clamps:\tConfigured for 1, 2, 3 Phase + 1 Phase (I4N)");
#else
    DBUGS.println("CT Current Clamps:\tConfigured for 1, 2, 3 Phase");
#endif

#endif

#endif

#if CT4_CONFIG == CT4_ESP || ATM90DEVICE == ATM90E32_DEVICE && CT4_CONFIG == CT4_ESP
    DBUGS.println("CT4 Current Input:\tConfigured for ESP32 ADC");
#else
#if ATM90DEVICE == ATM90E36_DEVICE
    DBUGS.print("CT4 Current Input:\tConfigured for I4N on the ");
    DBUGS.println(ATM90DEVICE);
#endif
#endif

    DBUGS.println("");

    // Registers
    String SysStatus0_Value = String(eic.GetSysStatus0(), HEX); // EMMState0
    SysStatus0_Value.toUpperCase();
    String SysStatus1_Value = String(eic.GetSysStatus1(), HEX); // EMMState1
    SysStatus1_Value.toUpperCase();
    String MeterStatus0_Value = String(eic.GetMeterStatus0(), HEX); // EMMIntState0
    MeterStatus0_Value.toUpperCase();
    String MeterStatus1_Value = String(eic.GetMeterStatus1(), HEX); // EMMIntState1
    MeterStatus1_Value.toUpperCase();

    DBUGS.println("[Status Values]");
    DBUGS.println("System Status: \t\tS0:0x" + SysStatus0_Value + "\t\tS1:0x" + SysStatus1_Value);
    DBUGS.println("Meter Status:  \t\tE0:0x" + MeterStatus0_Value + "\tE1:0x" + MeterStatus1_Value);

    // Stabalise
    delay(100);

    // if true the MCU is not getting data from the energy meter
    unsigned short sys0 = eic.GetSysStatus0(); // EMMState0
    if (sys0 == 65535 || sys0 == 0)
    {
        DBUGS.println("");
        DBUGS.println("*********************************************************************");
        DBUGS.print("* Error: Not Correct Data from ");
        DBUGS.print(ATM90DEVICE);
        DBUGS.println(" Energy Meter - Check Device *");
        DBUGS.println("*********************************************************************");
    }

    DBUGS.println("");
} // DisplayBoardConfiguration

// Test RGB LEDs
//======================================================================
//  Function used to test leds
void PowerOnLedTest(void) {
    uint8_t Count = 2;

    //lcd.clear();
    lcd.setCursor(0, 2);

    lcd.print("Testing LEDS => ");


    for (int i = 0; i < 4; i++) {
        Count = 2;
        do {
            OnOff_Output(Count, LED_ON);
            OnOff_Output(RELAY_1, OUTPUT_STATE_ON);
            OnOff_Output(RELAY_2, OUTPUT_STATE_ON);

        } while (++Count < sizeof(OutputPin));

        delay(300);
        Count = 2;

        do {
            OnOff_Output(Count, LED_OFF);
            OnOff_Output(RELAY_1, OUTPUT_STATE_OFF);
            OnOff_Output(RELAY_2, OUTPUT_STATE_OFF);

        } while (++Count < sizeof(OutputPin));

        delay(500);


    }

    lcd.print("PASS");
    delay(1000);
    

    

}
//===============================================================================================
void OnOff_Output(uint8_t SelectOutput, uint8_t OnOff) {
    if (OnOff != 1) {
        mcp23017.digitalWrite(OutputPin[SelectOutput], LED_ON);
        LastStateOutputPin[SelectOutput] = OUTPUT_STATE_ON;
    }
    else {

        mcp23017.digitalWrite(OutputPin[SelectOutput], LED_OFF);
        LastStateOutputPin[SelectOutput] = OUTPUT_STATE_OFF;
    }
}
// 
// 
// 
// I2C Bus Scanner
void ScanI2CBus()
{

    byte error, address;
    int nDevices;

    DBUGS.println("Scanning I2C Bus for Devices ...");
    lcd.setCursor(0, 3);
    lcd.print("Scanning I2C => ");
    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            DBUGS.print(" * I2C device found at address Decimal ");
            if (address < 16)
            {
                DBUGS.print("0");
            }
            DBUGS.print(address);
            DBUGS.print(" = Hex 0x");
            DBUGS.print(address, HEX);

            switch (address)
            {
            case 0x18 ... 0x1F:
                DBUGS.print(" MCP9808 Temerature Sensor");
                break;
            case 0x20 ... 0x27:
                DBUGS.print(" GPIO Expander");
                LCD_Enabled = true;
                break;
            case 0x28 ... 0x2B:
                DBUGS.print(" Digital Pot");
                break;
            case 0x38:
                DBUGS.print(" Humidity/Temperature Sensor");
                break;
            case 0x40 ... 0x47:
                DBUGS.print(" Humidity/Temperature Sensor");
                break;
            case 0x48 ... 0x4B:
                DBUGS.print(" ADS1115, ADS7828 or Sensor");
                break;
            case 0x4C ... 0x4F:
                DBUGS.print(" Sensor");
                break;
            case 0x3C ... 0x3D:
                DBUGS.print(" OLED");
                //OLED_Enabled = true;
                break;
            case 0x50 ... 0x5F:
                DBUGS.print(" EEPROM");
                break;
            case 0x76 ... 0x77:
                DBUGS.print(" BMP/BME Temerature/Humidity/Barometric");
                break;
            }
            DBUGS.println();

            nDevices++;
        }
        else if (error == 4)
        {
            DBUGS.print(" * Unknown error at address Decimal ");
            if (address < 16)
            {
                DBUGS.print("0");
            }
            DBUGS.print(address);
            DBUGS.print(" = Hex 0x");
            DBUGS.println(address, HEX);
        }
    }
    if (nDevices == 0)
    {
        DBUGS.println(" * No I2C devices found. Possible Hardware Issue?");
    }
    else
    {
        DBUGS.println(" * I2C Bus Scan Complete\n");
        lcd.print("PASS");

        delay(3000);
    }
} // ScanI2CBus

// Read PCB NTC Thermister on VN
void ReadPCBTemperature()
{

    char measurement[16];

    dht.begin();
    delay(1000);
    // Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    Hum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    Temp_C = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    Temp_F = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(Hum) || isnan(Temp_C) || isnan(Temp_F)) {
        DBUGS.println(F("Failed to read from DHT sensor!"));
        return;
    }

    // Compute heat index in Fahrenheit (the default)
    hif = dht.computeHeatIndex(Temp_F, Hum);
    // Compute heat index in Celsius (isFahreHumeit = false)
    hic = dht.computeHeatIndex(Temp_C, Hum, false);


    //DBUGS.println("Humidity\t\t: \t\t" + String(Hum, 2) + "%");
    //DBUGS.println("Temperature\t\t: \t\t" + String(Temp_C, 2)+ " °C"+ "\t " + String(Temp_F, 2) + " °F");
    //DBUGS.println("Heat index\t\t: \t\t" + String(hic, 2) + " °C" + "\t" + String(hif, 2) + " °F");



    DBUGS.println("\n\n");
} // ReadPCBTemperature

// Read DCV_IN via VP
void ReadADCVoltage()
{
    // Raw ADC Voltage Output to VoltageSensorRaw
    VoltageSensorRaw = CalculateADCAverage(DCV_IN);
    DBUGS.print("ADC Raw\t\t: \t\t ");
    DBUGS.print(VoltageSensorRaw);
    DBUGS.print(" > ");

    // Adjust Raw ADC Voltage
    VoltageSensorRaw = VoltageSensorRaw + VoltageRawFactor;
    DBUGS.print("ADC Adjusted\t\t: \t\t ");
    DBUGS.print(VoltageSensorRaw);
    DBUGS.print(" > ");

    // Calculated ADC Voltage Output
    VoltageCalculated = ((VoltageFactor / 4095.0) * VoltageSensorRaw);
    DBUGS.print("Calculated\t\t: \t\t ");
    DBUGS.print(VoltageCalculated);
    DBUGS.println(" V");

    DCVoltage = VoltageCalculated;
    DBUGS.println();
} // ReadADCVoltage

// Check DCV_IN DC Voltage
void CheckDCVINVoltage()
{
    // Raw ADC Voltage Output to VoltageSensorRaw
    VoltageSensorRaw = CalculateADCAverage(DCV_IN);
    VoltageSensorRaw = VoltageSensorRaw + VoltageRawFactor;
    VoltageCalculated = ((VoltageFactor / 4095.0) * VoltageSensorRaw);

    // DCV_IN ReadADCVoltage
    yield();
    if (VoltageCalculated < 5)
        DBUGS.print("* SM-GE32222M Board appears to be only USB Powered.\n* Not all ATM functions will work in this mode\n");
    if (VoltageCalculated > 5)
        DBUGS.print("DC Voltage Input Detected");
    if (VoltageCalculated > 30)
    {
        DBUGS.print("* WARNING: Please Check Input Voltage.  Too High! Value ");
        DBUGS.println(VoltageCalculated);
    }

    DCVoltage = VoltageCalculated;
    DBUGS.println();

} // CheckDCVINVoltage

// Read CT4 Current (Calculated for Low Current Use around < 1.7A - NO Current Flow Polarity).
// Update Values as required in above Energy Monitor Library Configuration.
void ReadCT4Current()
{
#if CT4_CONFIG == CT4_ESP || ATM90DEVICE == ATM90E32_DEVICE
    LineCurrentCT4 = emon1.calcIrms(EmonCalcIrms); // Calculate Irms only.
    LineCurrentCT4 = LineCurrentCT4 / 1000;

    // CT4 Squelch
    LineCurrentCT4 = NoiseFilterSquelch(LineCurrentCT4, EmonThreshold);

    CalculatedPowerCT4 = LineCurrentCT4 * LineVoltage1; // Use Voltage Input 1 to Calculate Power for CT4
#endif
} // ReadCT4Current


//======================================================================
String ReadStringCmd_FLASH(uint8_t* FlashPointer, uint8_t Lenght, boolean PrintCR, boolean NoPrint) {
    uint8_t k;
    char    myChar;
    String  TempString;

    for (k = 0; k < Lenght; k++) {
        myChar = pgm_read_byte_near(FlashPointer + k);
        if (NoPrint == FALSE) {
            DBUGS.print(myChar);
        }
        TempString += myChar;
    }
    if (NoPrint == FALSE) {
        if (PrintCR == TRUE) {
            DBUGS.print("\n");
        }
    }
    return(TempString);
}
//======================================================================


//======================================================================
void InputP1_Idle(void) {
    if (DigitalInput[0].Bit.InputStatus != 0) {
        TimeOutInputPin[0] = T_7SEC;
        //Mode Button Functions
        
        
        InputP1_Management = InputP1_Wait;
        //InputP1_Management();
        //Settings.Bit.SettingInProgress = 1;
        return;
    }
}

void InputP1_Released(void) {
    if (DigitalInput[0].Bit.InputStatus != 1) {

        Settings.Bit.SettingInProgress = 0;
        InputP1_Management = InputP1_Idle;
    }
}
//======================================================================

//======================================================================
void InputP2_Idle(void) {
    if (DigitalInput[1].Bit.InputStatus != 0) {
        TimeOutInputPin[1] = T_7SEC;
        //Mode Button Functions
        //InputP1_Management = InputP1_Wait;
        
        return;
    }
}

void InputP2_Released(void) {
    if (DigitalInput[1].Bit.InputStatus != 1) {

        Settings.Bit.SettingInProgress = 0;
        InputP2_Management = InputP2_Idle;
    }
}
//======================================================================
void InputP1_Wait(void) {

    if (Mode_SW.pressed &&(Settings.Bit.TimerFlag1==1) && (DigitalInput[0].Bit.InputStatus != 0)) {

        Settings.Bit.TimerFlag1 = 0;
        OnOff_Output(RELAY_1, OUTPUT_STATE_ON);
        
        
        InputP1_Management = LCD_MenuHandler;    //  Button released
        Settings.Bit.SettingInProgress = 1;
        pageChanged = true;
        lcd.clear();
        lcd.setCursor(5, 1);
        lcd.print("Menu Enabled");
        Mode_SW.numberKeyPresses = 0;
        //detachInterrupt(Mode_SW.PIN);
        //detachInterrupt(Set_SW.PIN);
        delay(2000);
        while (digitalRead(Mode_SW.PIN));
        OnOff_Output(RELAY_1, OUTPUT_STATE_OFF);

        Mode_SW.pressed = false;
        Settings.Bit.TimerFlag1 = 0;
        LCDCount = 0;

        lcd.clear();
        lcd.print(F(" System info"));
        lcd.setCursor(0, 1);
        lcd.print(F(" Energy info"));
        lcd.setCursor(0, 2);
        lcd.print(F(" Hardware Config"));
        lcd.setCursor(0, 3);
        lcd.print(F(" Exit Menu"));
        InputP1_Management();


    }

    else {

        pageChanged = false;
        Settings.Bit.TimerFlag1 = 0;
        Settings.Bit.SettingInProgress = 0;

    }





}

void executeAction() {
    switch (Mode_SW.numberKeyPresses) {
    case 1:
        Menu1();

        
        break;
    case 2:
        Settings.Bit.DisplayInProgress = 1;
        Menu2();
        if (LCDCount > 16) { LCDCount = 0; }
        
        //pageChanged = false;
        //InputP1_Management = InputP1_Idle;

        break;
    case 3:
        Menu3();
        //pageChanged = false;
        //InputP1_Management = InputP1_Idle;

        break;

    case 4:
        pageChanged = false;
        InputP1_Management = InputP1_Idle;
        Settings.Bit.SettingInProgress = 0;
        Mode_SW.numberKeyPresses = 0;
        lcd.clear();

        break;


    }
}

void LCD_MenuHandler() {

    while (pageChanged) {

        if (digitalRead(Mode_SW.PIN)) {
            Mode_SW.numberKeyPresses++;
            Select_Menu();
            delay(100);
            while (digitalRead(Mode_SW.PIN));
            Mode_SW.pressed = false;
            DigitalInput[0].Bit.InputStatus = 0;

            if (Mode_SW.numberKeyPresses > 3) { Mode_SW.numberKeyPresses = 0; }
            //break;
        }

        if (digitalRead(Set_SW.PIN)) {
            
            executeAction();
            Select_Menu();
            delay(100);
            while (digitalRead(Set_SW.PIN));
            Set_SW.pressed = false;
            DigitalInput[1].Bit.InputStatus = 0;

            if (Settings.Bit.DisplayInProgress == 1) { LCDCount++; }
            

            //break;
        }
    }

}




void Select_Menu() {

    if (pageChanged) {

        // reset page changed flag so only refresh the page when needed
        switch (Mode_SW.numberKeyPresses) {
            //case 0: Mode_SW.numberKeyPresses = 0;

        case 1:                      // page 1 chosen
            lcd.clear();
            lcd.print(F(">System info"));
            lcd.setCursor(0, 1);
            lcd.print(F(" Energy info"));
            lcd.setCursor(0, 2);
            lcd.print(F(" Hardware Config"));
            lcd.setCursor(0, 3);
            lcd.print(F(" Exit Menu"));



            break;

        case 2:                      // page 2 chosen
            lcd.clear();
            lcd.print(F(" System info"));
            lcd.setCursor(0, 1);
            lcd.print(F(">Energy info"));
            lcd.setCursor(0, 2);
            lcd.print(F(" Hardware Config"));
            lcd.setCursor(0, 3);
            lcd.print(F(" Exit Menu"));


            break;

        case 3:                      // page 3 chosen
            lcd.clear();
            lcd.print(F(" System info"));
            lcd.setCursor(0, 1);
            lcd.print(F(" Energy info"));
            lcd.setCursor(0, 2);
            lcd.print(F(">Hardware Config"));
            lcd.setCursor(0, 3);
            lcd.print(F(" Exit Menu"));

            break;

        case 4:                      // page 3 chosen
            lcd.clear();
            lcd.print(F(" System info"));
            lcd.setCursor(0, 1);
            lcd.print(F(" Energy info"));
            lcd.setCursor(0, 2);
            lcd.print(F(" Hardware Config"));
            lcd.setCursor(0, 3);
            lcd.print(F(">Exit Menu"));

            break;

        default:                    // something went wrong - invalid page, so set and show page 1
            Mode_SW.numberKeyPresses = 0;
            pageChanged = false;
            Mode_SW.pressed = false;
            lcd.clear();
            InputP1_Management = InputP1_Idle;
            Settings.Bit.SettingInProgress = 0;
            Mode_SW.numberKeyPresses = 0;
            break;


        }

  }



}

void Menu1() {
    lcd.clear();
    DBUGS.print(F("LCD MENU1 "));
    DBUGS.println("System Info");
    lcd.setCursor(0,0);
    lcd.print("Vendor Name:");
    lcd.setCursor(0, 1);
    lcd.print(Vendor_Name);

    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

    lcd.print("Product Code:");
    lcd.setCursor(0, 1);
    lcd.print(Product_Code);

    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

    lcd.print("Revision Number:");
    lcd.setCursor(0, 1);
    lcd.print(Revision_Number);

    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

    lcd.print("Meter Name:");
    lcd.setCursor(0, 1);
    lcd.print(meter_name);

    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

    lcd.print("Meter Model:");
    lcd.setCursor(0, 1);
    lcd.print(model);

    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

    lcd.print(F("Manufacturer:"));
    lcd.setCursor(0, 1);
    lcd.print("Microcode");
    lcd.setCursor(0, 2);
    lcd.print("Engineering");

    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

    MicroID.getUniqueIDString(id);
    serial_no = id;
    lcd.print("Serial_no:");
    lcd.setCursor(0, 1);
    lcd.print(serial_no);


    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

    lcd.print("Firmware ver:");
    lcd.setCursor(0, 1);
    lcd.print(firmware_ver);


    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

    lcd.print("MAC Address:");
    lcd.setCursor(0, 1);
    lcd.print(mac_add);


    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);


    lcd.print("Node:");
    lcd.setCursor(0, 1);
    lcd.print(node);


    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

    lcd.print("Node ID:");
    lcd.setCursor(0, 1);
    lcd.print(node_ID);


    ButtonCheck(Set_Button);
    ClearLCD(T_100MSEC);

}

void Menu2() {

    LCD_EnergyData();
    LCD_PrintData();
}

void Menu3() {


}



void SwitchTest(void) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PRESS MODE => ");
    ButtonCheck(Mode_Button);
    lcd.print("PASSED");
    OnOff_Output(RELAY_1, OUTPUT_STATE_ON);


    lcd.setCursor(0, 1);
    lcd.print("PRESS SET => ");
    ButtonCheck(Set_Button);
    lcd.print("PASSED");
    OnOff_Output(RELAY_2, OUTPUT_STATE_ON);

    delay(3000);

    OnOff_Output(RELAY_1, OUTPUT_STATE_OFF);
    OnOff_Output(RELAY_2, OUTPUT_STATE_OFF);
    lcd.clear();

}

void ButtonCheck(int buttonPin) {
    int buttonState = 0;
    while (1) {
        buttonState = digitalRead(buttonPin);
        if (buttonState == HIGH) {

            return;
        }
    }
}

void PeripheralsIdle() {}


void led_wififlash(int ton, int toff) {
    mcp23017.digitalWrite(WIFI_LED, LED_ON);
    delay(ton);
    mcp23017.digitalWrite(WIFI_LED, LED_OFF);
    delay(toff);
}

void led_runflash(int ton, int toff) {
    mcp23017.digitalWrite(RUN_LED, LED_ON);
    delay(ton);
    mcp23017.digitalWrite(RUN_LED, LED_OFF);
    delay(toff);
}

void led_faultflash(uint8_t SelectOutput, int ton, int toff) {
    mcp23017.digitalWrite(FLT_LED, LED_ON);
    delay(ton);
    mcp23017.digitalWrite(FLT_LED, LED_OFF);
    delay(toff);
}

void led_ModbusFlash(int ton, int toff) {
    mcp23017.digitalWrite(MODBUS_LED, LED_ON);
    delay(ton);
    mcp23017.digitalWrite(MODBUS_LED, LED_OFF);
    delay(toff);
}


void TestPeripherals() {
    // Peripherals Test
    PrintUnderline("Peripherals Test");

    InitialiseLCD();    // Initialize LCD

    if (DisableHardwareTest == false) {

        PowerOnLedTest();  // Initialize LED

        ScanI2CBus();

       // SwitchTest();
    }
    // Initialize EEPROM
    //InitializeEEPROM();


}

void PrintTimeStamp() {

    DBUGS.println("Time Stamp : \t" + AUS.dateTime(RFC850));
}

void MonitorPinOnInputChange() {

    uint8_t n = 3;
    uint8_t flagA, flagB;  // MCP23017 INTFA/B registers
    uint8_t capA, capB;    // MCP23017 INTCAPA/B registers
    if (switchDidChange) {
        delay(100);  // allow time for MCP23017 to set interrupt registers
        mcp23017.interruptedBy(flagA, flagB);  // retrieve pin causing interrupt
        mcp23017.clearInterrupts(capA, capB);  // clear interrupt and capture pin states at time of interrupt
        uint8_t pin = 0;  // input pin causing interrupt
        for (pin = 0; pin < 8; pin++) {
            if (bitRead(flagB, pin)) break;
        }


        do {
            DigitalInput[n].Bit.InputReaded = mcp23017.digitalRead(InputPin[n]);

            if (DigitalInput[n].Bit.InputReaded != DigitalInput[n].Bit.InputStatus) {
                if (DigitalInput[n].Bit.InputReaded != DigitalInput[n].Bit.InputVar) {
                    DigitalInput[n].Bit.InputVar = DigitalInput[n].Bit.InputReaded;

                }

                else {

                    DigitalInput[n].Bit.InputVar = DigitalInput[n].Bit.InputReaded;
                    DigitalInput[n].Bit.InputStatus = DigitalInput[n].Bit.InputReaded;

                }


            }
            else {
                DigitalInput[n].Bit.InputVar = DigitalInput[n].Bit.InputStatus;
            }
        } while (++n < 10);


        uint8_t value = bitRead(capB, pin);  // new value of input pin causing interrupt

        switch (pin)
        {
        case 0:
            MCP23017_INT.Bit.IRQ1_Flag = 1;
            break;

        case 1:
            MCP23017_INT.Bit.IRQ0_Flag = 1;
            break;

        case 2:
            MCP23017_INT.Bit.WO_Flag = 1;
            break;

        case 3:
            MCP23017_INT.Bit.CF4_Flag = 1;
            break;

        case 4:
            MCP23017_INT.Bit.CF3_Flag = 1;
            break;

        case 5:
            MCP23017_INT.Bit.CF2_Flag = 1;
            break;

        case 6:
            MCP23017_INT.Bit.CF1_Flag = 1;
            break;

        }

        switchDidChange = false;
    }
}

void ClearLCD(uint16_t lcdtime) {

    TimeOutLCDClear = lcdtime;
    
    while (TimeOutLCDClear != 0) {
        OnOff_Output(RUN_LED, LED_ON);

    }
        OnOff_Output(RUN_LED, LED_ON);
        lcd.clear();
        lcd.setCursor(0, 0);



}

void LEDPulser(uint8_t OutputPin,uint16_t PulseON, uint16_t PulseOFF) {

    TimeOutLEDPulseOn = PulseON;
    
    
    while (TimeOutLEDPulseOn != 0) {
        OnOff_Output(OutputPin, LED_ON);

    }

    TimeOutLEDPulseOff = PulseOFF;

    while (TimeOutLEDPulseOff != 0) {
        OnOff_Output(OutputPin, LED_OFF);

    }




}


void LCD_EnergyData() {

    if (Settings.Bit.DisplayInProgress == 1) {
        if (!pageChanged) {
            LCDCount++;
        }


        Peripheral_Management = LCD_PrintData;

        switch (LCDCount)
        {
        case 1:
            Display.Flag.Voltage = 1;
            break;

        case 2:
            Display.Flag.Current = 1;
            break;

        case 3:
            Display.Flag.CalPower = 1;
            break;

        case 4:
            Display.Flag.ActPower = 1;
            break;

        case 5:
            Display.Flag.ReactPower = 1;
            break;

        case 6:
            Display.Flag.AppPower = 1;
            break;

        case 7:
            Display.Flag.PFactor = 1;
            break;

        case 8:
            Display.Flag.PAngle = 1;
            break;

        case 9:
            Display.Flag.LineFreq = 1;
            break;

        case 10:
            Display.Flag.THData = 1;
            break;

        case 11:
            Display.Flag.ActEnergy_Imp = 1;
            break;

        case 12:
            Display.Flag.ActEnergy_Exp = 1;
            break;

        case 13:
            Display.Flag.ReactEnergy_Imp = 1;
            break;

        case 14:
            Display.Flag.ReactEnergy_Exp = 1;
            break;

        case 15:
            Display.Flag.ApptEnergy = 1;
            break;

        case 16:
            Display.Flag.WiFiState = 1;
            break;


        default:

            LCDCount = 0;
            Peripheral_Management = PeripheralsIdle;
            break;
        }



    }
}


//  Wait data from PC
void WaitDataFromPc(void) {

    char request[12]; // char array as buffer for reading from Serial
    String command;

    if (Settings.Bit.ReadDataInProgress != 0) {
        return;
    }

    while (1) {

        if (Serial.available() > 0) {
            command = Serial.readStringUntil('\n'); //Read input and length
            
            //DBUGS.println(F("/SYNC/"));

            if (command.equals("PC_ENABLED")) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(F("PC Connected "));
                //Settings.Bit.PcUartRequest = 1;

                SendDataToPC();  // Next Step
                Modbus_Poling();

                Serial.flush();
                command = "";
            }

            if (command.equals("Req_End")) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(F("Requested New"));
                Load_Power_Registers();
                ReadPCBTemperature();
                Serial.flush();
                command = "";
                
                if (COM_MODE != "USB") {
                    Settings.Bit.PcUartRequest = 0;
                    break;
                }

            }

            if (command.equals("ConfigSave")) {
                Serial.println(F("/SYNC/"));
                UpdateConfigSerial();

                //if (COM_MODE=="NONE") Settings.Bit.PcUartRequest = 0;

            }

            if (command.equals("Code_Cal1")) {
                Serial.println(F("/SYNC/"));
                SendCallibration1_ToPC();
            }

            if (command.equals("Code_Cal2")) {
                Serial.println(F("/SYNC/"));
                SendCallibration2_ToPC();
            }

            if (command.equals("Code_Cal3")) {
                Serial.println(F("/SYNC/"));
                SendCallibration3_ToPC();
            }

            if (command.equals("Code_Cal4")) {
                Serial.println(F("/SYNC/"));
                SendCallibration4_ToPC();
            }
                


 

            if (command.equals("Reboot")) {
                wifi_disconnect();
                esp_restart();
                break;
            }
                
               

        }
    }

    Settings.Bit.PcUartRequest = 0;





    /*
    while (Serial.available()) {

        if (Serial.available() > 0) {
            int number = Serial.readBytesUntil('\n', request, sizeof(request)); //Read input and length
            request[number] = '\0'; //Terminate the string by adding '\0' at its end
            if (strcmp(request, "PC_ENABLED") == 0) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(F("PC Connected "));
                Settings.Bit.PcUartRequest = 1;

                SendDataToPC();  // Next Step
                request[number] = {};

            }

            if (strcmp(request, "Req_End") == 0) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(F("Requested New"));
                Load_Power_Registers();
                ReadPCBTemperature();

                request[number] = {};
                // ComRequest = PC_Idle;
                //return;
            }

            if (strcmp(request, "PC_Disconnected") == 0) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print(F("PC Disconnected "));
                Serial.println("ACK");
                Settings.Bit.PcUartRequest = 0;


            }

            if (strcmp(request, "ConfigSave") == 0) UpdateConfigSerial();

        }
    }
    */



}
//======================================================================


void UpdateConfigSerial() {

    JsonDocument doc;

    DeserializationError Error = deserializeJson(doc, DBUGS);

    if (Error) {
        DBUGS.println("Invalid JSON Object");
        return;
    }

    DBUGS.println("Valid JSON Object Received");




    const char* _Cal_Page = doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    const char* _COM_MODE = doc["COM_MODE"];
    COM_MODE = String(_COM_MODE);
    Config_State = doc["Config_State"];
    SerialBaud = doc["SerialBaud"];
    WIFi_Mode = doc["WIFi_Mode"];
    const char* _WIFi_State = doc["WIFi_State"];
    WIFi_State = String(_WIFi_State);
    WIFiClient_Mode = doc["WIFiClient_Mode"];
    const char* _WIFi_SSID = doc["WIFiSSID"];
    WIFiSSID = String(_WIFi_SSID);
    const char* _WiFi_Password = doc["WiFiPassword"];
    WiFiPassword = String(_WiFi_Password);
    const char* _ServerIP = doc["Server_IP"];
    Server_IP = String(_ServerIP);
    const char* WiFi_Host_IP = doc["WiFiHost_IP"];
    WiFiHost_IP = String(WiFi_Host_IP);
    WiFiServerPort = doc["WiFiServerPort"];
    const char* _WiFi_Gateway = doc["WiFiGateway"];
    WiFiGateway = String(_WiFi_Gateway);
    const char* WiFi_Subnet = doc["WiFiSubnet"];
    WiFiSubnet = String(WiFi_Subnet);
    const char* DNS_1 = doc["DNS1"];
    DNS1 = String(DNS_1);
    const char* DNS_2 = doc["DNS2"];
    DNS2 = String(DNS_2);
    WiFiAP_Mode = doc["WiFiAP_Mode"];
    const char* _WiFiSoftAP_IP = doc["WiFiSoftAP_IP"];
    WiFiSoftAP_IP = String(_WiFiSoftAP_IP);
    const char* _SoftAP_SSID = doc["SoftAP_SSID"];
    SoftAP_SSID = String(_SoftAP_SSID);
    const char* _SoftAP_Password = doc["SoftAP_Password"];
    SoftAP_Password = String(_SoftAP_Password);
    ETH_Mode = doc["ETH_Mode"];
    const char* _ETHHostIP = doc["ETH_HostIP"];
    ETH_HostIP = String(_ETHHostIP);
    ETH_TCPPort = doc["ETH_TCPPort"];
    MDBUS_Mode = doc["MDBUS_Mode"];
    ModSlave_ID = doc["MDBUS_ID"];
    const char* _MDBusTCPHostIP = doc["MDBus_TCPHostIP"];
    MDBus_TCPHostIP = String(_MDBusTCPHostIP);
    MDBus_TCPPort = doc["MDBus_TCPPort"];
    MDBUS_BAUD = doc["MDBUS_BAUD"];
    PublishInterval = doc["Publish_Interval"];
    const char* _ConfigMode = doc["Config_Mode"];
    Config_Mode = String(_ConfigMode);


    JSONVar settings;

    settings["Page_Name"] = Cal_Page;
    settings["COM_MODE"] = COM_MODE;
    settings["Config_State"] = Config_State;
    settings["SerialBaud"] = SerialBaud;
    settings["WIFi_Mode"] = WIFi_Mode;
    settings["WIFi_State"] = WIFi_State;
    settings["WIFiClient_Mode"] = WIFiClient_Mode;
    settings["WIFiSSID"] = WIFiSSID;
    settings["WiFiPassword"] = WiFiPassword;
    settings["Server_IP"] = Server_IP;
    settings["WiFiHost_IP"] = WiFiHost_IP;
    settings["WiFiServerPort"] = WiFiServerPort;
    settings["WiFiGateway"] = WiFiGateway;
    settings["WiFiSubnet"] = WiFiSubnet;
    settings["DNS1"] = DNS1;
    settings["DNS2"] = DNS2;
    settings["WiFiAP_Mode"] = WiFiAP_Mode;
    settings["WiFiSoftAP_IP"] = WiFiSoftAP_IP;
    settings["SoftAP_SSID"] = SoftAP_SSID;
    settings["SoftAP_Password"] = SoftAP_Password;
    settings["ETH_Mode"] = ETH_Mode;
    settings["ETH_HostIP"] = ETH_HostIP;
    settings["ETH_TCPPort"] = ETH_TCPPort;
    settings["MDBUS_Mode"] = MDBUS_Mode;
    settings["MDBUS_ID"] = ModSlave_ID;
    settings["MDBus_TCPHostIP"] = MDBus_TCPHostIP;
    settings["MDBus_TCPPort"] = MDBus_TCPPort;
    settings["MDBUS_BAUD"] = MDBUS_BAUD;
    settings["Publish_Interval"] = PublishInterval;
    settings["Config_Mode"] = "Saved";

    String jsonString = JSON.stringify(settings);

    if (saveConfig(config_filename)) {
        DBUGS.println("Setup -> Config file saved");
        DBUGS.println("/SAVED/");
        DBUGS.println("/SYNC/");
        delay(1000);


        //if (COM_MODE == "USB") {
        DBUGS.println(jsonString);
        DBUGS.flush();

        //}


    }



}

//===========================================================================================================

void SendCallibration1_ToPC() {

    JsonDocument Doc;

    DeserializationError Error = deserializeJson(Doc, DBUGS);

    if (Error) {
        DBUGS.println("Invalid JSON Object");
        return;
    }

    DBUGS.println("Valid JSON Object Received");

    //Configuration Registers
    const char* _Cal_Page = Doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    MMode0_Reg = Doc["MMode0"];
    MMode1_Reg = Doc["MMode1"];
    PMPGA_Reg = Doc["PMPGA"];
    PStartTh_Reg = Doc["PStartTh"];
    QStartTh_Reg = Doc["QStartTh"];
    SStartTh_Reg = Doc["SStartTh"];
    PPhaseTh_Reg = Doc["PPhaseTh"];
    QPhaseTh_Reg = Doc["QPhaseTh"];
    SPhaseTh_Reg = Doc["SPhaseTh"];


    JSONVar CAL1;

    CAL1["Page_Name"] = Cal_Page;
    CAL1["Trans"] = "CAL1_SUCCESS";
    CAL1["MMode0"] = MMode0_Reg;
    CAL1["MMode1"] = MMode1_Reg;
    CAL1["PMPGA"] = PMPGA_Reg;
    CAL1["PStartTh"] = PStartTh_Reg;
    CAL1["QStartTh"] = QStartTh_Reg;
    CAL1["SStartTh"] = SStartTh_Reg;
    CAL1["PPhaseTh"] = PPhaseTh_Reg;
    CAL1["QPhaseTh"] = QPhaseTh_Reg;
    CAL1["SPhaseTh"] = SPhaseTh_Reg;

    String jsonString = JSON.stringify(CAL1);

    if (saveConfigRegisters(Cali1_filename)) {
        DBUGS.println("Setup -> Calibration_1 file saved");
        
        DBUGS.println("/SAVED/");
        DBUGS.println("/SYNC/");
        delay(1000);


        //if (COM_MODE == "USB") {
        //delay(50);

        //if (COM_MODE == "USB") {
        DBUGS.println(jsonString);
        DBUGS.flush();

    }


}

//===========================================================================================================
void SendCallibration2_ToPC() {

    JsonDocument Doc;

    DeserializationError Error = deserializeJson(Doc, DBUGS);

    if (Error) {
        DBUGS.println("Invalid JSON Object");
        return;
    }

    DBUGS.println("Valid JSON Object Received");



    //Calibration Registers
    const char* _Cal_Page = Doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    CalRegisters[0] = Doc["PoffsetA"];
    CalRegisters[1] = Doc["QoffsetA"];
    CalRegisters[2] = Doc["POffsetB"];
    CalRegisters[3] = Doc["QOffsetB"];
    CalRegisters[4] = Doc["POffsetC"];
    CalRegisters[5] = Doc["QOffsetC"];
    CalRegisters[6] = Doc["GainA"];
    CalRegisters[7] = Doc["PhiA"];
    CalRegisters[8] = Doc["GainB"];
    CalRegisters[9] = Doc["PhiB"];
    CalRegisters[10] = Doc["GainC"];
    CalRegisters[11] = Doc["PhiC"];

    JSONVar CAL2;

    CAL2["Page_Name"] = Cal_Page;
    CAL2["Trans"] = "CAL2_SUCCESS";
    CAL2["PoffsetA"] = CalRegisters[0];
    CAL2["QoffsetA"] = CalRegisters[1];
    CAL2["POffsetB"] = CalRegisters[2];
    CAL2["QOffsetB"] = CalRegisters[3];
    CAL2["POffsetC"] = CalRegisters[4];
    CAL2["QOffsetC"] = CalRegisters[5];
    CAL2["GainA"] = CalRegisters[6];
    CAL2["PhiA"] = CalRegisters[7];
    CAL2["GainB"] = CalRegisters[8];
    CAL2["PhiB"] = CalRegisters[9];
    CAL2["GainC"] = CalRegisters[10];
    CAL2["PhiC"] = CalRegisters[11];

    String jsonString = JSON.stringify(CAL2);

    if (saveCalRegisters(Cali2_filename)) {
        DBUGS.println("Setup -> Calibration_2 file saved");

        DBUGS.println("/SAVED/");
        DBUGS.println("/SYNC/");
        delay(1000);


        //if (COM_MODE == "USB") {
        //delay(50);

        //if (COM_MODE == "USB") {
        DBUGS.println(jsonString);
        DBUGS.flush();

    }

}

//===========================================================================================================
void SendCallibration3_ToPC() {

    JsonDocument Doc;

    DeserializationError Error = deserializeJson(Doc, DBUGS);

    if (Error) {
        DBUGS.println("Invalid JSON Object");
        return;
    }

    DBUGS.println("Valid JSON Object Received");



    //Fundamental/ Harmonic Energy Calibration registers
    const char* _Cal_Page = Doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    HarCalRegisters[0] = Doc["POffsetAF"];
    HarCalRegisters[1] = Doc["POffsetBF"];
    HarCalRegisters[2] = Doc["POffsetCF"];
    HarCalRegisters[3] = Doc["PGainAF"];
    HarCalRegisters[4] = Doc["PGainBF"];
    HarCalRegisters[5] = Doc["PGainCF"];

    JSONVar CAL3;

    CAL3["Page_Name"] = Cal_Page;
    CAL3["Trans"] = "CAL3_SUCCESS";
    CAL3["POffsetAF"] = HarCalRegisters[0];
    CAL3["POffsetBF"] = HarCalRegisters[1];
    CAL3["POffsetCF"] = HarCalRegisters[2];
    CAL3["PGainAF"] = HarCalRegisters[3];
    CAL3["PGainBF"] = HarCalRegisters[4];
    CAL3["PGainCF"] = HarCalRegisters[5];

    String jsonString = JSON.stringify(CAL3);


    if (saveFund_HarCalRegisters(Cali3_filename)) {
        DBUGS.println("Setup -> Calibration_3 file saved");
        //serializeJson(Doc, client);
        DBUGS.println("/SAVED/");
        DBUGS.println("/SYNC/");
        delay(1000);


        //if (COM_MODE == "USB") {
        //delay(50);

        //if (COM_MODE == "USB") {
        DBUGS.println(jsonString);
        DBUGS.flush();

    }

}

//===========================================================================================================
void SendCallibration4_ToPC() {

    JsonDocument Doc;

    DeserializationError Error = deserializeJson(Doc, DBUGS);

    if (Error) {
        DBUGS.println("Invalid JSON Object");
        return;
    }

    DBUGS.println("Valid JSON Object Received");



    //Measurement Calibration
    const char* _Cal_Page = Doc["Page_Name"];
    Cal_Page = String(_Cal_Page);
    MeasurCalRegisters[0] = Doc["UgainA"];
    MeasurCalRegisters[1] = Doc["IgainA"];
    MeasurCalRegisters[2] = Doc["UoffsetA"];
    MeasurCalRegisters[3] = Doc["IoffsetA"];
    MeasurCalRegisters[4] = Doc["UgainB"];
    MeasurCalRegisters[5] = Doc["IgainB"];
    MeasurCalRegisters[6] = Doc["UoffsetB"];
    MeasurCalRegisters[7] = Doc["IoffsetB"];
    MeasurCalRegisters[8] = Doc["UgainC"];
    MeasurCalRegisters[9] = Doc["IgainC"];
    MeasurCalRegisters[10] = Doc["UoffsetC"];
    MeasurCalRegisters[11] = Doc["IoffsetC"];
    MeasurCalRegisters[12] = Doc["IgainN"];
    MeasurCalRegisters[13] = Doc["IoffsetN"];

    JSONVar CAL4;

    CAL4["Page_Name"] = Cal_Page;
    CAL4["Trans"] = "CAL4_SUCCESS";
    CAL4["UgainA"] = MeasurCalRegisters[0];
    CAL4["IgainA"] = MeasurCalRegisters[1];
    CAL4["UoffsetA"] = MeasurCalRegisters[2];
    CAL4["IoffsetA"] = MeasurCalRegisters[3];
    CAL4["UgainB"] = MeasurCalRegisters[4];
    CAL4["IgainB"] = MeasurCalRegisters[5];
    CAL4["UoffsetB"] = MeasurCalRegisters[6];
    CAL4["IoffsetB"] = MeasurCalRegisters[7];
    CAL4["UgainC"] = MeasurCalRegisters[8];
    CAL4["IgainC"] = MeasurCalRegisters[9];
    CAL4["UoffsetC"] = MeasurCalRegisters[10];
    CAL4["IoffsetC"] = MeasurCalRegisters[11];
    CAL4["IgainN"] = MeasurCalRegisters[12];
    CAL4["IoffsetN"] = MeasurCalRegisters[13];

    String jsonString = JSON.stringify(CAL4);

    if (saveMeasureCalRegisters(Cali4_filename)) {
        DBUGS.println("Setup -> Calibration_4 file saved");
        //serializeJson(Doc, client);
        DBUGS.println("/SAVED/");
        DBUGS.println("/SYNC/");
        delay(1000);


        //if (COM_MODE == "USB") {
        //delay(50);

        //if (COM_MODE == "USB") {
        DBUGS.println(jsonString);
        DBUGS.flush();

    }

}
//==========================================================================================================

void SendDataToPC(void) {

    //===========================================================================
//Send Energy Data

    Serial.print("AE1"); Serial.println(PhaA_ForwardActiveEnergy);
    Serial.print("AE2"); Serial.println(PhaA_ReverseActiveEnergy);
    Serial.print("AE3"); Serial.println(PhaA_ForwardReactiveEnergy);
    Serial.print("AE4"); Serial.println(PhaA_ReverseReactiveEnergy);
    Serial.print("AE5"); Serial.println(PhaA_ApparentEnergy);

    Serial.print("BE1"); Serial.println(PhaB_ForwardActiveEnergy);
    Serial.print("BE2"); Serial.println(PhaB_ReverseActiveEnergy);
    Serial.print("BE3"); Serial.println(PhaB_ForwardReactiveEnergy);
    Serial.print("BE4"); Serial.println(PhaB_ReverseReactiveEnergy);
    Serial.print("BE5"); Serial.println(PhaB_ApparentEnergy);

    Serial.print("CE1"); Serial.println(PhaC_ForwardActiveEnergy);
    Serial.print("CE2"); Serial.println(PhaC_ReverseActiveEnergy);
    Serial.print("CE3"); Serial.println(PhaC_ForwardReactiveEnergy);
    Serial.print("CE4"); Serial.println(PhaC_ReverseReactiveEnergy);
    Serial.print("CE5"); Serial.println(PhaC_ApparentEnergy);

    Serial.print("TE1"); Serial.println(TotalForwardActiveEnergy);
    Serial.print("TE2"); Serial.println(TotalReverseActiveEnergy);
    Serial.print("TE3"); Serial.println(TotalForwardReactiveEnergy);
    Serial.print("TE4"); Serial.println(TotalReverseReactiveEnergy);
    Serial.print("TE5"); Serial.println(Total_ArithmeticSum_ApparentEnergy);
    Serial.print("TE6"); Serial.println(VectorSum_TotalApparentEnergy);

    //===========================================================================
    //Send Power Data

    Serial.print("AP1"); Serial.println(PhaA_ActivePower);
    Serial.print("AP2"); Serial.println(PhaA_ReactivePower);
    Serial.print("AP3"); Serial.println(PhaA_apparentpower);
    Serial.print("AP4"); Serial.println(PhaA_activefundamentalpower);
    Serial.print("AP5"); Serial.println(PhaA_powerfactor);
    Serial.print("AP6"); Serial.println(PhaA_activeharmonicpower);
    Serial.print("AP7"); Serial.println(PhaA_voltageRMS);
    Serial.print("AP8"); Serial.println(PhaA_currentRMS);
    Serial.print("AP9"); Serial.println(PhaA_meanphaseangle);
    Serial.print("AP10"); Serial.println(PhaA_voltagephaseangle);

    Serial.print("BP1"); Serial.println(PhaB_ActivePower);
    Serial.print("BP2"); Serial.println(PhaB_ReactivePower);
    Serial.print("BP3"); Serial.println(PhaB_apparentpower);
    Serial.print("BP4"); Serial.println(PhaB_activefundamentalpower);
    Serial.print("BP5"); Serial.println(PhaB_powerfactor);
    Serial.print("BP6"); Serial.println(PhaB_activeharmonicpower);
    Serial.print("BP7"); Serial.println(PhaB_voltageRMS);
    Serial.print("BP8"); Serial.println(PhaB_currentRMS);
    Serial.print("BP9"); Serial.println(PhaB_meanphaseangle);
    Serial.print("BP10"); Serial.println(PhaB_voltagephaseangle);

    Serial.print("CP1"); Serial.println(PhaC_ActivePower);
    Serial.print("CP2"); Serial.println(PhaC_ReactivePower);
    Serial.print("CP3"); Serial.println(PhaC_apparentpower);
    Serial.print("CP4"); Serial.println(PhaC_activefundamentalpower);
    Serial.print("CP5"); Serial.println(PhaC_powerfactor);
    Serial.print("CP6"); Serial.println(PhaC_activeharmonicpower);
    Serial.print("CP7"); Serial.println(PhaC_voltageRMS);
    Serial.print("CP8"); Serial.println(PhaC_currentRMS);
    Serial.print("CP9"); Serial.println(PhaC_meanphaseangle);
    Serial.print("CP10"); Serial.println(PhaC_voltagephaseangle);


    Serial.print("TP1"); Serial.println(Total_PhasesumActivePower);
    Serial.print("TP2"); Serial.println(Total_PhasesumReactivePower);
    Serial.print("TP3"); Serial.println(Total_ArithmeticSumapparentpower);
    Serial.print("TP4"); Serial.println(VectorSum_TotalApparentPower);
    Serial.print("TP5"); Serial.println(Totalpowerfactor);
    Serial.print("TP6"); Serial.println(Totalactivefundamentalpower);
    Serial.print("TP7"); Serial.println(Totalactiveharmonicpower);
    Serial.print("TP8"); Serial.println(Frequency);
    Serial.print("TP9"); Serial.println(LineCurrentCTN);
    Serial.print("TP10"); Serial.println(LineCurrentTotal);
    
    Serial.print("DP1"); Serial.println(Hum);
    Serial.print("DP2"); Serial.println(Temp_C);
    Serial.print("DP3"); Serial.println(Temp_F);

    TextForSms = TextForSms + Temp_C + "," + Hum + ",";
    Serial.println(TextForSms);
    TextForSms = "";

    Settings.Bit.TimerPCTimeout = 0;

}

void PC_Idle(){}