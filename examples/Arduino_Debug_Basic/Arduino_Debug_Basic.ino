#include "Arduino_DebugUtils.h"

void setup() {
  Serial.begin(9600);
  Debug.timestampOn();
}

int i = 0;

void loop() {
  DEBUG_INFO("i = %d", i);
  i++;
  Debug.processDebugConfigCommand();
  delay(1000);
}
