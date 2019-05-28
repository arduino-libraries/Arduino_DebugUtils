#include "Arduino_DebugUtils.h"

void setup() {
  Serial.begin(9600);
  Debug.timestampOn();
}

int i = 0;

void loop() {
  Debug.print(DBG_INFO, "i = %d", i);
  i++;
  delay(1000);
}
