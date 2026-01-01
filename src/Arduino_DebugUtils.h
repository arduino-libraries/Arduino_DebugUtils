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

#ifndef ARDUINO_DEBUG_UTILS_MAX_MODULES
// The global debug level is stored at index 0, so the maximum number of modules is ARDUINO_DEBUG_UTILS_MAX_MODULES+1
  #define ARDUINO_DEBUG_UTILS_MAX_MODULES 20 
#endif

#ifndef ARDUINO_DEBUG_UTILS_MODULE_LABEL_SIZE
  #define ARDUINO_DEBUG_UTILS_MODULE_LABEL_SIZE 10 // Define the maximum size of the module label. Can be overridden in the project
#endif

#define COMMAND_BUFFER_SIZE 20

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
    
    void setDebugOutputStream(Stream * stream); // Set the debug output stream - retained for backward compatibility
    void setDebugIOStream(Stream * stream); // Set the debug input/output stream

    void setDebugLevel(int const debug_level);  // Set the global debug level
    void setDebugLevel(int const module_id, int const debug_level); // Set the debug level for a specific module
    void setDebugLevelAll(int const debug_level); // Set the same debug level for all modules
    int  getDebugLevel() const; // Get the global debug level
    int  getDebugLevel(int const module_id) const; // Get the debug level for a specific module
    String getDebugLevelLabel(int const debug_level) const; // Get the debug level label

    void setModuleLabel(int const module_id, const char* label); // Set the label for a specific module
    String getModuleLabel(int const module_id); // Get the label for a specific module
    
    void moduleLabelsOn(); // Turn on display of module labels
    void moduleLabelsOff(); // Turn off display of module labels

    // void moduleLabelOn(const int module_id); // Turn on display of the label for a specific module
    // void moduleLabelOff(const int module_id); // Turn off display of the label for a specific module

    void timestampOn(); // Turn on display of timestamp
    void timestampOff(); // Turn off display of timestamp
    void formatTimestampOn(); // Turn on display of formatted timestamp - show [HH:MM:SS.mmm] 
    void formatTimestampOff(); // Turn off display of formatted timestamp - show [ms]

    void newlineOn(); // Turn on newline after each print
    void newlineOff(); // Turn off newline after each print

    void debugLabelOn();  // Turn on display of debug level label
    // void debugLabelOn(int const debug_level); // Turn on display of debug level label for a specific debug level - Future use
    void debugLabelOff(); // Turn off display of debug level label
    // void debugLabelOff(int const debug_level); // Turn off display of debug level label for a specific debug level - Future use

    void print(int const debug_level, const char * fmt, ...);
    void print(int const debug_level, const __FlashStringHelper * fmt, ...);

    void print(int const module_id, int const debug_level, const char * fmt, ...);
    void print(int const module_id, int const debug_level, const __FlashStringHelper * fmt, ...);

    void processDebugConfigCommand();   

  private:

    Stream *  _debug_io_stream;

    char      _commandBuffer[COMMAND_BUFFER_SIZE]; // Buffer to store config commands  

    bool      _timestamp_on;
    bool      _format_timestamp_on; // Format the timestamp as [HH:MM:SS.mmm] instead of [ms]
    bool      _newline_on;
    bool      _print_debug_level_label; // Display the debug level label for each print
    bool      _print_module_labels; // Display the module label for each print

    // int    _debug_global_level; // Not needed - stored as index 0 in _moduleDebugLevel
    int       _moduleDebugLevel[ARDUINO_DEBUG_UTILS_MAX_MODULES+1];  // Array to store debug levels for each module. Index 0 is the global debug level
    char       _moduleLabel[ARDUINO_DEBUG_UTILS_MAX_MODULES+1][ARDUINO_DEBUG_UTILS_MODULE_LABEL_SIZE];  // Array to store labels for each module
    //bool     _moduleLabelOn[ARDUINO_DEBUG_UTILS_MAX_MODULES+1];  // Array to store if each module label is on or off - Future use
    
    // int getModuleIndex(int const module_id);  // Helper function to get the module index

    void vPrint(char const * fmt, va_list args);
    void printTimestamp();
    void printDebugLabel(int const debug_level);
    bool shouldPrint(int const debug_level) const;
    bool shouldPrint(int const module_id, int const debug_level) const;

    void printDebugStatus();

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
