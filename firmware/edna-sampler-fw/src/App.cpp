#include "App.h"

void App::begin()
{
    Serial.begin(115200);
    delay(500);

    Serial.println();
    Serial.println("=== eDNA Sampler 24x ===");
    Serial.println("Firmware bootstrap gestart (zonder hardware)");
    Serial.println();

    _lastHeartbeat = millis();

}

void App::loop()
{
    const unsigned long now = millis();

    // Simuleer een "virtueel systeem" dat hartslagen logt
    if (now - _lastHeartbeat >= _heartbeatIntervalMs)
    {
        _lastHeartbeat = now;

        Serial.print("[");
        Serial.print(now);
        Serial.println(" ms] Heartbeat: systeemlogica draait (simulatie)");
    }

    // Hier kun je later simulaties toevoegen, bv:
    // - virtuele pomptaken
    // - geplande sampling-runs
    // - test van scheduler-logica

}
