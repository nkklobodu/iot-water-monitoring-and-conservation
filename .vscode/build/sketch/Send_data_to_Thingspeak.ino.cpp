#include <Arduino.h>
#line 1 "c:\\Users\\nkklo\\Documents\\Arduino\\ESP8266\\NodeMCU - ESP8266\\Send_data_to_Thingspeak\\Send_data_to_Thingspeak.ino"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;
HTTPClient http;

void connectWifi();

const char* ssid = "NAT";
const char* pass = "Nathaniel";

#line 13 "c:\\Users\\nkklo\\Documents\\Arduino\\ESP8266\\NodeMCU - ESP8266\\Send_data_to_Thingspeak\\Send_data_to_Thingspeak.ino"
void setup();
#line 18 "c:\\Users\\nkklo\\Documents\\Arduino\\ESP8266\\NodeMCU - ESP8266\\Send_data_to_Thingspeak\\Send_data_to_Thingspeak.ino"
void loop();
#line 34 "c:\\Users\\nkklo\\Documents\\Arduino\\ESP8266\\NodeMCU - ESP8266\\Send_data_to_Thingspeak\\Send_data_to_Thingspeak.ino"
void connectWiFi();
#line 55 "c:\\Users\\nkklo\\Documents\\Arduino\\ESP8266\\NodeMCU - ESP8266\\Send_data_to_Thingspeak\\Send_data_to_Thingspeak.ino"
void sendData(String apiKey, uint8_t field, long data);
#line 13 "c:\\Users\\nkklo\\Documents\\Arduino\\ESP8266\\NodeMCU - ESP8266\\Send_data_to_Thingspeak\\Send_data_to_Thingspeak.ino"
void setup() {
  Serial.begin(115200);
  connectWiFi();
}

void loop() {
//   int rand1 = random(0, 101); //generate random number from 0 exclusive to 101 exclusive ie. 1-100
//   int rand2 = random(0, 101);

//   Serial.print("Data 1 =");
//   Serial.println(rand1);
//   sendData("EWXI320J86RIXRPL", 1, rand1);

//   Serial.print("Data 2 =");
//   Serial.println(rand2);
//   sendData("EWXI320J86RIXRPL", 2, rand2);
  
}



void connectWiFi() {
  Serial.println("");
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Mac Address: ");
  Serial.println(WiFi.macAddress());
}

void sendData(String apiKey, uint8_t field, long data) {
  String url = "http://api.thingspeak.com/update?api_key=";
  url += apiKey;
  url += "&field";
  url += field;
  url += "=";
  url += data;

  http.begin(client, url);

  if(http.GET() > 0) Serial.println("Successful");
  else Serial.println("ERROR");
  http.end();
  delay(15000);
}
