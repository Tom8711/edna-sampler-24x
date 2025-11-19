#pragma once

#include <Arduino.h>


class App {
public:
    void begin();
    void loop();

private:
    unsigned long _lastHeartbeat = 0;
    unsigned long _heartbeatIntervalMs = 2000; // elke 2 seconden loggen
    

};
