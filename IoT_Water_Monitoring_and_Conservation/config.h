// WiFi & network configuration
#define USER_SSID "SSID"
#define USER_PASS "PASSWORD"
#define USER_SERVER "datalog"

// Pin configuration
#define CS 5
#define FLOWPIN 32
#define TURBPIN 35
#define TRIGPIN 25
#define ECHOPIN 26
#define RELAYPIN 33

// time interval configuration
#define LOGTIME 10000     // log to sd card every x millisecs 

// custom configuration
#define MINWATERLEVEL 8  // more distance means less water in the tank
#define MAXWATERLEVEL 2  // DO NOT go less than 2cm; that's the minimum range of the ultrasonic sensor