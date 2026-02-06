// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       SM_GE3222M V1.0.ino
    Created:	13/04/2024 2:35:57 PM
    Author:     DESKTOP-4QUALUP\chami
*/

// Define User Types below here or use a .h file
//


// Define Function Prototypes that use User Types below here or use a .h file
//



#include <WebServer.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include "Definitions.h"
#include "WiFi_OTA.h"


// **************** Initialize ATM90E32 or ATM90E36 IC Class ****************
ATM90E3x eic(05); //

MCP23017 mcp23017 = MCP23017(MCP_ADDR);  // instance of the connected MCP23017 IC

// **************** FUNCTIONS AND ROUTINES ****************




Timezone AUS;


// Display BIN from Var
void DisplayBIN16(int var)
{
    for (unsigned int i = 0x8000; i; i >>= 1)
    {
        DBUGS.write(var & i ? '1' : '0');
    }
    DBUGS.print(" ");
} // DisplayBIN16

// Display Hex from Var
void DisplayHEX(unsigned long var, unsigned char numChars)
{
    unsigned long mask = 0x0000000F;
    mask = mask << 4 * (numChars - 1);

    for (unsigned int i = numChars; i > 0; --i)
    {
        DBUGS.print(((var & mask) >> (i - 1) * 4), HEX);
        mask = mask >> 4;
    }
    DBUGS.print(" ");
} // DisplayHEX

// Return Rounded Value to 2 decimals and Remove +/- Noise Threshold.  Default 0.02
float NoiseFilterSquelch(float Value, float Threshold = 0.02, boolean AllowNegative = true, float ValueMax = 0.0)
{
    if (EnableNoiseFilterSquelch == true) // If false, output is raw value
    {
        Value = roundf(Value * 100) / 100; // Round to 2 Decimal Places

        if (Value >= -Threshold && Value <= Threshold) // Window Comparator / Squelch.  Default 0.2
            Value = 0;

        if (AllowNegative == false && Value < 0) // Negative Value Filter.  Default true
            Value = 0;

        if (ValueMax != 0 && Value > ValueMax) // Maximum Filter.  Default 0
            Value = 0;
    }

    return Value;
} // NoiseFilterSquelch

// Map float values to percentages
float MapValues(float x, float DC_Min, float DC_Max, float Percentage_Min, float Percentage_Max)
{
    return (x - DC_Min) * (Percentage_Max - Percentage_Min) / (DC_Max - DC_Min) + Percentage_Min;
}

