#include <Arduino.h>
#include <Preferences.h>
#include "esp_sleep.h"

#include "Pump.h"
#include "Clock.h"

// NVS opslag om te onthouden in welke fase we zitten
Preferences prefs;

// DS3231 INT/SQW -> ESP32 GPIO4 (PAS AAN als je een andere pin gebruikt)
static const gpio_num_t RTC_INT_PIN = GPIO_NUM_4;

// Twee pompen direct op 4 pins van de ESP32
// Pomp 1: IO5 vooruit, IO6 achteruit
// Pomp 2: IO7 vooruit, IO8 achteruit
Pump pump1(5, 6);
Pump pump2(15, 16);

// Tijden in milliseconden
static const uint32_t PUMP_FORWARD_MS = 60 * 1000; // 1 minuut
static const uint32_t PUMP_REVERSE_MS = 10 * 1000; // 10 seconden

// Hoe lang we willen slapen tussen pomp 1 en pomp 2 (in minuten)
static const uint32_t SLEEP_MINUTES = 15;
static const uint32_t SLEEP_HOURS = 9;

// In welke "fase" van het schema zitten we?
// 0 = nog niets gedaan (we moeten pomp 1 draaien)
// 1 = pomp 1 is gedaan, na wake-up moeten we pomp 2 draaien
// 2 = alles gedaan
enum PumpPhase : uint8_t
{
    PHASE_FIRST = 0,
    PHASE_SECOND = 1,
    PHASE_DONE = 2
};

// Herbruikbare functie: pompsequence (vooruit + achteruit) uitvoeren
void runPumpSequence(Pump &pump, const char *label)
{
    Serial.println();
    Serial.print("=== Start sequence voor ");
    Serial.print(label);
    Serial.println(" ===");

    Serial.print(label);
    Serial.println(": vooruit (1 minuut)...");
    pump.startPump(); // default power = 255
    delay(PUMP_FORWARD_MS);

    Serial.print(label);
    Serial.println(": stoppen...");
    pump.stopPump();
    delay(500);

    Serial.print(label);
    Serial.println(": achteruit (10s)...");
    pump.reverseDirection();
    delay(PUMP_REVERSE_MS);

    Serial.print(label);
    Serial.println(": stoppen (klaar)...");
    pump.stopPump();
}

// Hulpfunctie: voorbereiden op deep sleep en volgende fase opslaan
void enterDeepSleepAndScheduleNext(PumpPhase nextPhase)
{
    Serial.println();
    Serial.println("=== Voorbereiden op deep sleep ===");

    // Volgende fase in NVS opslaan
    prefs.putUChar("phase", static_cast<uint8_t>(nextPhase));
    Serial.print("Volgende fase opgeslagen in NVS: ");
    Serial.println(static_cast<uint8_t>(nextPhase));

    // Alarm over SLEEP_MINUTES minuten plannen
    Serial.print("Plan alarm over ");
    Serial.print(SLEEP_HOURS);
    Serial.println(" uren...");

    if (!clockScheduleAlarmInMinutes(SLEEP_MINUTES, SLEEP_HOURS))
    {
        Serial.println("Kon geen alarm plannen. Geen deep sleep.");
        return;
    }

    // Wake-up bron configureren: DS3231 INT/SQW pin (actief laag)
    Serial.print("Configureer EXT0 wakeup op pin ");
    Serial.println((int)RTC_INT_PIN);
    esp_sleep_enable_ext0_wakeup(RTC_INT_PIN, 0); // 0 = WAKEUP bij LOW

    Serial.println("Ga nu in deep sleep...");
    Serial.flush();

    esp_deep_sleep_start();
}

// Hoofdlogica
void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("=== Twee pompen met klok + deep sleep ===");

    // NVS openen (namespace "pump_two_sleep")
    prefs.begin("pump_two_sleep", false);

    // Klok initialiseren
    clockBegin();

    // DS3231 INT-pin als input (meestal met pull-up op het bordje)
    pinMode(RTC_INT_PIN, INPUT_PULLUP);

    // Pompen initialiseren
    pump1.begin();
    pump2.begin();

    // Wakeup-oorzaak opvragen (kan handig zijn voor debug)
    esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    Serial.print("Wakeup oorzaak: ");
    Serial.println((int)cause);

    // Huidige fase uit NVS lezen (default: PHASE_FIRST)
    uint8_t phaseVal = prefs.getUChar("phase", static_cast<uint8_t>(PHASE_FIRST));
    PumpPhase phase = static_cast<PumpPhase>(phaseVal);

    Serial.print("RTC time: ");
    clockLogNow();

    Serial.print("Huidige fase uit NVS: ");
    Serial.println(phaseVal);

    if (phase == PHASE_FIRST)
    {
        Serial.println("Fase 0: Pomp 1 moet draaien.");
        runPumpSequence(pump1, "Pomp 1");

        // Na pomp 1 -> deep sleep, daarna pomp 2
        enterDeepSleepAndScheduleNext(PHASE_SECOND);
    }
    else if (phase == PHASE_SECOND)
    {
        Serial.println("Fase 1: Pomp 2 moet draaien (na wake-up).");
        runPumpSequence(pump2, "Pomp 2");

        // Alles klaar, markeer als DONE
        prefs.putUChar("phase", static_cast<uint8_t>(PHASE_DONE));
        Serial.println("Beide pompen zijn klaar. Geen nieuwe deep sleep.");
        prefs.putUChar("phase", PHASE_FIRST);
    }
    else
    {
        Serial.println("Fase 2 (DONE): niets meer te doen.");
    }

    Serial.println("Einde van setup(). Er gebeurt niets meer in loop(). Na herstart start fase 1 opnieuw");
}

void loop()
{
    // Leeg: alle logica gebeurt in setup()
}
