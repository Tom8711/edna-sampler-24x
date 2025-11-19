// Let op: echte hardware-aansturing zit in PumpHardware.cpp.
// Via DEBUG_PUMP_MODE kun je een debug-simulator inschakelen.

#pragma once
#include <Arduino.h>

class Pump {
public:
    Pump(uint8_t pinForward, uint8_t pinReverse);

    void begin();
    void startPump(uint32_t power = 255);   // power = PWM intensiteit (future proof)
    void reverseDirection(uint32_t power = 255);
    void stopPump();

private:
    uint8_t _pinForward;
    uint8_t _pinReverse;

    void stopAllPins();
};
