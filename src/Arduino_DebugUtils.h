/*
   This file is part of ArduinoDebugUtils.

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

static int const DEBUG_LVL_NONE    = -1;
static int const DEBUG_LVL_ERROR   =  0;
static int const DEBUG_LVL_WARNING =  1;
static int const DEBUG_LVL_INFO    =  2;
static int const DEBUG_LVL_DEBUG   =  3;
static int const DEBUG_LVL_VERBOSE =  4;

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


  private:

    bool      _timestamp_on;
    int       _debug_level;
    Stream *  _debug_output_stream;

    void vPrint(char const * fmt, va_list args);

};

/******************************************************************************
   EXTERN
 ******************************************************************************/

extern Arduino_DebugUtils Debug;

#endif /* ARDUINO_DEBUG_UTILS_H_ */