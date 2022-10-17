/*
   Advanced Debug can be helpful in embedded applications when
   there are more than two microcontrollers connected serially
   or a wireless sensor like XBee is connected to the serial port
   that will send data wirelessly to other XBee nodes.

   In boards like Arduino Nano, UNO, or MEGA only one serial port is available,
   therefore additional software serial ports can be made using the
   SoftwareSerial library.
*/

#include "Arduino_DebugUtils.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  mySerial.begin(9600);
  Debug.setDebugOutputStream(&mySerial);
  Debug.setDebugLevel(DBG_VERBOSE);
  Debug.timestampOn();
}

int i = 0;

void loop() {
  DEBUG_VERBOSE("i = %d", i);
  i++;
  delay(1000);
}
