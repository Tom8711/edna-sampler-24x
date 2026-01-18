#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);

  Serial.println("15/16 LOW");
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);
  delay(5000);

  Serial.println("15/16 HIGH");
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);
  delay(5000);

  Serial.println("Klaar");
}

void loop() {}
