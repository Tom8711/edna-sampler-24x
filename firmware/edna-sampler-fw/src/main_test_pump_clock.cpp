// src/main_test_pump_clock.cpp

#include <Arduino.h>
#include "Pump.h"
#include "Clock.h"

// Kies hier de gebruikte pomp-pinnen
static const int PUMP_PIN_FORWARD = 5;
static const int PUMP_PIN_REVERSE = 6;

// Pomp-instantie
Pump pump(PUMP_PIN_FORWARD, PUMP_PIN_REVERSE);

// Eenvoudige state-machine voor de pomp-sequentie
enum PumpState {
  PUMP_STATE_FORWARD,
  PUMP_STATE_WAIT_AFTER_FORWARD,
  PUMP_STATE_REVERSE,
  PUMP_STATE_DONE
};

PumpState pumpState = PUMP_STATE_FORWARD;
unsigned long stateStartMs = 0;

// Duraties in milliseconden
const uint32_t PUMP_FORWARD_DURATION_MS      = 5000; // 5 s
const uint32_t PUMP_STOP_PAUSE_MS           = 3000; // 3 s
const uint32_t PUMP_REVERSE_DURATION_MS     = 5000; // 5 s

// Tijd-log interval
unsigned long lastClockLogMs = 0;
const uint32_t CLOCK_LOG_INTERVAL_MS = 1000; // 1 s

void setup() {
  Serial.begin(115200);
  delay(1000);

#ifdef DEBUG_PUMP_MODE
  Serial.println(F("[MAIN] Start in DEBUG_PUMP_MODE (alleen logging, geen echte hardware)."));
#else
  Serial.println(F("[MAIN] Start in normale modus (hardware actief)."));
#endif

  // Klok initialiseren
  clockBegin();
  Serial.println(F("[MAIN] Clock initialised (DS3231 via IO9/IO10)."));

  // Eénmalig direct tijd loggen
  clockLogNow();

  // Optioneel: DS3231 op compile-tijd zetten
  // Let op: dit zet bij elke upload de tijd terug naar compile time.
  // clockSetCompileTime();

  // Pomp initialiseren
  pump.begin();
  Serial.println(F("[MAIN] Pump initialised. Starting forward sequence."));

  // Start: pomp vooruit
  pump.startPump(255); // maximale “power” (afhankelijk van jouw implementatie)
  pumpState     = PUMP_STATE_FORWARD;
  stateStartMs  = millis();
  lastClockLogMs = millis();
}

void loop() {
  unsigned long nowMs = millis();

  // Elke seconde tijd loggen
  if (nowMs - lastClockLogMs >= CLOCK_LOG_INTERVAL_MS) {
    lastClockLogMs += CLOCK_LOG_INTERVAL_MS; // zodat de drift beperkt blijft
    clockLogNow();
  }

  // Pomp state-machine
  switch (pumpState) {
    case PUMP_STATE_FORWARD:
      if (nowMs - stateStartMs >= PUMP_FORWARD_DURATION_MS) {
        pump.stopPump();
        Serial.println(F("[MAIN] Pump: stop na forward."));
        pumpState    = PUMP_STATE_WAIT_AFTER_FORWARD;
        stateStartMs = nowMs;
      }
      break;

    case PUMP_STATE_WAIT_AFTER_FORWARD:
      if (nowMs - stateStartMs >= PUMP_STOP_PAUSE_MS) {
        Serial.println(F("[MAIN] Pump: start reverse."));
        pump.reverseDirection(255);
        pumpState    = PUMP_STATE_REVERSE;
        stateStartMs = nowMs;
      }
      break;

    case PUMP_STATE_REVERSE:
      if (nowMs - stateStartMs >= PUMP_REVERSE_DURATION_MS) {
        pump.stopPump();
        Serial.println(F("[MAIN] Pump: stop na reverse. Sequence klaar."));
        pumpState = PUMP_STATE_DONE;
      }
      break;

    case PUMP_STATE_DONE:
      // Niets meer doen: klok blijft elke seconde loggen,
      // pomp blijft uit.
      break;
  }
}
