// main.cpp is de enige entrypoint voor de ESP32-S3.
// Zorg dat geen andere bestanden setup()/loop() definiëren.

#include <Arduino.h>
#include "Pump.h"
#include "Clock.h"

// voorbeeldpomp op willekeurige pins
// Pump pump(5, 6);

void setup()
{
  Serial.begin(115200);
  delay(2000); // seriële monitor laten opstarten

  Serial.println();
  Serial.println(F("=== eDNA-sampler: DS3231 kloktest ==="));

  clockBegin();

  // Eenmalig de tijd op compile-tijd zetten (alleen eerste keer nodig)
  // Daarna kun je deze regel weer uitcommentariëren om de RTC niet telkens te overschrijven.
  // clockSetCompileTime();

  // Serial.begin(115200);
  // delay(2000);
  // Serial.println("Sampler debug-mode test");
  // pump.begin();
}

void loop()
{
  static unsigned long lastLog = 0;

  if (millis() - lastLog >= 1000)
  { // elke seconde loggen
    lastLog = millis();
    clockLogNow();
  }

  // Serial.println("Forward...");
  // pump.startPump();
  // delay(1000);

  // Serial.println("Stop...");
  // pump.stopPump();
  // delay(1000);

  // Serial.println("Reverse...");
  // pump.reverseDirection();
  // delay(1000);

  // Serial.println("Stop...");
  // pump.stopPump();
  // delay(2000);
}
