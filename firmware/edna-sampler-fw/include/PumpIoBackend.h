#pragma once
#include <Arduino.h>

class PumpIoBackend {
public:
  virtual ~PumpIoBackend() = default;

  // Moet fail-safe zijn: outputs low + correct als output.
  virtual bool begin() = 0;

  // Zet 1 kanaal (0..N-1) naar HIGH/LOW.
  virtual void writeChannel(uint8_t channel, bool level) = 0;

  // Fail-safe: alles uit (LOW).
  virtual void allOff() = 0;
};