// Display WiFI Status LCD
void DisplayWiFiStatus()
{
    // Firmware Version / Board Location - Display on OLED
    if (LCD_Enabled == true && EnableLCDLoop == true)

    {

        // WiFi Connection Status - Display on OLED
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(AppAcronym);
        lcd.setCursor(0, 1);
        lcd.print(HardwareRev);
        delay(2000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WiFi AP");

        if (WIFi_State == "ON")
        {

            if (WiFiState == true && WiFi_StaMode == true && WiFi_APMode == false) {

                if (WiFi.status() == WL_CONNECTED)
                {
                    lcd.setCursor(0, 1);
                    lcd.print("SSID: " + String(WiFi.SSID()));
                    lcd.setCursor(0, 2);
                    lcd.print(String(WiFi.RSSI()) + "dBm");
                    lcd.setCursor(0, 3);
                    lcd.print(RSSI_Info(WiFi.RSSI()));
                }

                else
                {
                    lcd.setCursor(0, 1);
                    lcd.print("Not Connected");
                    OnOff_Output(FLT_LED, LED_ON);


                }

                delay(2000);
                lcd.clear();

                lcd.setCursor(0, 0);
                lcd.print("WiFi IP");


                if (WiFi.status() == WL_CONNECTED)
                {
                    IPAddress myAddress = WiFi.localIP();
                    char tmpStr[40];
                    sprintf(tmpStr, "%d.%d.%d.%d", myAddress[0], myAddress[1], myAddress[2], myAddress[3]);
                    ipaddress = tmpStr;


                    lcd.setCursor(0, 1);
                    lcd.print(String(tmpStr));
                    lcd.setCursor(0, 2);
                    lcd.print("SSID => " + String(WiFi.SSID()));
                    
                    
                    lcd.setCursor(0, 3);
                    lcd.print("Host:" + String(WiFi.getHostname()));
                    OnOff_Output(FLT_LED, LED_OFF);
                }

                else {
                    WiFiState = false;
                    OnOff_Output(FLT_LED, LED_ON);
                    wifi_setup();
                }

            }


            //Check AP Mode

            if (WiFiState == true && WiFi_StaMode == false && WiFi_APMode == true) {

                lcd.clear();
                             
               
                 lcd.setCursor(0, 0);
                 lcd.print("WIFI AP MODE");
                 OnOff_Output(FLT_LED, LED_OFF);


                 delay(2000);
                 lcd.clear();

                 lcd.setCursor(5, 0);
                 lcd.print("WiFi IP");

                 IPAddress myIP = WiFi.softAPIP();
                 char tmpStr[40];
                 sprintf(tmpStr, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);

                 ipaddress = tmpStr;

                 lcd.setCursor(1, 3);
                 lcd.print(ipaddress);
                 lcd.setCursor(2, 3);
                 lcd.print("Host:" + String(WiFi.getHostname()));
                 OnOff_Output(FLT_LED, LED_OFF);
               



            }






        }






    }
} // DisplayLCDWiFiStatus


//==================================================================
void IRAM_ATTR mcp23017ChangeDetectedOnPortB() {
    switchDidChange = true;
}

void IRAM_ATTR mode_sw_State()
{

    button_time = millis();
    if (button_time - last_button_time > T_50MSEC)
    {

       // Mode_SW.numberKeyPresses++;
        Mode_SW.pressed = true;


        last_button_time = button_time;

        DigitalInput[0].Bit.InputReaded = digitalRead(InputPin[0]);

        if (DigitalInput[0].Bit.InputReaded == HIGH) {

            DigitalInput[0].Bit.InputReaded = 1;
            DigitalInput[0].Bit.InputStatus = DigitalInput[0].Bit.InputReaded;

        }

        if (DigitalInput[0].Bit.InputReaded == LOW) {

            DigitalInput[0].Bit.InputReaded = 0;
            DigitalInput[0].Bit.InputStatus = DigitalInput[0].Bit.InputReaded;
            Mode_SW.pressed = false;

        }

        if (Mode_SW.numberKeyPresses > 4) Mode_SW.numberKeyPresses = 0;

    }

    else 
    {

            Mode_SW.pressed = false;
            DigitalInput[0].Bit.InputVar = DigitalInput[0].Bit.InputReaded;
            DigitalInput[0].Bit.InputReaded = 0;
            DigitalInput[0].Bit.InputStatus = DigitalInput[0].Bit.InputReaded;

    }
   
    
}

//==================================================================
void IRAM_ATTR set_sw_State()
{


    button_time = millis();
    if (button_time - last_button_time > T_50MSEC)
    {

        Set_SW.numberKeyPresses++;
        Set_SW.pressed = true;



        last_button_time = button_time;
    }



}

//==================================================================

void IRAM_ATTR Timer0_ISR()
{

    portENTER_CRITICAL_ISR(&timerMux);
    Ticks++;
    if (DebouncingInput[0] > 0) { DebouncingInput[0]--; }
    if (DebouncingInput[1] > 0) { DebouncingInput[1]--; }
    if (TimeOutInputPin[0] > 0) { TimeOutInputPin[0]--; }
    if (TimeOutInputPin[1] > 0) { TimeOutInputPin[1]--; }


    if (TimeOutReadData > 0) { TimeOutReadData--; }
    if (TimeOutPrintData > 0) { TimeOutPrintData--; }
    if (TimeOutLCDData > 0) { TimeOutLCDData--; }
    if (TimeOutLCDClear > 0) { TimeOutLCDClear--; }

    if (TimeOutInputPin[0]== 0) { Settings.Bit.TimerFlag1 = 1; }

    if (TimeOutLEDPulseOn > 0) { TimeOutLEDPulseOn--; }
    if (TimeOutLEDPulseOff > 0) { TimeOutLEDPulseOff--; }

    TimeOutLEDPulse++;



    if (TimeOutReadData == 0) {

        Settings.Bit.TimerPCTimeout = 1;

    }




    if (Ticks > 15000) {

       // Settings.Bit.TimerFlag1 = 1;
        Ticks = 0;
    }

    portEXIT_CRITICAL_ISR(&timerMux);





}
//==================================================================


//Setup Timer0 configurations
void Setup_Timer0()
{
    // ESP32 Arduino core v3.x timer API changed (no prescaler/countUp parameters).
    // We run the hardware timer at 1 MHz so 1 tick = 1 µs, then set the alarm for 1000 ticks = 1 ms.
    timer = timerBegin(1000000);                 // 1,000,000 Hz
    timerAttachInterrupt(timer, &Timer0_ISR);    // ISR callback
    timerAlarm(timer, TimerTicks, true, 0);      // alarm value (ticks), auto-reload, reload count (0 = unlimited)
    timerStart(timer);
}


//This function configures the MCP23017 interrupt system and attaches an interrupt service routine.
void configureInterrupts() {
    // Configure MCP23017 interrupts
    mcp23017.interruptMode(MCP23017InterruptMode::Separated);  // INTA and INTB act independently
    mcp23017.interrupt(MCP23017Port::B, CHANGE);  // trigger an interrupt when an input pin CHANGE is detected on PORTB
    // Set up interrupt connection and attach interrupt service routine
    mcp23017.clearInterrupts();  // reset interrupt system
    pinMode(MCP23017_INTB, INPUT_PULLUP);  // utilize microprocessor's internal pull-up resistor
    attachInterrupt(digitalPinToInterrupt(MCP23017_INTB), mcp23017ChangeDetectedOnPortB, FALLING);  // INTB is active LOW
    pinMode(Mode_SW.PIN, INPUT_PULLUP);
    attachInterrupt(Mode_SW.PIN, mode_sw_State, RISING);
    pinMode(Set_SW.PIN, INPUT_PULLUP);
    attachInterrupt(Set_SW.PIN, set_sw_State, RISING);

}
// Define Functions below here or use other .ino or cpp files
//


// The setup() function runs once each time the micro-controller starts
void setup()
{

    Setup_Timer0(); //Setup Timer
// Stabalise
    delay(250);


   eic.DebugIdeMonitor(true, BAUD_115200, SERIAL_8N1);

   //delay(50);
   
       
   //CallibrationSave();

   LoadConfig();

   
   if (COM_MODE == "TCP" || COM_MODE == "USB/TCP") {    //
       Setup_Ethernet();
   }


    ConfigureBoard();   // Config board I/O
    TestPeripherals();  //Test Peripheral Hardware

   LoadCallibration();
   PrintCallibrationSettings();

    
    

    //----------------------------------------------- 
//  Sets State Machine

   //=================================================
   //if (COM_MODE == "TCP" || COM_MODE == "USB/TCP") {
   //    ComRequest1 = Ethernetloop;
   //    ComRequest2 = WaitDataFromPc;
   //    Settings.Bit.PcUartRequest = 1;
   //}

   if (COM_MODE == "TCP") {
       ComRequest1 = Ethernetloop;
       ComRequest2 = StateIdele2;
       Settings.Bit.PcUartRequest = 1;
   }

 //  if (COM_MODE == "USB" || COM_MODE == "USB/TCP"){
 //      ComRequest1 = WaitDataFromPc;
 //      ComRequest2 = Ethernetloop;
 //      Settings.Bit.PcUartRequest = 1;
 //}

   if (COM_MODE == "USB") {
       ComRequest1 = WaitDataFromPc;
       ComRequest2 = StateIdele2;
       Settings.Bit.PcUartRequest = 1;
   }
//=================================================
   //if (COM_MODE == "NONE") {
   //    ComRequest1 = StateIdele1;
   //    ComRequest2 = StateIdele2;
   //    Settings.Bit.PcUartRequest = 0;
   //}

//if (COM_MODE == "USB") {
//    ComRequest = WaitDataFromPc;
//}
   
   SetBoardConfigure = ConfigIdle;   //Setup Board Configurations
   Power_Management = ReadDataIdle;
   InputP1_Management = InputP1_Idle;
   InputP2_Management = InputP2_Idle;
   Peripheral_Management = PeripheralsIdle;
    //----------------------------------------------- 

    TimeOutReadData = T_2SEC;
    TimeOutPrintData = T_10SEC;
    TimeOutLCDData = T_5SEC;
    TimeOutLEDPulse = 0;
    Settings.Bit.TimerPCTimeout = 0;


    //-----------------------------------------------
//  Prints sketch revision
    DBUGS.println("");

    // Application Info
    DBUGS.println("==========================================================================================");
    DBUGS.println(AppName);
    DBUGS.println("==========================================================================================");


    ReadStringCmd_FLASH((uint8_t*)STR_SKETCH_REV, strlen(STR_SKETCH_REV), FALSE, FALSE);
    DBUGS.println(SKETCH_SM_GE3222M_REV);
    DBUGS.println("");

    ReadStringCmd_FLASH((uint8_t*)STR_REV, strlen(STR_REV), FALSE, FALSE);
    DBUGS.println(HardwareRev);
    DBUGS.println("-----------------------------------------------------------------------------------------");


    DBUGS.println("SM-GE32222M Hardware Setup and Power-Up Test");

    DBUGS.println("==========================================================================================");



    // Initialise WiFi 

    // Initialise WiFi and WebServer/OTA
    if (WIFi_Mode == true)
    {
        // Initial LED on
        led_wififlash(1000, 100);

        if (WiFiState == false) {
            PrintUnderline("Software Optons");
            wifi_setup();

            

        }


        if (WiFiState == true && WiFi_StaMode == true && WiFi_APMode == false) {

            if (WiFi.status() == WL_CONNECTED)
            {
                
                OnOff_Output(FLT_LED, LED_OFF);
                DBUGS.println("WiFi is Enabled");
                led_wififlash(50, 50);
                lcd.clear();
                lcd.setCursor(0, 0);

                lcd.print("WiFi => CONNECTED");
                delay(2500);
            }
        }

        if (WiFiState == true && WiFi_APMode == true && WiFi_StaMode == false) {

            OnOff_Output(FLT_LED, LED_OFF);
            OnOff_Output(WIFI_LED, LED_ON);
            DBUGS.println("WiFi AP Mode ENABLED");
            lcd.clear();
            lcd.setCursor(0, 0);

            lcd.print("WiFi => CONNECTED");
        }
        
       
        // WiFi Integration Status


        // DisplayWiFiStatus
        DisplayWiFiStatus();

        Setup_Webserver();


        //InitialiseWebServer();
       // InitialiseMQTT();
       // InitialiseThingSpeak();
    }
    else
    {
        DBUGS.println("WiFi Force Disabled");
        lcd.print("Disabled");
    }




    /*
        // MQTT Integration Status
    if (EnableMQTT == true && WiFi.status() == WL_CONNECTED)
    {
        DBUGS.println("MQTT Enabled - Register Values Will be Published");
    }
    else
    {
        DBUGS.println("MQTT Publishing is Disabled");
    }

    // ThingSpeak Integration Status
    if (EnableThingSpeak == true && WiFi.status() == WL_CONNECTED)
    {
        DBUGS.println("ThingSpeak Enabled - Register Values Will be Published");
    }
    else
    {
        DBUGS.println("ThingSpeak Publishing is Disabled");
    }
    */



    // Check DCV_IN
    CheckDCVINVoltage();

  //===================================================================  
    //    waitForSync();
    //// 
    ////printLocalTime();
    //AUS.setPosix("AEST+2:00");
    //PrintTimeStamp();

      //===================================================================

    

    

/*
#ifdef ENABLE_WDT
    DBUGS.println("Watchdog timer is enabled.");
    feedLoopWDT();
#endif
*/


    //Setup_Webserver();


    // ****************  Initialise the ATM90E3x & Pass related calibrations to its library ****************
    //DeviceInit();

    Load_Reg_Callibration();
    //DisplayPower_Data(true);

    EEPROM_Update_ManufactureInfo();
    EEPROM_READ_ManufactureInfo();


    Setup_Modbus();

    



}   //End Setup

// Add the main program code into the continuous loop() function
void loop()
{


    LEDPulser(RUN_LED, T_1SEC, T_1SEC);


  

        DebouncingInputPin();
        
        SetBoardConfigure();
        InputP1_Management();
        InputP2_Management();
        Power_Management();
        Peripheral_Management();


        if (Settings.Bit.ReadDataInProgress == 0) {


            if (Serial.available() > 0 && COM_MODE != "TCP") {  //COM_MODE != "TCP"
                ComRequest1 = WaitDataFromPc;
                ComRequest2 = StateIdele2;
                DBUGS.println(F("/SYNC/"));
                //ComRequest1();
            }

            if (!Serial.available() && COM_MODE == "NONE") {
                ComRequest1 = StateIdele1;
                ComRequest2 = StateIdele2;
                Settings.Bit.PcUartRequest = 0;
            }

            //while (Settings.Bit.PcUartRequest == 1) {
            //WaitDataFromPc();
                ComRequest1();
                ComRequest2();

            //}

            //if (COM_MODE == "TCP") {
            //    Ethernetloop();
            //}



            if (COM_MODE == "NONE") {
                Settings.Bit.PcUartRequest = 0;
            }
             
        }




    if (pageChanged && (Settings.Bit.SettingInProgress == 1)) {

        InputP1_Management = LCD_MenuHandler;    //  Button released

    }



    if (pageChanged == false) {

        events();


        if (millis() > mem_info_update) {
            mem_info_update = millis() + 2000;
            uint32_t current = ESP.getFreeHeap();
            int32_t diff = (int32_t)(last_mem - current);
            if (diff != 0) {
                DBUGS.printf("Free memory %u - diff %d %d\n", current, diff, start_mem - current);
                last_mem = current;
            }
        }


        if (WIFi_Mode == true) {
            wifi_loop();
            Webserver_loop();

        }


        

        if (Settings.Bit.PcUartRequest != 1) {
            if ((Settings.Bit.TimerPCTimeout == 1) && (Settings.Bit.ReadDataInProgress == 0)) {
                TimeOutReadData = T_5SEC;
                Power_Management = Load_Power_Registers;   //  Set state machine to start to read data from MCP39F511
                Modbus_Poling();
                //Peripheral_Management = SendDataToPC;
                


            }



            if ((TimeOutPrintData == 0) && (Settings.Bit.ReadDataInProgress == 0)) {
                TimeOutPrintData = T_10SEC; // T_10SEC;

                //if (Settings.Bit.DisplayInProgress == 0) {
                DBUGS.println("\n\n");
                DisplayPower_Data(true);
                EEPROM_Update_Power_Registers();
                EEPROM_Updtate_DHT_DATA(Temp_C, Temp_F, Hum, hic, hif);
                EEPROM_Read_DHT_DATA();
                EEPROM_READ_Power_Registers();
                // }

            }

            if ((TimeOutLCDData == 0) && (Settings.Bit.ReadDataInProgress == 0)) {
                TimeOutLCDData = T_5SEC; // T_10SEC;

                LCD_EnergyData();

            }



        }



        String sDIR = "";

        // Basic Handler for WebServer
        //CheckOTA();



        // Simply DisplayRegisters on Loop if Enabled
        if (EnableBasicLoop == true)
        {
            DisplayPower_Data(true);
            DBUGS.println("- - - / / - - -\n");
        }


        // Display Values to LCD Display
        if (EnableLCDLoop == true || LCD_Enabled == true)
        {

            DisplayPower_Data(false); // Refresh Values and Display.  Default false = Mute Expanded Info to Serial

            // Update the LCD Display
            lcd.clear();


            if (LineVoltageAverage > 100 || LineVoltage1 > 100) // Arbitrary Value
            {

                lcd.setCursor(0, 0);

                // Reset Counter
                if (LCDCount >= 3)
                    LCDCount = 0;

                LCDCount++;

                // Check Active Power and Display on Each Channel
                lcd.printf("%d: ", LCDCount);

                switch (LCDCount)
                {
                case 1:
                    if (ActivePowerImportCT1 > 0)
                    {
                        lcd.printf("%.0f W", ActivePowerImportCT1);
                        sDIR = "Impt";
                    }
                    else if (ActivePowerExportCT1 > 0)
                    {
                        lcd.printf("%.0f W", ActivePowerExportCT1);
                        sDIR = "Expt";
                    }
                    else
                    {
                        lcd.printf("0 W");
                        sDIR = "Zero";
                    }
                    break;

                case 2:
                    if (ActivePowerImportCT2 > 0)
                    {
                        lcd.printf("%.0f W", ActivePowerImportCT2);
                        sDIR = "Impt";
                    }
                    else if (ActivePowerExportCT2 > 0)
                    {
                        lcd.printf("%.0f W", ActivePowerExportCT2);
                        sDIR = "Expt";
                    }
                    else
                    {
                        lcd.printf("0 W");
                        sDIR = "Zero";
                    }
                    break;

                case 3:
                    if (ActivePowerImportCT3 > 0)
                    {
                        lcd.printf("%.0f W", ActivePowerImportCT3);
                        sDIR = "Impt";
                    }
                    else if (ActivePowerExportCT3 > 0)
                    {
                        lcd.printf("%.0f W", ActivePowerExportCT3);
                        sDIR = "Expt";
                    }
                    else
                    {
                        lcd.printf("0 W");
                        sDIR = "Zero";
                    }
                    break;
                }

                // Header
                lcd.setCursor(0, 0);
                lcd.print(LineVoltage1, 0);
                lcd.println(" V " + sDIR);
            }
            else
            {
                // Houston, We may have a probem
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Please Chk");
                lcd.setCursor(0, 1);
                lcd.print("Line Volts");


            }

        }

        Flash_LED(RUN_LED, 50, 1000, 1);

        PrintTimeStamp();

        

    }

   






    /*
    // Publish Values to Domoticz (Set WiFi and Indexes)
    if (EnableDomoticz == true && WiFi.status() == WL_CONNECTED)
    {
        DisplayRegisters(false); // Refresh Values and Display.  Default false = Mute Expanded Info to Serial
        PublishDomoticzValues(); // Publish Values to Domoticz
    }



        // Publish Values to MQTT (Set WiFi and Indexes)
    if (EnableMQTT == true && WiFi.status() == WL_CONNECTED)
    {
        DisplayRegisters(false); // Refresh Values and Display.  Default false = Mute Expanded Info to Serial
        PublishMQTTValues();     // Publish Values to MQTT
    }

    // Publish Values to ThingSpeak (Set WiFi and Indexes)
    if (EnableThingSpeak == true && WiFi.status() == WL_CONNECTED)
    {
        DisplayRegisters(false);   // Refresh Values and Display.  Default false = Mute Expanded Info to Serial
        PublishThingSpeakValues(); // Publish Values to ThingSpeak
    }
    */


    // Heatbeat LED with loop delay 1Sec




       

}   //End Loop

void StateIdele1(void){}
void StateIdele2(void) {}



