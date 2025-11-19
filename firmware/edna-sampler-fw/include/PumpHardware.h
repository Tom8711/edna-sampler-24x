#pragma once
#include <Arduino.h>

// Deze functies worden door Pump.cpp gebruikt.
// Ze doen óf echte hardware-aansturing, óf alleen loggen (debug mode).

void pumpHardwareBegin(uint8_t pinForward, uint8_t pinReverse);
void pumpHardwareStartForward(uint8_t pinForward, uint8_t pinReverse, uint32_t power);
void pumpHardwareStartReverse(uint8_t pinForward, uint8_t pinReverse, uint32_t power);
void pumpHardwareStop(uint8_t pinForward, uint8_t pinReverse);
