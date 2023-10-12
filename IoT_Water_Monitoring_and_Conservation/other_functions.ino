// ISR for counting pulses from flow rate sensor
void IRAM_ATTR pulse() {
  pulse_freq++;
}


// read and log turbidity & flow
void logData () {
  static unsigned long logTimeLast = 0;
  
  if (millis() - logTimeLast >= LOGTIME) {
    // get date and time stamps
    DateTime rtcTime = rtc.now();
    dayToday = rtcTime.dayOfTheWeek();

    String dateStamp = rtcTime.timestamp(DateTime::TIMESTAMP_DATE);
    String timeStamp = rtcTime.timestamp(DateTime::TIMESTAMP_TIME);
    Serial.print(dateStamp + " " + timeStamp + " ");

    // get readings from sensors
    turbidity = analogRead(turbPin) * 100. / 2245.;
    flow = 0.00225 * pulse_freq;

    // form the log strings
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



unsigned readUtrasonicDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  return (pulseInLong(echoPin, HIGH) / 2) * 0.0344;
}