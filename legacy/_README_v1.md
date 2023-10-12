# IoT Water Monitoring and Conservation
  This Arduino project code aims to efficiently monitor and help conserve water usage in real-time. The code runs on an ESP32 WiFi board and utilizes sensors, modules and devices to track the flow and turbidity of water supply, as well as maintaining the water level of a water tank or reservoir within a predetermined range. The data is logged to an SD card and the log files are made available on a webpage through a mDNS web server for later analysis.

  The project is a useful and practical tool for anyone who wants to monitor and manage their water usage more effectively. By implementing this system, individuals and organizations can positively impact the environment. Whether you are a homeowner, farmer, business owner, or environmentalist, this project can help save water, reduce waste, save utility bills, and assist decision-making.

## Collaborators
  * [Nathaniel Klobodu](https://github.com/nkklobodu)
  * [Jeffrey Asiedu-Brako](https://github.com/ziegler121)

## Features
  * Monitors water level using an ultrasonic distance sensor and turns the pump on or off as needed to maintain the water level within a certain range
  * Logs data to an SD card, including date and time stamps, turbidity, and total water flow
  * Provides a web interface for viewing the data log and downloading data files


## Hardware
  * DOIT ESP32 DevKit V1 board
  * HC-SR04 ultrasonic sensor
  * YF-S201C flow sensor
  * Turbidity sensor module
  * SD card module and SD card
  * DS3231 RTC module
  * DC water pump
  * Relay module


## Code Structure
  The code includes several libraries for interacting with the sensors and hardware components, including `WiFi.h`, `ESP32WebServer.h`, `ESPmDNS.h`, `Wire.h`, `SPI.h`, and `SD.h`. It also uses the RTClib library, `RTClib.h` for handling real-time clock functionality.

  The code defines several functions for handling different tasks, including `pulse()` for measuring the flow rate, `fill()` for controlling the pump and maintaining the water level, `logData()` for logging data to the SD card, and `sdDir()` for displaying the data log files on the SD card to a webpage from an mDNS web server.

  There are also several global variables defined for storing data and configuring the code's behavior, such as `pulse_freq`, `flow`, and `turbidity`.


## Functionality
  The code uses the HC-SR04 ultrasonic sensor to measure the distance from the sensor to the water surface, which is then converted to a water level in centimeters. If the water level falls below a certain threshold, the pump is turned on to fill the tank. If the water level exceeds another threshold, the pump is turned off to prevent overflow.

  The YF-S201C flow sensor is used to measure the total flow rate of water per day in liters. This data is collected using an interrupt-based pulse counting method, which allows the code to accurately measure the flow rate even while performing other tasks.
  
  The turbidity sensor is used to measure the turbidity of the water, which is a measure of the amount of suspended particles in the water. The turbidity data is then logged to the SD card along with the total flow data.

  The data is logged to the SD card at a specified interval, and the log files can be accessed through the web interface provided by the code. The web interface also allows for downloading the data files.

## Schematics and Circuit Board
<img src="/assets/images/schematic.png" alt = "schematic" width="800">
<img src="/assets/images/circuit_board.png" alt = "circuit board" width="426" height="198">


## Files and Directories
  The **IoT_Water_Monitoring_and_Conservation** directory contains:
  * **HTML.h**: a header file containing HTML code for the web server
  * **IoT_Water_Monitoring_and_Conservation.ino**: the main code which calls the necessary functions
  * **other_functions.ino**: contains functions which perform physical actions
  * **web_page_functions.ino**: contains functions which perform webpage actions

  The **images** directory contains images linked in the **README.md**


## Setup
  1. Connect the hardware as follows:
     | Hardware                       | Connect from device | Connect to ESP32 |
     | --------------                 | ------------------- | ---------------- |
     | **HC-SR04 Ultrasonic**         | VCC                 | 5V               |
     |                                | GND                 | GND              |
     |                                | TRIG                | GPIO25           |
     |                                | ECHO                | GPIO26           |
     | **YF-S201C Flow sensor**       | VCC                 | 5V               |
     |                                | GND                 | GND              |
     |                                | OUT                 | GPIO32           |
     | **Turbidity sensor**           | VCC                 | 3V3              |
     |                                | GND                 | GND              |
     |                                | OUT                 | GPIO35           |
     | **SD card module**             | VCC                 | 3V3              |
     |                                | GND                 | GND              |
     |                                | MOSI                | GPIO23           |
     |                                | MISO                | GPIO19           |
     |                                | SCK                 | GPIO18           |
     |                                | CS                  | GPIO5            |
     | **DS3231 RTC module**          | VCC                 | 3V3              |
     |                                | GND                 | GND              |
     |                                | SDA                 | GPIO21           |
     |                                | SCL                 | GPIO22           |
     |                                | SQW                 | X                |
     |                                | 32K                 | X                |
     | **Relay module**               | VCC                 | 5V               |
     |                                | GND                 | GND              |
     |                                | S                   | GPIO33           |

  The barebones relay with a transistor and a diode can also be created using [this diagram](/assets/images/relay.png)
 
  2. Perform other non circuit connections such as the pump and tube hose connections.
  3. Install the Arduino IDE software if necessary from the [Arduino website](https://www.arduino.cc/en/software/).
  4. Setup the Arduino IDE to communicate with the ESP32 board with [this article](https://circuitdigest.com/microcontroller-projects/programming-esp32-with-arduino-ide) if necessary.
  5. Connect the ESP32 board to your computer using and micro USB to USB cable.
  6. Open the **IoT Water Monitoring and Conservation** sketch in its folder in Arduino IDE and select the correct board and port from the Tools menu.
  7. Upload the sketch.


## Additional Information
  * The code creates a WiFi access point called "Water" with password "12345678". Connect to this access point to access the web server and download the data stored on the SD card. This can be changed by replacing `Water` and `12345678` in the code with your preferred SSID and password respectively:
    ```c++
    // WiFi network settings
      const char* ssid = "Water";
      const char* pass = "12345678";
    ```
  * The code uses the `ESP32WebServer` library to create a simple web server for displaying the contents of the SD card. Use a web browser to navigate to [http://datalog.local/](http://datalog.local/) to access the web interface. The server name can be changed by replacing `datalog` with your preferred server name:
    ```c++
    //define server name
      #define servername "datalog"
    ```
  * The code logs data to the SD card at a predetermined interval of 10 seconds. The data is stored in two separate files, one for turbidity data and one for flow data.
  * The data log files are named "/DATA/QUALITY.CSV" and "/DATA/USAGE.CSV" for the turbidity and consumption data, respectively.
  * The downloaded data is in CSV format which can be analyzed in a spreadsheet application like Microsoft Excel.
  * The code is currently set up to use the DOIT ESP32 DevKit V1 board, but it should work with other compatible boards as well. Some minor modifications may be necessary for different hardware configurations.


## Future Plans
  * Add support for additional sensors, such as a pH sensor or temperature sensor for more comprehensive analysis of water data.
  * Implement automatic notifications or alerts via email or sms when the water level falls below a certain threshold or when the flow or usage exceeds a certain limit.
  * Add a user app or web interface to allow users to:
    - configure and control the system by setting custom thresholds, data logging frequency, and schedule automatic tank filling.
    - visualize the current data with charts and graphs without the need to download it.
  * Integrate the system with a cloud platform to enable configuring the system and monitoring water usage and quality from anywhere in the world.
  * Optimize the code for better performance and reliability. For example, optimizing the code for faster data collection and processing, or reducing the code size to save memory on the ESP32 board.


## Road Map
- [ ] Add support for additional sensors
- [ ] Implement automatic notifications or alerts
- [ ] Add a user app or web interface
- [ ] Integrate the system with a cloud platform
- [ ] Optimize the code


## Contributions
  We welcome contributions to this project. If you have an idea for a new feature or have found a bug, please open an issue in the repository.

