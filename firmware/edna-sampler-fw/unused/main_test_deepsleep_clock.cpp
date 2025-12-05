#include <Arduino.h>
#include "Clock.h"
#include <esp_sleep.h>

#define WAKE_PIN GPIO_NUM_4   // DS3231 INT/SQW op deze pin aansluiten (open drain)

// Alarm delay (seconden)
const int WAKE_AFTER_SECONDS = 60;

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println("\n=== Deep Sleep Clock Wake Test ===");

  // Init clock
  clockBegin();

  // Bepaal huidige tijd
  ClockDateTime now;
  if (!clockNow(now)) {
    Serial.println("[RTC] Klok niet OK — Aborting test");
    return;
  }

  // Toon huidige tijd
  Serial.print("[RTC] Nu:     ");
  Serial.print(now.year); Serial.print("-");
  Serial.print(now.month); Serial.print("-");
  Serial.print(now.day); Serial.print(" ");
  Serial.print(now.hour); Serial.print(":");
  Serial.print(now.minute); Serial.print(":");
  Serial.println(now.second);

  // Bereken volgende tijd
  int nextSecond = now.second + WAKE_AFTER_SECONDS;
  int carryMin = nextSecond / 60;

  // Nieuwe tijd voor alarm
  int alarmSecond = nextSecond % 60;
  int alarmMinute = (now.minute + carryMin) % 60;

  Serial.print("[RTC] Wekker ingesteld op +");
  Serial.print(WAKE_AFTER_SECONDS);
  Serial.println(" seconden.");

  Serial.print("[RTC] Alarm tijd:  ");
  Serial.print(now.hour); Serial.print(":");
  Serial.print(alarmMinute); Serial.print(":");
  Serial.println(alarmSecond);

  // DS3231 alarm instellen
  // ALARM1: match seconds + minutes (uren worden niet gematcht)
  // Let op: RTC_DS3231::setAlarm1 argumenten = (DateTime, alarm_mode)
  DateTime alarmTime(now.year, now.month, now.day,
                     now.hour, alarmMinute, alarmSecond);

  rtc.clearAlarm(1);
  rtc.setAlarm1(alarmTime, DS3231_A1_Minute);

  // Alarm pin resetten
  rtc.clearAlarm(1);
  rtc.writeSqwPinMode(DS3231_OFF);

  pinMode(WAKE_PIN, INPUT_PULLUP);

  // ESP32 wakker laten worden via deze GPIO
  esp_sleep_enable_ext0_wakeup(WAKE_PIN, 0);   // DS3231 INT gaat LOW bij alarm

  Serial.println("[SLEEP] Ga nu slapen...");
  delay(200);

  // START deep sleep
  esp_deep_sleep_start();
}

void loop() {
  // Wordt nooit bereikt
}
