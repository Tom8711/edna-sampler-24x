#pragma once

#include <Arduino.h>

// Eenvoudige eigen tijdstructuur
struct ClockDateTime {
  uint16_t year;
  uint8_t  month;
  uint8_t  day;
  uint8_t  hour;
  uint8_t  minute;
  uint8_t  second;
};

// I2C-pinnen voor de DS3231 op jouw bord
constexpr uint8_t CLOCK_I2C_SDA = 9;   // IO9
constexpr uint8_t CLOCK_I2C_SCL = 10;  // IO10

// RTC initialiseren (I2C + DS3231)
void clockBegin();

// Huidige tijd ophalen; geeft false als RTC niet ok is
bool clockNow(ClockDateTime &out);

// Huidige tijd naar Serial loggen in formaat "YYYY-MM-DD HH:MM:SS"
void clockLogNow();

// Optioneel: RTC-tijd zetten op compile-tijd (handig bij eerste keer instellen)
void clockSetCompileTime();

// Helper: minutes from now
bool clockNowPlusMinutes(ClockDateTime &out, uint32_t minutes, uint32_t hours);

// Alarm plannen op een absolute tijd (Alarm1, minute-match)
bool clockScheduleAlarmAt(const ClockDateTime &t);

// Alarm plannen over X minuten vanaf nu
bool clockScheduleAlarmInMinutes(uint32_t minutes, uint32_t hours);

