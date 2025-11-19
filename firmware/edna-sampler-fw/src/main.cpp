// #include <Arduino.h>
// #include "App.h"

// App app;

// void setup() {
//     app.begin();
// }

// void loop() {
//     app.loop();
// }

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(2000);  // even wachten tot USB staat
  Serial.println("=== ESP32-S3 teststart ===");
}

void loop() {
  static uint32_t last = 0;
  uint32_t now = millis();
  if (now - last >= 1000) {
    last = now;
    Serial.print("Heartbeat: ");
    Serial.println(now);
  }
}
