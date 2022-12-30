// import libraries
  #include <WiFi.h>               // Library for WiFi functionality
  #include <ESP32WebServer.h>     // Library for creating a web server
  #include <ESPmDNS.h>            // Library for multicast domain name system (mDNS) support
  #include <Wire.h>               // Library for communication with I2C devices
  #include <SPI.h>                // Library for communication with SPI devices
  #include <SD.h>                 // Library for reading and writing to an SD card
  #include <RTClib.h>             // Library for real-time clock (RTC) support

// declare an instance of the ESP32WebServer class and listens at port 80
  ESP32WebServer server(80);

// WiFi network settings
  const char* ssid = "Water";
  const char* pass = "12345678";

// declare an instance of the RTC_DS3231 class
  RTC_DS3231 rtc;

// include web file
  #include "HTML.h"

// define constants for the server name and pin numbers
  #define servername "MCserver"
  #define CS 5
  #define flowPin 32
  #define turbidityPin 35
  #define trigPin 25
  #define echoPin 26
  #define pumpRelayPin 33


// Declare variables to track if the day has changed
  uint8_t dayYesterday;
  uint8_t dayToday;

// sd card variables
  bool sdStatus = 0;      // bool to track if sd card is present
  bool firstEntry;        // bool to track first time of logging usage data
  String turbidityLog;    // string of turbidity data to be logged directly to sd card
  String flowLog;         // string of flow data to be logged directly to sd card
  int sizeOfPrevData;     // size of previously logged data
  File turbidityData;     // file object to access turbidity data file on SD card
  File flowData;          // file object to access flow data file on SD card

// ultrasonic sensor variables
  long duration;          // time taken for ultrasonic pulse to return
  unsigned long fillCheckTimeLast = 0;  // time of last water level check
  unsigned long fillCheckTime = 200; // check tank level every 0.2 secs (200 millisecs)
  int16_t waterLevel;     // current water level
  int16_t minWaterLevel = 8; // more distance means less water in the tank
  int16_t maxWaterLevel = 2; // DO NOT go less than 2cm; that's the minimum range of the ultrasonic sensor
                             // trying to measure less than 2cm will result in random values
                             // the ultrasonic sensor might think the tank is empty and activate pump
                             // water will overflow, enter the sensor and damage it
  
// data logging variables
  unsigned long logTimeLast = 0;  // time of last data log
  unsigned long logTime = 10000;  //log to sd card every 10 secs (10000 millisecs)

// turbidity sensor variables
  int turbidity;

// flow sensor variables
  unsigned long volatile pulse_freq;  // gets increased by an ISR funtion when water flows
  double flow;  //calculated liters of passing water


// declare functions
  void IRAM_ATTR pulse();
  void fill();
  void logData ();

  void sdDir();
  void printDirectory(const char* dirname, uint8_t levels);
  void sdFileDownload(String filename);
  void sendHTMLHeader();
  void sendHTMLContent();
  void sendHTMLStop();
  void reportSDNotPresent();
  void reportFileNotPresent();
  String fileSize(int bytes);
  


void setup() {
  // comment out if you want to disable logging to serial monitor
  Serial.begin(115200);

  // setup hardware pins
    pinMode(flowPin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(turbidityPin, INPUT);
    pinMode(pumpRelayPin, OUTPUT);

  //initialize rtc
    rtc.begin();

  // attach an interrupt handler to the flow sensor pin
    attachInterrupt(digitalPinToInterrupt(flowPin), pulse, RISING);

  // create soft access point
    WiFi.softAP(ssid, pass);

  // start mDNS responder
    if (!MDNS.begin(servername)) {
    Serial.print("Error setting up MDNS responder!\n"); 
    ESP.restart();
    }

  // initialize SD card and store the result in the sdStatus variable
    Serial.print("Initializing SD card...\n");
    if (!SD.begin(CS)) sdStatus = 0;
    else sdStatus = 1;
    Serial.print("sdPresent: " + String(sdStatus) + "\n");

  // update yesterday variable
    DateTime rtcTime = rtc.now();                         
    dayYesterday = rtcTime.dayOfTheWeek();

  // Set up a handler for the root URL and start the web server
    server.on("/", sdDir);
    server.begin();
    Serial.print("HTTP server started\n");
  
  // wait a sec
    delay(1000);
}



void loop() {
  // handle incoming client requests
    server.handleClient();

  // check and fill tank
    fill();

  // get and log data
    logData();
}
