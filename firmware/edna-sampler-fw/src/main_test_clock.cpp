#include <Arduino.h>
#include <RTClib.h>   // voor DateTime
#include "Clock.h"

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println(F("=== Compile time vs RTC test ==="));

  // 1) Compile-tijd bepalen (wat clockSetCompileTime ook gebruikt)
  DateTime compileTime(F(__DATE__), F(__TIME__));

  Serial.print(F("Compile time: "));
  Serial.print(compileTime.year());   Serial.print("-");
  Serial.print(compileTime.month());  Serial.print("-");
  Serial.print(compileTime.day());    Serial.print(" ");
  Serial.print(compileTime.hour());   Serial.print(":");
  Serial.print(compileTime.minute()); Serial.print(":");
  Serial.print(compileTime.second()); Serial.println();

  // 2) RTC initialiseren en op compile-tijd zetten
  clockBegin();
  clockSetCompileTime();

  // 3) RTC-tijd via clockNow() teruglezen
  ClockDateTime now;
  if (clockNow(now)) {
    Serial.print(F("RTC after adjust: "));
    Serial.print(now.year);   Serial.print("-");
    Serial.print(now.month);  Serial.print("-");
    Serial.print(now.day);    Serial.print(" ");
    Serial.print(now.hour);   Serial.print(":");
    Serial.print(now.minute); Serial.print(":");
    Serial.print(now.second); Serial.println();
  } else {
    Serial.println(F("RTC after adjust: RTC niet OK"));
  }
}

void loop() {
  // niets nodig
}
