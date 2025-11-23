// main.cpp is de enige entrypoint voor de ESP32-S3.
// Zorg dat geen andere bestanden setup()/loop() definiëren.

#include <Arduino.h>
#include <SPI.h>
#include "Pump.h"


// voorbeeldpomp op willekeurige pins
Pump pump(5, 6);

void setup()
{
  Serial.begin(115200);
  delay(2000); // seriële monitor laten opstarten

  Serial.begin(115200);
  delay(2000);
  Serial.println("Sampler debug-mode test");
  pump.begin();
}

void loop()
{

  Serial.println("Forward...");
  pump.startPump();
  delay(1000);

  Serial.println("Stop...");
  pump.stopPump();
  delay(1000);

  Serial.println("Reverse...");
  pump.reverseDirection();
  delay(1000);

  Serial.println("Stop...");
  pump.stopPump();
  delay(2000);
}
