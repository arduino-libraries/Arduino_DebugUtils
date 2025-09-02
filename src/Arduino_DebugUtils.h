/*
  This file is part of the Arduino_DebugUtils library.

  Copyright (c) 2019 Arduino SA

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
int  getDebugMessageLevel();

/******************************************************************************
  CLASS DECLARATION
 ******************************************************************************/

class Arduino_DebugUtils {

  public:

    Arduino_DebugUtils();

    void setDebugLevel(int const debug_level);
    int  getDebugLevel() const;

    void setDebugOutputStream(Stream * stream);

    void timestampOn();
    void timestampOff();

    void newlineOn();
    void newlineOff();

    void debugLabelOn();
    void debugLabelOff();

    void formatTimestampOn();
    void formatTimestampOff();

    void print(int const debug_level, const char * fmt, ...);
    void print(int const debug_level, const __FlashStringHelper * fmt, ...);


  private:

    bool      _timestamp_on;
    bool      _newline_on;
    bool      _print_debug_label;
    bool      _format_timestamp_on;
    int       _debug_level;
    Stream *  _debug_output_stream;

    void vPrint(char const * fmt, va_list args);
    void printTimestamp();
    void printDebugLabel(int const debug_level);
    bool shouldPrint(int const debug_level) const;

};

/******************************************************************************
  EXTERN
 ******************************************************************************/

extern Arduino_DebugUtils Debug;

/******************************************************************************
  DEFINE
 ******************************************************************************/

#ifndef DEBUG_ERROR
#  define DEBUG_ERROR(fmt, ...) Debug.print(DBG_ERROR, fmt, ## __VA_ARGS__)
#endif

#ifndef DEBUG_WARNING
#  define DEBUG_WARNING(fmt, ...) Debug.print(DBG_WARNING, fmt, ## __VA_ARGS__)
#endif

#ifndef DEBUG_INFO
#  define DEBUG_INFO(fmt, ...) Debug.print(DBG_INFO, fmt, ## __VA_ARGS__)
#endif

#ifndef DEBUG_DEBUG
#  define DEBUG_DEBUG(fmt, ...) Debug.print(DBG_DEBUG, fmt, ## __VA_ARGS__)
#endif

#ifndef DEBUG_VERBOSE
#  define DEBUG_VERBOSE(fmt, ...) Debug.print(DBG_VERBOSE, fmt, ## __VA_ARGS__)
#endif

#endif /* ARDUINO_DEBUG_UTILS_H_ */
