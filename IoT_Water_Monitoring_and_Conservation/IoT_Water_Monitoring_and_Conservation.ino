// import libraries
#include <ESP32WebServer.h>
#include <ESPmDNS.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include "config.h"
#include "html.h"

// WiFi network settings
const char* ssid = USER_SSID;
const char* pass = USER_PASS;
const char* servername = USER_SERVER;  // String servername = USER_SERVER;

// create objects
ESP32WebServer server(80);  // ESP32WebServer instance, listens at port 80
RTC_DS3231 rtc;             // RTC object

// define pin numbers constants
uint8_t csPin = CS;
uint8_t flowPin = FLOWPIN;
uint8_t turbPin = TURBPIN;
uint8_t trigPin = TRIGPIN;
uint8_t echoPin = ECHOPIN;
uint8_t relayPin = RELAYPIN;

// sensor data variables
unsigned waterLevel;                // current water level in cm
int turbidity;                      // water quality
volatile unsigned long pulse_freq;  // gets increased by an ISR funtion when water flows
double flow;                        //calculated liters of passing water


// Declare variables to track if the day has changed
uint8_t dayYesterday;
uint8_t dayToday;

// sd card variables
bool sdStatus = 0;    // bool to track if sd card is present
bool firstEntry;      // bool to track first time of logging usage data
String turbidityLog;  // string of turbidity data to be logged directly to sd card
String flowLog;       // string of flow data to be logged directly to sd card
int sizeOfPrevData;   // size of previously logged data
File turbidityData;   // file object to access turbidity data file on SD card
File flowData;        // file object to access flow data file on SD card



// declare functions
void IRAM_ATTR pulse();
void logData();

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
  pinMode(turbPin, INPUT);
  pinMode(relayPin, OUTPUT);

  //initialize rtc
  rtc.begin();

  // attach an interrupt handler to the flow sensor pin
  attachInterrupt(digitalPinToInterrupt(flowPin), pulse, RISING);

  // create soft access point
  WiFi.softAP(ssid, pass);

  // start mDNS responder
  while (!MDNS.begin(servername)) delay(500);
 
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

  // update current water level and control pump
  waterLevel = readUtrasonicDistance();
  if (waterLevel >= MINWATERLEVEL)
    digitalWrite(relayPin, HIGH);  // turn pump on if water level is below minimum
  if (waterLevel <= MAXWATERLEVEL)
    digitalWrite(relayPin, LOW);   // turn pump off if water level is above maximum

  // get and log data
  logData();
}
