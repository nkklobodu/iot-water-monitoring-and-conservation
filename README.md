# ESP32 Water Monitoring and Conservation System
  This Arduino code is designed to monitor the water level, flow rate, and turbidity of a tank or reservoir. It uses a variety of sensors and hardware components to collect this data and store it in a local SD card for later analysis.


## Hardware
  * DOIT ESP32 DevKit V1 board
  * HC-SR04 ultrasonic sensor for measuring water level
  * YF-S201C flow sensor for measuring flow rate
  * Turbidity sensor module for measuring water turbidity
  * SD card module for storing data
  * Relay module or some components to function as a relay (more on it later)
  * DC water pump


## Code Structure
  The code includes several libraries for interacting with the sensors and hardware components, including `WiFi.h`, `ESP32WebServer.h`, `ESPmDNS.h`, `Wire.h`, `SPI.h`, and `SD.h`. It also uses the RTClib library, `RTClib.h` for handling real-time clock functionality.

  The code defines several functions for handling different tasks, including `pulse()` for measuring the flow rate, `fill()` for controlling the pump and maintaining the water level, `logData()` for storing data to the SD card, and `sdDir()` for displaying the contents of the SD card on a web server.

  There are also several global variables defined for storing data and configuring the code's behavior, such as `pulse_freq`, `flow`, and `turbidity`.


## Functionality
  The code uses the HC-SR04 ultrasonic sensor to measure the distance from the sensor to the water surface, which is then converted to a water level in centimeters. If the water level falls below a certain threshold, the pump is turned on to fill the tank. If the water level exceeds another threshold, the pump is turned off to prevent overflow.

  The YF-S201C flow sensor is used to measure the total flow rate of water per day in liters. This data is collected using an interrupt-based pulse counting method, which allows the code to accurately measure the flow rate even while performing other tasks.
  
  The turbidity sensor is used to measure the turbidity of the water, which is a measure of the amount of suspended particles in the water. The turbidity data is then logged to the SD card along with the total flow data.

  The data is logged to the SD card at a specified interval, and the log files can be accessed through the web interface provided by the code. The web interface also allows for downloading the data files.


## Setup


## Additi
  
