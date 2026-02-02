#include "PumpHardware.h"
#include "PumpIoBackend.h"

static PumpIoBackend *g_backend = nullptr;
void pumpHardwareSetBackend(PumpIoBackend *backend) { g_backend = backend; }

// Als DEBUG_PUMP_MODE gedefinieerd is, wordt alles gesimuleerd en gelogd.
// Anders gaat het naar de pins.

void pumpHardwareBegin(uint8_t pinForward, uint8_t pinReverse)
{
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
#ifdef PUMP_USE_IOEXPANDER
    if (g_backend)
    {
        // pins worden geïnterpreteerd als expander-channels
        g_backend->allOff(); // fail-safe
        return;
    }
#endif
    pinMode(pinForward, OUTPUT);
    pinMode(pinReverse, OUTPUT);
    analogWrite(pinForward, 0);
    analogWrite(pinReverse, 0);

#endif
}

void pumpHardwareStartForward(uint8_t pinForward, uint8_t pinReverse, uint32_t power)
{
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
#ifdef PUMP_USE_IOEXPANDER
    if (g_backend)
    {
        // power: voor nu alleen 0=off, >0=on
        g_backend->writeChannel(pinReverse, false);
        g_backend->writeChannel(pinForward, (power > 0));
        return;
    }
#endif
    analogWrite(pinReverse, 0);
    analogWrite(pinForward, power);
#endif
}

void pumpHardwareStartReverse(uint8_t pinForward, uint8_t pinReverse, uint32_t power)
{
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
#ifdef PUMP_USE_IOEXPANDER
    if (g_backend)
    {
        g_backend->writeChannel(pinForward, false);
        g_backend->writeChannel(pinReverse, (power > 0));
        return;
    }
#endif
    analogWrite(pinForward, 0);
    analogWrite(pinReverse, power);
#endif
}

void pumpHardwareStop(uint8_t pinForward, uint8_t pinReverse)
{
#ifdef DEBUG_PUMP_MODE
    Serial.print("[DEBUG] stop pump on pins F=");
    Serial.print(pinForward);
    Serial.print(" R=");
    Serial.println(pinReverse);
#else
    analogWrite(pinForward, 0);
    analogWrite(pinReverse, 0);
#ifdef PUMP_USE_IOEXPANDER
    if (g_backend)
    {
        g_backend->writeChannel(pinForward, false);
        g_backend->writeChannel(pinReverse, false);
        return;
    }
#endif
    analogWrite(pinForward, 0);
    analogWrite(pinReverse, 0);
#endif
}
