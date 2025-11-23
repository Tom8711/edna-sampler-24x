// main_test_pump_clock.cpp
//
// Test 1 pomp met klokmodule (DS3231)
// Sequence:
//   5s forward  -> 3s stop -> 5s reverse -> 3s stop
// Elke seconde: tijd loggen via klok
// Geen echte hardware-aansturing als DEBUG_PUMP_MODE is gedefinieerd
// (PumpHardware zou in dat geval alleen loggen).

#include <Arduino.h>
#include "Clock.h"
#include "Pump.h"

// Pas deze pinnen aan als jouw hardware anders is:
static const int PUMP0_PIN_FORWARD = 5;
static const int PUMP0_PIN_REVERSE = 6;

// Pomp-instantie
Pump pump0(PUMP0_PIN_FORWARD, PUMP0_PIN_REVERSE);

// Eenvoudige state machine voor de sequence
enum PumpPhase {
  PHASE_FORWARD = 0,
  PHASE_STOP1,
  PHASE_REVERSE,
  PHASE_STOP2,
  PHASE_DONE      // eventueel hergebruikt of eindstatus
};

PumpPhase currentPhase = PHASE_FORWARD;

// Timestamps
unsigned long phaseStartMs = 0;
unsigned long lastClockLogMs = 0;

// Hulpfunctie om van fase te wisselen
void enterPhase(PumpPhase next) {
  currentPhase = next;
  phaseStartMs = millis();

  switch (currentPhase) {
    case PHASE_FORWARD:
      Serial.println("[PHASE] FORWARD (5s)");
      pump0.startPump(255);     // max power; in debug-mode alleen log
      break;

    case PHASE_STOP1:
      Serial.println("[PHASE] STOP 1 (3s)");
      pump0.stopPump();
      break;

    case PHASE_REVERSE:
      Serial.println("[PHASE] REVERSE (5s)");
      pump0.reverseDirection(255);
      break;

    case PHASE_STOP2:
      Serial.println("[PHASE] STOP 2 (3s)");
      pump0.stopPump();
      break;

    case PHASE_DONE:
      Serial.println("[PHASE] DONE (blijft gestopt)");
      pump0.stopPump();
      break;
  }
}

void setup() {
  Serial.begin(115200);
  // Kleine delay zodat de seriële monitor rustig kan openen
  delay(2000);

  Serial.println();
  Serial.println("=== main_test_pump_clock.cpp ===");
  Serial.println("Test: pomp + klok, debug-mode");
#ifdef DEBUG_PUMP_MODE
  Serial.println("DEBUG_PUMP_MODE is actief (geen echte hardware-aansturing).");
#else
  Serial.println("LET OP: DEBUG_PUMP_MODE staat NIET aan (hardware wordt echt aangestuurd).");
#endif

  // Klok initialiseren
  clockBegin();
  // Optioneel: compile-time als starttijd gebruiken
  clockSetCompileTime();
  Serial.println("[Clock] Clock initialized & set to compile time (indien gewenst).");

  // Pomp initialiseren
  pump0.begin();
  Serial.println("[Pump] Pump initialized.");

  // Start in de FORWARD-fase
  enterPhase(PHASE_FORWARD);

  // Startwaarden voor logging
  unsigned long now = millis();
  phaseStartMs = now;
  lastClockLogMs = now;
}

void loop() {
  unsigned long now = millis();

  // Elke seconde tijd loggen via klok
  if (now - lastClockLogMs >= 1000) {
    lastClockLogMs = now;

    Serial.print("[Clock] Now = ");
    // Verwachting: clockLogNow() logt de huidige tijd zelf
    clockLogNow();
  }

  // Fase-timing
  unsigned long elapsed = now - phaseStartMs;

  switch (currentPhase) {
    case PHASE_FORWARD:
      // 5s forward
      if (elapsed >= 5000UL) {
        enterPhase(PHASE_STOP1);
      }
      break;

    case PHASE_STOP1:
      // 3s stop
      if (elapsed >= 3000UL) {
        enterPhase(PHASE_REVERSE);
      }
      break;

    case PHASE_REVERSE:
      // 5s reverse
      if (elapsed >= 5000UL) {
        enterPhase(PHASE_STOP2);
      }
      break;

    case PHASE_STOP2:
      // 3s stop
      if (elapsed >= 3000UL) {
        // Keuze 1: sequence één keer draaien, dan stoppen
        enterPhase(PHASE_DONE);

        // Als je de sequence eindeloos wilt herhalen, vervang bovenstaande regel door:
        // enterPhase(PHASE_FORWARD);
      }
      break;

    case PHASE_DONE:
      // Niets meer doen; pomp blijft uit.
      break;
  }

  // Geen delay(), zodat millis()-gebaseerde timing strak blijft.
}
