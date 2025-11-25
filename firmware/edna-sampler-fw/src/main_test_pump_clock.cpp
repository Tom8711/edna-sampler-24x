#include <Arduino.h>
#include <Preferences.h>
#include "esp_sleep.h"

#include "Pump.h"
#include "PumpHardware.h"
#include "Clock.h"

Preferences prefs;

// DS3231 INT/SQW -> ESP32 GPIO4 (PAS AAN als je een andere pin gebruikt)
static const gpio_num_t RTC_INT_PIN = GPIO_NUM_4;

static const int PUMP_POWER = 255;
static const uint32_t FORWARD_MS = 5000;
static const uint32_t REVERSE_MS = 5000;
static const uint32_t PAUSE_MS = 3000;

// Interval tussen runs via RTC-alarm
static const uint32_t NEXT_RUN_MINUTES = 2;

uint32_t runCounter = 0;

// Pomp-instantie
Pump pump(PUMP_PIN_FORWARD, PUMP_PIN_REVERSE);

void setup()
{
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("=== TEST: Pump + Clock + DeepSleep via DS3231 ===");

  // Wake-up oorzaak loggen
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  Serial.print("Wake-up cause: ");
  Serial.println((int)cause);

  // NVS / Preferences
  prefs.begin("edna", false);
  runCounter = prefs.getULong("runCounter", 0);
  runCounter++;
  prefs.putULong("runCounter", runCounter);
  Serial.printf("Run counter: %lu\n", (unsigned long)runCounter);

  // RTC + klok
  clockBegin();

    Serial.print("Compile time: ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);

  Serial.print("RTC time: ");
  clockLogNow();

  Serial.print("Huidige tijd (RTC): ");
  clockLogNow();

  // INT-pin van DS3231 als input met pull-up
  pinMode((int)RTC_INT_PIN, INPUT_PULLUP);

  // Pomp-hardware (gesimuleerd of echt, afhankelijk van PumpHardware.cpp)
  pumpHardwareBegin(PUMP_PIN_FORWARD, PUMP_PIN_REVERSE);
  pump.begin();

  // --- Pomp-sequentie ---
  Serial.println("Start pomp-sequentie: forward -> stop -> reverse -> stop");

  Serial.println("Forward ON");
  pump.startPump(PUMP_POWER);
  delay(FORWARD_MS);

  Serial.println("Stop");
  pump.stopPump();
  delay(PAUSE_MS);

  Serial.println("Reverse ON");
  pump.reverseDirection(PUMP_POWER);
  delay(REVERSE_MS);

  Serial.println("Stop");
  pump.stopPump();

  // --- RTC-alarm over 2 minuten ---
  Serial.printf("Plan RTC-alarm over %lu minuten...\n", (unsigned long)NEXT_RUN_MINUTES);
  if (!clockScheduleAlarmInMinutes(NEXT_RUN_MINUTES))
  {
    Serial.println("Kon RTC-alarm niet plannen, geen deep sleep.");
    return;
  }

  // Nog een keer tijd loggen ter controle
  Serial.print("Na het plannen is de tijd: ");
  clockLogNow();

  Serial.println("Ga nu in deep sleep, wake via DS3231 INT (low).");
  delay(200); // even tijd om de log uit te sturen

  // Let op: we gebruiken HIER GEEN DEBUG_PUMP_MODE-guard meer.
  // Deep sleep is nu altijd actief in deze test.
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
  esp_sleep_enable_ext0_wakeup(RTC_INT_PIN, 0); // 0 = low niveau (INT/SQW active low)

  esp_deep_sleep_start();
}

void loop()
{
  // wordt nooit bereikt; alles gebeurt in setup()
}
