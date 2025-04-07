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

void setDebugMessageLevel(int const debug_level);
int  getDebugMessageLevel();

#define DEBUG_LEVEL_NONE                   0x0000
#define DEBUG_LEVEL_ERROR                  0x0001
#define DEBUG_LEVEL_WARNING                0x0003
#define DEBUG_LEVEL_INFO                   0x0007
#define DEBUG_LEVEL_DEBUG                  0x000F
#define DEBUG_LEVEL_VERBOSE                0x001F
#define DEBUG_LEVEL_ALL                    0xFFFF

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
   CONSTANTS
 ******************************************************************************/

static constexpr int DBG_NONE    = DEBUG_LEVEL_NONE;
static constexpr int DBG_ERROR   = DEBUG_LEVEL_ERROR;
static constexpr int DBG_WARNING = DEBUG_LEVEL_WARNING;
static constexpr int DBG_INFO    = DEBUG_LEVEL_INFO;
static constexpr int DBG_DEBUG   = DEBUG_LEVEL_DEBUG;
static constexpr int DBG_VERBOSE = DEBUG_LEVEL_VERBOSE;
static constexpr int DBG_ALL     = DEBUG_LEVEL_ALL;

/******************************************************************************
   EXTERN
 ******************************************************************************/

extern Arduino_DebugUtils Debug;

/**************************************************************************************
 * DEFINE
 **************************************************************************************/


#ifndef DEBUG_LEVEL
#  define DEBUG_LEVEL DEBUG_LEVEL_ALL
#endif

#if !defined(DEBUG_ERROR) && ((DEBUG_LEVEL & DEBUG_LEVEL_ERROR) == DEBUG_LEVEL_ERROR)
#  define DEBUG_ERROR(fmt, ...)       Debug.print(DBG_ERROR, fmt, ## __VA_ARGS__)
#else
#  define DEBUG_ERROR(fmt, ...)       (void) 0
#endif

#if !defined(DEBUG_WARNING) && ((DEBUG_LEVEL & DEBUG_LEVEL_WARNING) == DEBUG_LEVEL_WARNING)
#  define DEBUG_WARNING(fmt, ...)     Debug.print(DBG_WARNING, fmt, ## __VA_ARGS__)
#else
#  define DEBUG_WARNING(fmt, ...)     (void) 0
#endif

#if !defined(DEBUG_INFO) && ((DEBUG_LEVEL & DEBUG_LEVEL_INFO) == DEBUG_LEVEL_INFO)
#  define DEBUG_INFO(fmt, ...)        Debug.print(DBG_INFO, fmt, ## __VA_ARGS__)
#else
#  define DEBUG_INFO(fmt, ...)        (void) 0
#endif

#if !defined(DEBUG_DEBUG) && ((DEBUG_LEVEL & DEBUG_LEVEL_DEBUG) == DEBUG_LEVEL_DEBUG)
#  define DEBUG_DEBUG(fmt, ...)       Debug.print(DBG_DEBUG, fmt, ## __VA_ARGS__)
#else
#  define DEBUG_DEBUG(fmt, ...)       (void) 0
#endif

#if !defined(DEBUG_VERBOSE) && ((DEBUG_LEVEL & DEBUG_LEVEL_VERBOSE) == DEBUG_LEVEL_VERBOSE)
#  define DEBUG_VERBOSE(fmt, ...)     Debug.print(DBG_VERBOSE, fmt, ## __VA_ARGS__)
#else
#  define DEBUG_VERBOSE(fmt, ...)     (void) 0
#endif

#endif /* ARDUINO_DEBUG_UTILS_H_ */
