#include <Arduino.h>
#line 1 "C:\\Users\\nkklo\\Documents\\GitHub\\iot-water-monitoring-and-conservation\\IoT_Water_Monitoring_and_Conservation\\IoT_Water_Monitoring_and_Conservation.ino"
// import libraries
  // #include <WiFi.h>               // Library for WiFi functionality
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

//define server name
  #define servername "datalog"

// define pin numbers constants
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

#line 1 "C:\\Users\\nkklo\\Documents\\GitHub\\iot-water-monitoring-and-conservation\\IoT_Water_Monitoring_and_Conservation\\other_functions.ino"
// Interrupt service routine to count the number of pulses from the flow rate sensor
  void IRAM_ATTR pulse() {
    pulse_freq++;
  }


// check and fill tank function
  void fill() {
    if (millis() - fillCheckTimeLast >= fillCheckTime) {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      duration = pulseInLong(echoPin, HIGH);
      waterLevel = (duration / 2) * 0.0344;

      fillCheckTimeLast = millis();
    }
    
    if (waterLevel >= minWaterLevel) {
      digitalWrite(pumpRelayPin, HIGH);
    }
    if (waterLevel <= maxWaterLevel) {
      digitalWrite(pumpRelayPin, LOW);
    }
  }


// read and log turbidity & flow
  void logData () {
    if (millis() - logTimeLast >= logTime) {

      // get date and time stamps
        DateTime rtcTime = rtc.now();
        dayToday = rtcTime.dayOfTheWeek();
        String dateStamp = rtcTime.timestamp(DateTime::TIMESTAMP_DATE);
        String timeStamp = rtcTime.timestamp(DateTime::TIMESTAMP_TIME);
        Serial.print(dateStamp + " " + timeStamp + " ");

      // get data and form the log strings
        turbidity = analogRead(turbidityPin) * 100. / 2245.;
        flow = 0.00225 * pulse_freq;

        Serial.print("Turbidity=" + String(turbidity) + " ");
        Serial.print("Flow=" + String(flow) + " ");

        turbidityLog = dateStamp + "," + timeStamp + "," + String(turbidity) + "\n";      
        flowLog = dateStamp + "," + String(flow) + "\n";

      //log turbidity
        turbidityData = SD.open("/DATA/QUALITY.CSV", FILE_APPEND);
        if (turbidityData) {
          turbidityData.print(turbidityLog);
          turbidityData.close();
          Serial.print("loggedTurb ");
        }

      //log usage
        flowData = SD.open("/DATA/USAGE.CSV", FILE_READ);
        if (flowData) { // check the size of the USAGE.CSV file
          firstEntry = (flowData.size() == 11); // if size is 11, no data has been logged
          flowData.close();
        }
        
        if (firstEntry) { //if no data has been logged, log first data
          flowData = SD.open("/DATA/USAGE.CSV", FILE_APPEND);
          if (flowData) {
            flowData.print(flowLog);
            flowData.close();
            sizeOfPrevData = flowLog.length(); //update previous data length
            Serial.print("loggedFlowFirst ");
          }
        }
        else if (dayYesterday != dayToday) { // else if day has changed, log usage on newline
          flowData = SD.open("/DATA/USAGE.CSV", FILE_APPEND);
          if (flowData) {
            flowData.print(flowLog);
            flowData.close();
            pulse_freq = 0;
            dayYesterday = dayToday;
            sizeOfPrevData = flowLog.length(); //update previous data length
            Serial.print("loggedFlowNew ");
          }
        }
        else { // else update usage for the day
          flowData = SD.open("/DATA/USAGE.CSV", FILE_WRITE);
          if (flowData) {
            flowData.seek(flowData.size() - sizeOfPrevData);
            if (flowData.position() == (flowData.size() - sizeOfPrevData)) {
              flowData.print(flowLog);
              flowData.close();
              sizeOfPrevData = flowLog.length(); //update previous data length
              Serial.print("loggedFlowUpdate ");
            }              
          }
        }
        //Serial.print("waterLevel=" + String(waterLevel) + " ");
      Serial.print("DONE\n");
      logTimeLast = millis();
    }
  }



