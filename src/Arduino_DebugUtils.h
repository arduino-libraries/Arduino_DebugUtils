/*
   This file is part of Arduino_DebugUtils.

   Copyright 2019 ARDUINO SA (http://www.arduino.cc/)

   This software is released under the GNU General Public License version 3,
   which covers the main part of arduino-cli.
   The terms of this license can be found at:
   https://www.gnu.org/licenses/gpl-3.0.en.html

   You can be released from the requirements of the above licenses by purchasing
   a commercial license. Buying such a license is mandatory if you want to modify or
   otherwise use the software for commercial activities involving the Arduino
   software without disclosing the source code of your own applications. To purchase
   a commercial license, send an email to license@arduino.cc.
*/

#ifndef ARDUINO_DEBUG_UTILS_H_
#define ARDUINO_DEBUG_UTILS_H_

/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include <Arduino.h>

#include <stdarg.h>

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

static int const DBG_NONE    = -1;
static int const DBG_ERROR   =  0;
static int const DBG_WARNING =  1;
static int const DBG_INFO    =  2;
static int const DBG_DEBUG   =  3;
static int const DBG_VERBOSE =  4;

void setDebugMessageLevel(int const debug_level);

/******************************************************************************
   CLASS DECLARATION
 ******************************************************************************/

class Arduino_DebugUtils {

  public:

    Arduino_DebugUtils();

    void setDebugLevel(int const debug_level);

    void setDebugOutputStream(Stream * stream);

    void timestampOn();
    void timestampOff();

    void print(int const debug_level, const char * fmt, ...);
    void print(int const debug_level, const __FlashStringHelper * fmt, ...);


  private:

    bool      _timestamp_on;
    int       _debug_level;
    Stream *  _debug_output_stream;

    void vPrint(char const * fmt, va_list args);
    void printTimestamp();
    bool shouldPrint(int const debug_level) const;

};

/******************************************************************************
   EXTERN
 ******************************************************************************/

extern Arduino_DebugUtils Debug;

#endif /* ARDUINO_DEBUG_UTILS_H_ */