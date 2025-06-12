#include "Dispensador.h"

void setup() {
  Serial.begin(115200);
  dispensadorSetup();
}

void loop() {
  dispensadorLoop();
}