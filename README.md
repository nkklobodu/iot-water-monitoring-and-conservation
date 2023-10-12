# IoT Water Monitoring and Conservation

<!-- 
## "Image"
  * An image or screenshot of the project
  * Embed the image in the README.md file
    ### Example
      <img src="assets/images/setup.jpg" alt = "project image" width="780">
 -->

<!-- ## "Description" -->
  <!-- * A brief description of what the project does -->
  The IoT Water Monitoring and Conservation project is an Arduino-based project that aims to monitor and conserve water usage in real-time. The system uses sensors, modules, and devices to track the flow and turbidity of water supply, maintain the water level of a water tank or reservoir, and log the data to an SD card. The logged data can be accessed through a mDNS web server for later analysis.
  
  This project is a practical tool for anyone who wants to manage their water usage more effectively and positively impact the environment. It can help save water, reduce waste, save utility bills, and assist decision-making.

  <!-- * The problems it solves (SDGs, add image if possible) -->
  The IoT Water Monitoring and Conservation project contributes to achieving several Sustainable Development Goals (SDGs), including [SDG 6: Clean Water and Sanitation][sdg6], [SDG 12: Responsible Consumption and Production][sdg12], and [SDG 13: Climate Action][sdg13].

  [sdg6]: <https://sdgs.un.org/goals/goal6>
  [sdg12]: <https://sdgs.un.org/goals/goal12>
  [sdg13]: <https://sdgs.un.org/goals/goal13>
  
  
  By monitoring and managing water usage more effectively, this project helps reduce water waste and promote sustainable water consumption, directly supporting **SDG 6**. Additionally, by promoting responsible consumption and production of resources, such as water, this project contributes to **SDG 12**. Finally, by reducing water waste and promoting sustainable water consumption, this project helps mitigate the impacts of climate change, aligning with **SDG 13**.

## Key Features
  * Maintains custom set water level by monitoring water level and turning a water pump on or off as needed
  * Logs the following data to an SD card 
    * Turbidity with date and time stamps
    * Total daily water usage
  * Provides a web interface for viewing the data log and downloading data files

## Collaborators
  * [Nathaniel Klobodu](https://github.com/nkklobodu)
  * [Jeffrey Asiedu-Brako](https://github.com/ziegler121)

## Watch the Project Video
[![Watch the video](https://img.youtube.com/vi/HQruk8vdTt4/0.jpg)](https://www.youtube.com/watch?v=HQruk8vdTt4)

https://www.youtube.com/watch?v=HQruk8vdTt4

## Repository Contents
  <!--
  * **src/** - Source code of the project
  * **docs/** - Documentation of the project
  * **images/** - Images used in the documentation
  * **README.md** - This file
  -->

## Requirements
  ### Hardware
  <!-- add links to online stores or local stores where the hardware can be purchased -->
   * DOIT ESP32 DevKit V1 board
   * HC-SR04 ultrasonic sensor
   * YF-S201C flow sensor
   * Turbidity sensor module
   * SD card module and SD card
   * DS3231 RTC module
   * DC water pump
   * Relay module

  ### Software & online services
  <!-- add links to download software and online sites to use -->
   * [Arduino IDE](https://www.arduino.cc/en/software "Download Arduino IDE")
   * Microsoft Excel or any spreadsheet software
  
  ### Libraries
  <!-- add links to download or documentation of libraries used -->
   * ESP32WebServer
   * ESPmDNS
   * RTClib
   * SD
   * SPI
   * WiFi
   * Wire

## Detailed Documentation
  <!-- * A link to the detailed documentation in the wiki -->
  For more detailed documentation about this project and how to implement it, please see our [Wiki section](https://github.com/nkklobodu/iot-water-monitoring-and-conservation/wiki "Wiki Documentation Outline"). (Coming soon!)

## Contribution
  We welcome contributions to this project. If you have an idea for a new feature or have found a bug, please open an issue in the repository.
