# Getting Started with SM Energy GE3222M

This guide will help you set up and get started with your SM Energy GE3222M Smart Energy Monitor. Follow these steps to successfully install, configure, and begin monitoring your energy usage.

## What's in the Box

- SM Energy GE3222M board
- USB cable (optional)
- Documentation

## What You'll Need

- 9-12V DC power supply with 2.1mm barrel jack
- CT clamps (e.g., YHDC SCT-013-000)
- USB-B cable for programming
- Arduino IDE installed on your computer
- ESP32 board package installed in Arduino IDE

## Hardware Setup

### Step 1: Connect CT Clamps

1. Identify the CT clamp input terminals on the SM Energy GE3222M board.
2. Connect your CT clamps to the appropriate terminals:
   - CT1 for Phase A
   - CT2 for Phase B
   - CT3 for Phase C
   - CTN for Neutral (only for ATM90E36)

> **IMPORTANT SAFETY WARNING**: Never connect CT clamps to live conductors while the other end is disconnected. Always connect the CT clamp to the energy monitor first, then attach it to the conductor.

### Step 2: Connect Voltage References

For voltage monitoring, you'll need to connect voltage reference inputs:

1. Connect L1, L2, L3 inputs to the appropriate phases through suitable fuses.
2. Connect N to neutral.

> **SAFETY WARNING**: The voltage inputs should be connected through appropriate fuses and isolation. Although the board uses isolation for safety, all electrical connections should be made by a qualified electrician in compliance with local regulations.

### Step 3: Power the Board

1. Connect the 9-12V DC power supply to the power jack on the board.
2. The power LED should illuminate, indicating the board is powered.

### Step 4: Connect for Programming

1. Connect the USB-B cable to the onboard USB connector.
2. Connect the other end to your computer.

## Software Setup

### Step 1: Install Arduino IDE

1. Download and install the latest Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software).
2. Launch the Arduino IDE.

### Step 2: Install ESP32 Board Package

1. Go to **File > Preferences**.
2. In the "Additional Board Manager URLs" field, add:

https://dl.espressif.com/dl/package_esp32_index.json

3. Click "OK".
4. Go to **Tools > Board > Boards Manager**.
5. Search for "ESP32" and install the "ESP32 by Espressif Systems".

### Step 3: Install Required Libraries

Install the following libraries from **Sketch > Include Library > Manage Libraries**:

1. Search for and install "ATM90E32_Arduino" library
2. Install "DHT sensor library" for the onboard DHT22 sensor

### Step 4: Select Board Configuration

1. Connect the SM Energy GE3222M board to your computer via USB.
2. In Arduino IDE, select:
- **Tools > Board > ESP32 Arduino > ESP32 Dev Module**
- **Tools > Port > [Select the COM port of your board]**
- **Tools > Upload Speed > 921600** (for faster uploads)

### Step 5: Upload Example Code

1. Go to **File > Examples > ATM90E32_Arduino > BasicEnergyMonitor**.
2. Review and modify the code as needed (you may need to adjust CS_PIN and other settings).
3. Click the "Upload" button to compile and upload the sketch to your board.
4. Open the Serial Monitor (**Tools > Serial Monitor**) and set the baud rate to 115200 to view the output.

## Configuration and Calibration

For accurate measurements, your SM Energy GE3222M may need calibration. Follow these steps:

1. Upload the calibration sketch (Examples > ATM90E32_Arduino > Calibration).
2. Connect a known load (e.g., a 100W light bulb).
3. Follow the instructions in the sketch to calibrate voltage and current readings.
4. Save the calibration values for use in your final sketch.

## Using the Web Interface

The SM Energy GE3222M can be configured to provide a web interface for monitoring:

1. Upload the WiFiWebServer example sketch (found in this repository).
2. Modify the WiFi credentials in the sketch.
3. Upload the sketch to your board.
4. Open the Serial Monitor to see the assigned IP address.
5. Navigate to that IP address in a web browser to view the energy monitoring dashboard.

## Troubleshooting

### No Power LED
- Check the power supply is properly connected and providing 9-12V DC.
- Check the polarity of the power supply.

### Can't Upload Sketches
- Ensure the correct board and port are selected in Arduino IDE.
- Try pressing the BOOT button while initiating the upload.
- Check that the USB cable is a data cable, not just a charging cable.

### No Readings or Incorrect Readings
- Verify CT clamps are properly connected.
- Check that CT clamps are oriented correctly (arrow pointing toward the load).
- Ensure voltage reference connections are correct.
- Check the calibration values in your sketch.

### WiFi Connection Issues
- Verify the WiFi credentials in your sketch.
- Ensure the WiFi network is within range.
- Try reducing the distance between the board and your WiFi router.

## Next Steps

Once your SM Energy GE3222M is up and running, you can:

1. Explore advanced examples in the Examples folder.
2. Integrate with home automation systems like Home Assistant or Node-RED.
3. Set up data logging to track energy usage over time.
4. Develop custom applications using the API.

For more information, visit the [GitHub repository](https://github.com/Chamil1983/SM-GE3222M-Smart-Energy-Monitor.git) or contact Microcode Embedded Solutions, Australia.