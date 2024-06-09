#include <Arduino.h>

#define def_LED 0

void setup() {
  pinMode(def_LED,OUTPUT);
}

void loop() {
  digitalWrite(def_LED,HIGH);
  delay(1000);
  digitalWrite(def_LED,LOW);
  delay(1000);
}

