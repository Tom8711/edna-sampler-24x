#include "Mcp23017Backend.h"

// MCP23017 registers (bank=0 default)
static constexpr uint8_t REG_IODIRA = 0x00;
static constexpr uint8_t REG_IODIRB = 0x01;
static constexpr uint8_t REG_OLATA  = 0x14;
static constexpr uint8_t REG_OLATB  = 0x15;

Mcp23017Backend::Mcp23017Backend(uint8_t i2cAddr, uint8_t sdaPin, uint8_t sclPin)
: _addr(i2cAddr), _sda(sdaPin), _scl(sclPin) {}

bool Mcp23017Backend::writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(_addr);
  Wire.write(reg);
  Wire.write(val);
  return (Wire.endTransmission() == 0);
}

bool Mcp23017Backend::readReg(uint8_t reg, uint8_t &val) {
  Wire.beginTransmission(_addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false; // repeated start
  if (Wire.requestFrom((int)_addr, 1) != 1) return false;
  val = (uint8_t)Wire.read();
  return true;
}

void Mcp23017Backend::commitOutputs() {
  // Schrijf beide latches
  (void)writeReg(REG_OLATA, (uint8_t)(_olatShadow & 0xFF));
  (void)writeReg(REG_OLATB, (uint8_t)((_olatShadow >> 8) & 0xFF));
}

bool Mcp23017Backend::begin() {
  Wire.begin(_sda, _scl);

  // Quick presence check: lees IODIRA
  uint8_t tmp = 0;
  if (!readReg(REG_IODIRA, tmp)) {
    Serial.println(F("[IOX] FOUT: MCP23017 niet bereikbaar op I2C"));
    return false;
  }

  // FAIL-SAFE: eerst outputs low in shadow + commit
  _olatShadow = 0;
  commitOutputs();

  // Alles OUTPUT: IODIR=0
  if (!writeReg(REG_IODIRA, 0x00) || !writeReg(REG_IODIRB, 0x00)) {
    Serial.println(F("[IOX] FOUT: IODIR schrijven mislukt"));
    return false;
  }

  // Nogmaals outputs low commit (belt & suspenders)
  _olatShadow = 0;
  commitOutputs();

  Serial.print(F("[IOX] MCP23017 OK @0x"));
  Serial.println(_addr, HEX);
  return true;
}

void Mcp23017Backend::allOff() {
  _olatShadow = 0;
  commitOutputs();
}

void Mcp23017Backend::writeChannel(uint8_t channel, bool level) {
  if (channel >= 16) return;
  const uint16_t mask = (uint16_t)(1u << channel);
  if (level) _olatShadow |= mask;
  else       _olatShadow &= (uint16_t)~mask;
  commitOutputs();
}
