#include <Arduino.h>
#include <Preferences.h>
#include "esp_sleep.h"

#include "Pump.h"
#include "PumpHardware.h"
#include "Mcp23017Backend.h"
#include "Clock.h"

Preferences prefs;

// DS3231 INT/SQW -> ESP32 GPIO4 (zelfde als je huidige test)
static const gpio_num_t RTC_INT_PIN = GPIO_NUM_4;

static constexpr uint8_t IOX_ADDR = 0x20;
static Mcp23017Backend io(IOX_ADDR, CLOCK_I2C_SDA, CLOCK_I2C_SCL);

// 2 pompen via channels
static constexpr uint8_t P1_FWD = 0, P1_REV = 1;
static constexpr uint8_t P2_FWD = 2, P2_REV = 3;

Pump pump1(P1_FWD, P1_REV);
Pump pump2(P2_FWD, P2_REV);

static const int PUMP_POWER = 255;
static const uint32_t NEXT_RUN_MINUTES = 2;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println(F("=== TEST: IOX + 2 Pumps + Clock + DeepSleep ==="));

  // Wake cause
  Serial.print(F("Wake-up cause: "));
  Serial.println((int)esp_sleep_get_wakeup_cause());

  // Preferences counter
  prefs.begin("edna", false);
  uint32_t runCounter = prefs.getULong("runCounter", 0) + 1;
  prefs.putULong("runCounter", runCounter);
  Serial.printf("Run counter: %lu\n", (unsigned long)runCounter);

  // RTC init
  clockBegin();
  Serial.print(F("RTC time: "));
  clockLogNow();

  pinMode((int)RTC_INT_PIN, INPUT_PULLUP);

  // IOX init + fail-safe
  pumpHardwareSetBackend(&io);
  if (!io.begin()) {
    Serial.println(F("IOX init faalde -> fail-safe: geen pompactie, geen deep sleep."));
    return;
  }
  io.allOff();

  // Pumps
  pump1.begin();
  pump2.begin();

  // Test-sequentie (max 1 tegelijk)
  Serial.println(F("P1 forward 2s -> stop -> P2 forward 2s -> stop"));
  pump1.startPump(PUMP_POWER);
  delay(2000);
  pump1.stopPump();
  delay(500);

  pump2.startPump(PUMP_POWER);
  delay(2000);
  pump2.stopPump();

  // Fail-safe vóór slaap
  io.allOff();

  // Plan alarm
  Serial.printf("Plan RTC-alarm over %lu minuten...\n", (unsigned long)NEXT_RUN_MINUTES);
  if (!clockScheduleAlarmInMinutes(NEXT_RUN_MINUTES)) {
    Serial.println(F("Kon RTC-alarm niet plannen -> geen deep sleep."));
    return;
  }

  Serial.println(F("Ga nu in deep sleep (wake via DS3231 INT low)."));
  delay(200);

  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
  esp_sleep_enable_ext0_wakeup(RTC_INT_PIN, 0);
  esp_deep_sleep_start();
}

void loop() {}