#line 1 "C:\\Users\\nkklo\\Documents\\GitHub\\iot-water-monitoring-and-conservation\\IoT_Water_Monitoring_and_Conservation\\web_page_functions.ino"
// generate HTML page and list contents of the data directory on SD card.
  void sdDir() {
    if (sdStatus) {
      if (server.args() > 0 ) {
        String Order = server.arg(0);
        if (Order.indexOf("download_")>=0) {
          Order.remove(0,9);
          sdFileDownload(Order);
        }
      }

      File data = SD.open("/DATA");
      if (data) {
        data.rewindDirectory();
        sendHTMLHeader();
        webpage += F("<table align='center'>");
        webpage += F("<tr><th>Name/Type</th><th style='width:20%'>Type File/Dir</th><th>File Size</th></tr>");
        printDirectory("/DATA",0);
        webpage += F("</table>");
        sendHTMLContent();
        data.close();
      }
      else
      {
        sendHTMLHeader();
        webpage += F("<h3>No Files Found</h3>");
      }
      appendPageFooter();
      sendHTMLContent();
      sendHTMLStop();
    } else reportSDNotPresent();
  }


// recursively list contents of a directory on SD card as html tables
  void printDirectory(const char* dirname, uint8_t levels) {
    File data = SD.open(dirname);
    if (!data) return;
    if (!data.isDirectory()) return;

    File file = data.openNextFile();
    int i = 0;
    while(file) {
      if (webpage.length() > 1000) sendHTMLContent();
      if (file.isDirectory()) {
        webpage += "<tr><td>"+String(file.isDirectory()?"Dir":"File")+"</td><td>"+String(file.name())+"</td><td></td></tr>";
        printDirectory(file.name(), levels-1);
      }
      else
      {
        webpage += "<tr><td>"+String(file.name())+"</td>";
        webpage += "<td>"+String(file.isDirectory()?"Dir":"File")+"</td>";
          String fsize = fileSize(file.size());
        webpage += "<td>"+fsize+"</td>";
        webpage += "<td>";
        webpage += F("<FORM action='/' method='post'>");
        webpage += F("<button type='submit' name='download'");
        webpage += F("' value='"); webpage +="download_"+String(file.name()); webpage +=F("'>Download</button>");
        webpage += "</td></tr>";
      }
      file = data.openNextFile();
      i++;
    }
    file.close();
  }


// handle client requests to download files from SD card
  void sdFileDownload(String filename) {
    if (sdStatus) {
      File download = SD.open("/"+filename);
      if (download) {
        server.sendHeader("Content-Type", "text/text");
        server.sendHeader("Content-Disposition", "attachment; filename="+filename);
        server.sendHeader("Connection", "close");
        server.streamFile(download, "application/octet-stream");
        download.close();
      } else reportFileNotPresent();
    } else reportSDNotPresent();
  }


// send HTTP header to client and begin an HTML page
  void sendHTMLHeader() {
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.setContentLength(CONTENT_LENGTH_UNKNOWN); 
    server.send(200, "text/html", "");
    appendPageHeader();
    server.sendContent(webpage);
    webpage = "";
  }


//  send the contents of the 'webpage' variable to the client.
  void sendHTMLContent() {
    server.sendContent(webpage);
    webpage = "";
  }


// send empty string to client and terminate connection.
  void sendHTMLStop() {
    server.sendContent("");
    server.client().stop();
  }


// generates HTML page with message indicating no SD card is present
  void reportSDNotPresent() {
    sendHTMLHeader();
    webpage += F("<h3>No SD Card present</h3>");
    webpage += F("<a href='/'>[Back]</a><br><br>");
    appendPageFooter();
    sendHTMLContent();
    sendHTMLStop();
  }


// generates HTML page with message indicating the requested file is not present on the SD card
  void reportFileNotPresent() {
    sendHTMLHeader();
    webpage += F("<h3>File does not exist</h3>");
    webpage += F("<a href='/'>[Back]</a><br><br>");
    appendPageFooter();
    sendHTMLContent();
    sendHTMLStop();
  }


// convert file size to human-readable values
  String fileSize(int bytes) {
    String fsize = "";
    if (bytes < 1024)                  fsize = String(bytes)+" B";
    else if (bytes < (1024*1024))      fsize = String(bytes/1024.0,3)+" KB";
    else if (bytes < (1024*1024*1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
    else                               fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
    return fsize;
  }
