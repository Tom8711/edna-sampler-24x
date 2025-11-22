// main.cpp is de enige entrypoint voor de ESP32-S3.
// Zorg dat geen andere bestanden setup()/loop() definiëren.

#include <Arduino.h>
#include "Pump.h"
#include "Clock.h"

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

}

void loop()
{
  static unsigned long lastLog = 0;

  if (millis() - lastLog >= 1000)
  { // elke seconde loggen
    lastLog = millis();
    clockLogNow();
  }

}
