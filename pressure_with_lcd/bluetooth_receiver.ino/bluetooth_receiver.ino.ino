#include "Arduino.h"
#include <SoftwareSerial.h>

SoftwareSerial BTserial(4, 6); // RX , TX

void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);
}

void loop() {
  //if (BTserial.available())
  if (BTserial.available() > 0) {
    String sensorVal = (String) BTserial.read();
  }
}
