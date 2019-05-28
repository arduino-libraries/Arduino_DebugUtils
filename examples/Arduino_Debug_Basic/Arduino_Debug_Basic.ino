#include "ArduinoDebugUtils.h"

void setup() {
  Serial.begin(9600);

  ArduinoDebugUtils.timestampOn();
}

int i = 0;

void loop() {

  ArduinoDebugUtils.debugPrint(DEBUG_LVL_INFO, "i = %d", i);
  i++;
  delay(1000);
}
