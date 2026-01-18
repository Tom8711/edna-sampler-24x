#include "Clock.h"

#include <Wire.h>
#include <RTClib.h>

static RTC_DS3231 rtc;
static bool rtcOk = false;

void clockBegin()
{
  // I2C starten op de gekozen pins
  Wire.begin(CLOCK_I2C_SDA, CLOCK_I2C_SCL);

  Serial.println(F("[RTC] I2C gestart op IO9 (SDA) en IO10 (SCL)"));

  if (!rtc.begin())
  {
    Serial.println(F("[RTC] FOUT: DS3231 niet gevonden op I2C-bus!"));
    rtcOk = false;
    return;
  }

  rtcOk = true;
  Serial.println(F("[RTC] DS3231 gevonden en geïnitialiseerd."));
  
  if (rtc.lostPower())
  {
    Serial.println(F("[RTC] Waarschuwing: RTC heeft voeding verloren, tijd nog niet ingesteld."));
    // Je kunt hier automatisch clockSetCompileTime() aanroepen als je dat wilt:
    // clockSetCompileTime();
  }
}

bool clockNow(ClockDateTime &out)
{
  if (!rtcOk)
  {
    return false;
  }

  DateTime now = rtc.now();

  out.year = now.year();
  out.month = now.month();
  out.day = now.day();
  out.hour = now.hour();
  out.minute = now.minute();
  out.second = now.second();

  return true;
}

void clockLogNow()
{
  ClockDateTime t;
  if (!clockNow(t))
  {
    Serial.println(F("[RTC] Geen geldige tijd (RTC niet klaar?)"));
    return;
  }

  char buf[32];
  snprintf(buf, sizeof(buf), "%04u-%02u-%02u %02u:%02u:%02u",
           t.year, t.month, t.day,
           t.hour, t.minute, t.second);

  Serial.print(F("[RTC] Nu: "));
  Serial.println(buf);
}

void clockSetCompileTime()
{
  if (!rtcOk)
  {
    Serial.println(F("[RTC] Kan compile-tijd niet zetten: RTC niet klaar."));
    return;
  }

  // Stel tijd in op het moment dat de firmware is gecompileerd.
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Serial.println(F("[RTC] Tijd ingesteld op compile-tijd."));
}

bool clockNowPlusMinutes(ClockDateTime &out, uint32_t minutes, uint32_t hours)
{
  if (!rtcOk)
  {
    Serial.println(F("[RTC] Kan clockNowPlusMinutes niet uitvoeren: RTC niet klaar."));
    return false;
  }

  DateTime now = rtc.now();

  // TimeSpan(dagen, uren, minuten, seconden)
  TimeSpan delta(0, hours, minutes, 0);
  DateTime future = now + delta;

  out.year = future.year();
  out.month = future.month();
  out.day = future.day();
  out.hour = future.hour();
  out.minute = future.minute();
  out.second = future.second();

  return true;
}

bool clockScheduleAlarmAt(const ClockDateTime &t)
{
  if (!rtcOk)
  {
    Serial.println(F("[RTC] Kan alarm niet zetten: RTC niet klaar."));
    return false;
  }

  DateTime target(t.year, t.month, t.day, t.hour, t.minute, t.second);

  // Eerst alle oude alarm-flags weg en SQW uit
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.writeSqwPinMode(DS3231_OFF);

  // Alarm1 op "match minute" (jaar/maand/dag/uur/minuut)
  rtc.setAlarm1(
      target,
      DS3231_A1_Hour);

  char buf[32];
  snprintf(
      buf, sizeof(buf),
      "%04u-%02u-%02u %02u:%02u:%02u",
      t.year, t.month, t.day,
      t.hour, t.minute, t.second);
  Serial.print(F("[RTC] Alarm1 gezet op: "));
  Serial.println(buf);

  return true;
}

bool clockScheduleAlarmInMinutes(uint32_t minutes, uint32_t hours)
{
  ClockDateTime future{};
  if (!clockNowPlusMinutes(future, minutes, hours))
  {
    return false;
  }
  return clockScheduleAlarmAt(future);
}
