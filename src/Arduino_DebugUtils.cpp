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

/******************************************************************************
   INCLUDE
 ******************************************************************************/

#include "Arduino_DebugUtils.h"

/******************************************************************************
   CONSTANTS
 ******************************************************************************/

static int const DEFAULT_DEBUG_LEVEL   = DBG_INFO;
static Stream *  DEFAULT_OUTPUT_STREAM = &Serial;

/******************************************************************************
   CTOR/DTOR
 ******************************************************************************/

Arduino_DebugUtils::Arduino_DebugUtils() {
  timestampOff();
  newlineOn();
  debugLabelOff();
  formatTimestampOff();
  setDebugLevel(DEFAULT_DEBUG_LEVEL);
  setDebugOutputStream(DEFAULT_OUTPUT_STREAM);
}

/******************************************************************************
   PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void Arduino_DebugUtils::setDebugLevel(int const debug_level) {
  _debug_level = debug_level;
}

int Arduino_DebugUtils::getDebugLevel() const {
  return _debug_level;
}

void Arduino_DebugUtils::setDebugOutputStream(Stream * stream) {
  _debug_output_stream = stream;
}

void Arduino_DebugUtils::newlineOn() {
    _newline_on = true;
}

void Arduino_DebugUtils::newlineOff() {
    _newline_on = false;
}

void Arduino_DebugUtils::debugLabelOn() {
  _print_debug_label = true;
}

void Arduino_DebugUtils::debugLabelOff() {
  _print_debug_label = false;
}

void Arduino_DebugUtils::formatTimestampOn() {
  _format_timestamp_on = true;
}

void Arduino_DebugUtils::formatTimestampOff() {
  _format_timestamp_on = false;
}

void Arduino_DebugUtils::timestampOn() {
  _timestamp_on = true;
}

void Arduino_DebugUtils::timestampOff() {
  _timestamp_on = false;
}

void Arduino_DebugUtils::print(int const debug_level, const char * fmt, ...)
{
  if (!shouldPrint(debug_level))
    return;

  if (_print_debug_label)
    printDebugLabel(debug_level);

  if (_timestamp_on)
    printTimestamp();

  va_list args;
  va_start(args, fmt);
  vPrint(fmt, args);
  va_end(args);
}

void Arduino_DebugUtils::print(int const debug_level, const __FlashStringHelper * fmt, ...)
{
  if (!shouldPrint(debug_level))
    return;

  if (_print_debug_label)
    printDebugLabel(debug_level);

  if (_timestamp_on)
    printTimestamp();

  String fmt_str(fmt);

  va_list args;
  va_start(args, fmt);
  vPrint(fmt_str.c_str(), args);
  va_end(args);
}

/******************************************************************************
   PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

void Arduino_DebugUtils::vPrint(char const * fmt, va_list args) {
  // calculate required buffer length
  int msg_buf_size = vsnprintf(nullptr, 0, fmt, args) + 1; // add one for null terminator
#if __STDC_NO_VLA__ == 1
  // in the rare case where VLA is not allowed by compiler, fall back on heap-allocated memory
  char * msg_buf = new char[msg_buf_size];
#else
  char msg_buf[msg_buf_size];
#endif

  vsnprintf(msg_buf, msg_buf_size, fmt, args);

  if (_newline_on) {
    _debug_output_stream->println(msg_buf);
  } else {
    _debug_output_stream->print(msg_buf);
  }

#if __STDC_NO_VLA__ == 1
  // remember to clean up memory
  delete[] msg_buf;
#endif
}

void Arduino_DebugUtils::printTimestamp()
{
  char timestamp[32];

  if (_format_timestamp_on)
  {
    auto    const  msCount      = millis();

    uint16_t const milliseconds = msCount % 1000;           // ms remaining when converted to seconds
    uint16_t const allSeconds   = msCount / 1000;           // total number of seconds to calculate remaining values

    uint16_t const hours            = allSeconds / 3600;    // convert seconds to hours
    uint16_t const secondsRemaining = allSeconds % 3600;    // seconds left over

    uint16_t const minutes  = secondsRemaining / 60 ;       // convert seconds left over to minutes
    uint16_t const seconds  = secondsRemaining % 60;        // seconds left over

    snprintf(timestamp, sizeof(timestamp),                  // "prints" formatted output to a char array (string)
                "[ "
                "%02d:"   //HH:
                "%02d:"   //MM:
                "%02d."   //SS.
                "%03d"    //MMM
                " ] ",
                hours,
                minutes,
                seconds,
                milliseconds
            );
  }
  else
  {
    snprintf(timestamp, sizeof(timestamp), "[ %lu ] ", millis());
  }

  _debug_output_stream->print(timestamp);
}

void Arduino_DebugUtils::printDebugLabel(int const debug_level)
{
  static char const * DEBUG_MODE_STRING[5] =
  {
    "[DBG_ERROR  ] ",
    "[DBG_WARNING] ",
    "[DBG_INFO   ] ",
    "[DBG_DEBUG  ] ",
    "[DBG_VERBOSE] ",
  };

  bool is_valid_debug_level = (debug_level >= DBG_ERROR) && (debug_level <= DBG_VERBOSE);
  if (!is_valid_debug_level)
    return;

  _debug_output_stream->print(DEBUG_MODE_STRING[debug_level]);
}

bool Arduino_DebugUtils::shouldPrint(int const debug_level) const
{
  return ((debug_level >= DBG_ERROR) && (debug_level <= DBG_VERBOSE) && (debug_level <= _debug_level));
}

/******************************************************************************
   CLASS INSTANTIATION
 ******************************************************************************/

Arduino_DebugUtils Debug;

void setDebugMessageLevel(int const debug_level) {
  Debug.setDebugLevel(debug_level);
}

int getDebugMessageLevel() {
  return Debug.getDebugLevel();
}
