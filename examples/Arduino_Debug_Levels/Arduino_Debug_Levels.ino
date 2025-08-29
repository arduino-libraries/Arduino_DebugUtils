#define DEBUG_LEVEL DEBUG_LEVEL_INFO
#include <Arduino_DebugUtils.h>

void setup() {
  Serial.begin(9600);
  Debug.timestampOn();
  Debug.debugLabelOn();
  Debug.setDebugLevel(DBG_ALL);
}

int i = 0;

void loop() {
  DEBUG_ERROR("i = %d", i);
  DEBUG_WARNING("i = %d", i);
  DEBUG_INFO("i = %d", i);
  DEBUG_DEBUG("i = %d", i);
  DEBUG_VERBOSE("i = %d", i);

  Serial.println();
  i++;
  delay(1000);
}
