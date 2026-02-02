#include <Arduino.h>
#include "Pump.h"
#include "PumpHardware.h"
#include "Mcp23017Backend.h"
#include "Clock.h" // alleen voor I2C pin-constants, geen RTC gebruik

// IO-expander config
static constexpr uint8_t IOX_ADDR = 0x20;

// Mapping 2 pompen (channels op MCP23017)
static constexpr uint8_t P1_FWD = 0;
static constexpr uint8_t P1_REV = 1;
static constexpr uint8_t P2_FWD = 2;
static constexpr uint8_t P2_REV = 3;

static Mcp23017Backend io(IOX_ADDR, CLOCK_I2C_SDA, CLOCK_I2C_SCL);

// Pump gebruikt “pins”; bij IOX zijn dit “channels”
Pump pump1(P1_FWD, P1_REV);
Pump pump2(P2_FWD, P2_REV);

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(F("=== TEST: IO-expander + 2 pumps ==="));

  pumpHardwareSetBackend(&io);

  if (!io.begin()) {
    Serial.println(F("STOP: IO-expander init faalde (fail-safe: geen pompactie)."));
    while (true) delay(1000);
  }

  // Fail-safe extra: alles uit vóór begin()
  io.allOff();

  pump1.begin();
  pump2.begin();

  Serial.println(F("Start sequentie: P1 forward 1s -> stop -> P2 reverse 1s -> stop"));
}

void loop() {
  Serial.println(F("[P1] FORWARD"));
  pump1.startPump(255);
  delay(1000);

  Serial.println(F("[P1] STOP"));
  pump1.stopPump();
  delay(500);

  Serial.println(F("[P2] REVERSE"));
  pump2.reverseDirection(255);
  delay(1000);

  Serial.println(F("[P2] STOP"));
  pump2.stopPump();
  delay(1500);
}
