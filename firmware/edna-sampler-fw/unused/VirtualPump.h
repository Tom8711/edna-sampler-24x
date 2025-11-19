// NOTE: This file is currently unused.
// It is kept for future development of a pump simulator.

#pragma once
#include <stdint.h>

class VirtualPump {
public:
    explicit VirtualPump(uint8_t id) : _id(id) {}

    void on() {
        _isOn = true;
    }

    void off() {
        _isOn = false;
    }

    bool isOn() const { return _isOn; }
    uint8_t id() const { return _id; }

private:
    uint8_t _id;
    bool _isOn = false;
};
