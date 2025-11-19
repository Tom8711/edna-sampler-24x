#include "Pump.h"
#include "PumpHardware.h"

Pump::Pump(uint8_t pinForward, uint8_t pinReverse)
    : _pinForward(pinForward),
      _pinReverse(pinReverse)
{}

void Pump::begin() {
    pumpHardwareBegin(_pinForward, _pinReverse);
}

void Pump::startPump(uint32_t power) {
    pumpHardwareStartForward(_pinForward, _pinReverse, power);
}

void Pump::reverseDirection(uint32_t power) {
    pumpHardwareStartReverse(_pinForward, _pinReverse, power);
}

void Pump::stopPump() {
    pumpHardwareStop(_pinForward, _pinReverse);
}

void Pump::stopAllPins() {
    analogWrite(_pinForward, 0);
    analogWrite(_pinReverse, 0);
}
