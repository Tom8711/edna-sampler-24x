#include "PumpHardware.h"

// Als DEBUG_PUMP_MODE gedefinieerd is, wordt alles gesimuleerd en gelogd.
// Anders gaat het naar de pins.

void pumpHardwareBegin(uint8_t pinForward, uint8_t pinReverse) {
#ifdef DEBUG_PUMP_MODE
    Serial.print("[DEBUG] pumpHardwareBegin: F=");
    Serial.print(pinForward);
    Serial.print(" R=");
    Serial.println(pinReverse);
#else
    pinMode(pinForward, OUTPUT);
    pinMode(pinReverse, OUTPUT);
    analogWrite(pinForward, 0);
    analogWrite(pinReverse, 0);
#endif
}

void pumpHardwareStartForward(uint8_t pinForward, uint8_t pinReverse, uint32_t power) {
#ifdef DEBUG_PUMP_MODE
    Serial.print("[DEBUG] start FORWARD on pins F=");
    Serial.print(pinForward);
    Serial.print(" R=");
    Serial.print(pinReverse);
    Serial.print(" power=");
    Serial.println(power);
#else
    analogWrite(pinReverse, 0);
    analogWrite(pinForward, power);
#endif
}

void pumpHardwareStartReverse(uint8_t pinForward, uint8_t pinReverse, uint32_t power) {
#ifdef DEBUG_PUMP_MODE
    Serial.print("[DEBUG] start REVERSE on pins F=");
    Serial.print(pinForward);
    Serial.print(" R=");
    Serial.print(pinReverse);
    Serial.print(" power=");
    Serial.println(power);
#else
    analogWrite(pinForward, 0);
    analogWrite(pinReverse, power);
#endif
}

void pumpHardwareStop(uint8_t pinForward, uint8_t pinReverse) {
#ifdef DEBUG_PUMP_MODE
    Serial.print("[DEBUG] stop pump on pins F=");
    Serial.print(pinForward);
    Serial.print(" R=");
    Serial.println(pinReverse);
#else
    analogWrite(pinForward, 0);
    analogWrite(pinReverse, 0);
#endif
}
