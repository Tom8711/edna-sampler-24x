#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "PumpIoBackend.h"

class Mcp23017Backend : public PumpIoBackend {
public:
  Mcp23017Backend(uint8_t i2cAddr, uint8_t sdaPin, uint8_t sclPin);

  bool begin() override;
  void writeChannel(uint8_t channel, bool level) override;
  void allOff() override;

private:
  uint8_t _addr;
  uint8_t _sda, _scl;
  uint16_t _olatShadow = 0; // 16-bit output latch shadow (LOW=off)

  bool writeReg(uint8_t reg, uint8_t val);
  bool readReg(uint8_t reg, uint8_t &val);
  void commitOutputs();
};
