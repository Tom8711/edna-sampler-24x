#pragma once
#include <Arduino.h>

// Hardware-pinnen voor de pomp
constexpr uint8_t PUMP_PIN_FORWARD = 5;
constexpr uint8_t PUMP_PIN_REVERSE = 6;

// Deze functies worden door Pump.cpp gebruikt.
// Ze doen óf echte hardware-aansturing, óf alleen loggen (debug mode).

void pumpHardwareBegin(uint8_t pinForward, uint8_t pinReverse);
void pumpHardwareStartForward(uint8_t pinForward, uint8_t pinReverse, uint32_t power);
void pumpHardwareStartReverse(uint8_t pinForward, uint8_t pinReverse, uint32_t power);
void pumpHardwareStop(uint8_t pinForward, uint8_t pinReverse);

// Optioneel: IO-expander backend (alleen gebruikt als PUMP_USE_IOEXPANDER is gedefinieerd)
class PumpIoBackend;
void pumpHardwareSetBackend(PumpIoBackend* backend);

