#include <Arduino.h>
#include "Pump.h"

// Kies 4 GPIO's op de ESP32-S3 voor de 2 pompen.
// Voorbeeld:
//  - Pomp 1: IO5 = vooruit, IO6 = achteruit
//  - Pomp 2: IO7 = vooruit, IO8 = achteruit
//
// Pas deze pinnen aan als je andere wilt gebruiken.
Pump pump1(15, 16);
Pump pump2(7, 8);

// Tijden in milliseconden
const uint32_t PUMP_FORWARD_MS = 60 * 1000; // 1 minuut
const uint32_t PUMP_REVERSE_MS = 10 * 1000; // 10 seconden

void runPump1Once()
{
  Serial.println("=== POMP 1: start ===");

  Serial.println("Pomp 1: vooruit (1 minuut)...");
  pump1.startPump();                // default power = 255
  delay(PUMP_FORWARD_MS);

  Serial.println("Pomp 1: stoppen...");
  pump1.stopPump();
  delay(500);                       // heel korte pauze

  Serial.println("Pomp 1: achteruit (10s)...");
  pump1.reverseDirection();
  delay(PUMP_REVERSE_MS);

  Serial.println("Pomp 1: stoppen (klaar)...");
  pump1.stopPump();
}

void runPump2Once()
{
  Serial.println("=== POMP 2: start ===");

  Serial.println("Pomp 2: vooruit (1 minuut)...");
  pump2.startPump();
  delay(PUMP_FORWARD_MS);

  Serial.println("Pomp 2: stoppen...");
  pump2.stopPump();
  delay(500);

  Serial.println("Pomp 2: achteruit (10s)...");
  pump2.reverseDirection();
  delay(PUMP_REVERSE_MS);

  Serial.println("Pomp 2: stoppen (klaar)...");
  pump2.stopPump();
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("=== Twee pompen basic test (zonder deep sleep) ===");

  // Pomp-hardware initialiseren via Pump::begin()
  pump1.begin();
  pump2.begin();

  // Eerst pomp 1
  runPump1Once();

  // Kleine pauze tussen pomp 1 en pomp 2
  Serial.println("Pauze tussen pomp 1 en pomp 2...");
  delay(2000);

  // Daarna pomp 2
  runPump2Once();

  Serial.println("Beide pompen zijn klaar. Er gebeurt nu niets meer.");
}

void loop()
{
  // Leeg laten: alles gebeurt eenmalig in setup()
}
