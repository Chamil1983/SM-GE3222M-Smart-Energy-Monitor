## SM Energy ESP32 based ATM90E36A Arduino Programmable Smart Energy Monitor Model SM-GE3222M


SM Energy Overview

![Display-Type-B](https://github.com/Chamil1983/SM-GE3222M-Smart-Energy-Monitor/blob/main/Pictures/IMG_3.jpg?raw=true)

![Display-Type-B](https://github.com/Chamil1983/SM-GE3222M-Smart-Energy-Monitor/blob/main/Pictures/IMG_4798.jpg?raw=true)

![Display-Type-B](https://github.com/Chamil1983/SM-GE3222M-Smart-Energy-Monitor/blob/main/Pictures/IMG_4800.jpg?raw=true)

**SM-GE3222M Smart Power/Energy Monitor Device Overview**

**SM-GE3222M Main Board**
![Display-Type-B](https://github.com/Chamil1983/SM-GE3222M-Smart-Energy-Monitor/blob/main/Pictures/Main_Board.jpg?raw=true)

**SM-GE3222M Front LCD Board**
![Display-Type-B](https://github.com/Chamil1983/SM-GE3222M-Smart-Energy-Monitor/blob/main/Pictures/Front_Panel.jpg?raw=true)


**The SM Energy GE3222M Smart Energy Monitor has the main features:**

The board layout is designed to be a flexible dual-purpose.

ESP32 WROOM, with Internal Antenna, or UF.L connector for external antenna
ATM90E32 or ATM90E36 Energy ICs


ATM90E36 (Energy Monitor) Metering Features:

- ESP32 WROOM 32 Dev Module - Wireless and Bluetooth
- **Compact** and **Flexible Design** SDK Board
- All connections are **low voltage**, for **safe operation**
- **Easy to interface**
 - *Optional* **ATM90E** Series
	 - **ATM90E32** Energy Monitor 
		 - 3 x Phases Line CT Clamp Input  
			 -  Example YHDC SCT013000
		 - 3 x Voltage Inputs (AC RMS)
		 - Power Modes Settings
	 - **ATM90E36** Energy Monitor 
		 - 3 x Phases Line CT Clamp Input 
			 -  Example YHDC SCT013000
		 - 3 x Voltage Inputs (AC RMS) 
		 - 1 x Phase Neutral CT Clamp
		 - DMA Mode (Logging via SPI)
		 - Power Modes Settings (J1 and J2 Jumpers)
		 - DMA/SPI selection option via Jumper J3

 - **24C64** EEPROM (I2C)
	 - Parameter Settings
	 - Logging
- I2C 20X4 LCD Module 
- Onboard **3V3 DC SMPS** Power Supply
- Power Supply Requirement is 9 to 12VDC
- Current Clamp Input (Measuring Current up to 100A with CT Clamps Below)
**Compatible CT clamps (20A/25mA SCT-006, 30A/1V SCT-013-030 (JP1,JP2 and JP3 jumper Should be Open) ,50A/1V SCT-013-050 (JP1,JP2 and JP3   jumper Should be Open), 80A/26.6mA SCT-010,100A/50mA SCT-013-000,120A/40mA: SCT-016,200A/100mA SCT-024, 200A/50mA SCT-024)**
- AC Low Voltage Input (Voltage < 250V)

- **Arduino ESP32 Flashing and Programming Compatibility**
 - Arduino **ESP32 Compatible** USB Interface
	 - On-board USB Type B Socket
	 - Flashing/Programming (Auto Programming, No need to press BOOT button)
   - Meter designed to Auto/Manual program and serial communication through a USB interface.
	 - Select Board "ESP32 Dev Module" in Arduino IDE to Programming
	 - Debugging
 - On Board **DHT22** Temperature and Humidity Sensor
- 2 OPTO Isolated Digital Output (VCC Supply < 80V DC>)
**Meter Active and Reactive Energy pulse**
- ESP32 BOOT Button (Optional for Manual upload)
- Reset Button
- **MODBUS and WiFi Connectivity**
- Ethernet Connector (Ethernet/MODBUS can be configured in programming or Meter Sofware)
- 2 X User GPIO (SET and MODE User Programmable Button)
- 4 Meter status LEDs and Power LED. 
- Size 180 x 140mm x 35mm
    
**Remember!**
- Our SDKs for the ESP32 Dev module are fully compatible with Arduino IDE
- Set the BOARD to ESP32 Dev Module, 'WEMOS D1 MINI ESP32' DEV Module (or similar).
- You can also set the BAUD rate to 921600 to speed up flashing.
- The SDK does need external power to flash. 
- Fully tested versions of this board are available. 
***You must provide an external 12V DC to power up the Energy Monitor.*
*You will need to provide a CT Current Clamp.  Ideally YHDC SCT-013-000***

All test code is OPEN SOURCE and although it is not intended for real-world use, it may be freely used, or modified as needed.  It is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See Example Code https://github.com/Chamil1983/SM-GE3222M-Smart-Energy-Monitor/blob/main/Examples/


## **Further Information**

Additional information, and other technical details on this project, may be found in the related repository pages.

**Repository Folders**

 - **Code** *(Code examples for Arduino  IDE and Visual Studio)*
 -  **Datasheets and Information** *(Component Datasheets, Photos and Technical Documentation)*
 - **Certification** *(Related Repository Project or Part, Certification Information)*

**Repository Tabs**

 - **Wiki** *(Related Repository Wiki pages and Technical User Information)*
 - **Discussions** *(Related Repository User Discussion Forum)*
 - **Issues** *(Related Repository Technical Issues and Fixes)*


**To buy this product please go to the link below;**
https://www.ebay.com.au/itm/364952547719

We value our Customers, Users of our designs, and STEM Communities, all over the World. Should you have any other questions, or feedback to share with others, please feel free to:

***Chamil Vithanage, Microcode Embedded Solutions, Australia.***

Electronics Engineer | Software Developer | R&D Support | RF Engineering | Product Certification and Testing 

