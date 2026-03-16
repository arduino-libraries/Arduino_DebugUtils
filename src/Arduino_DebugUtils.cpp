/*
  This file is part of the Arduino_DebugUtils library.

  Copyright (c) 2019 Arduino SA

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

  // Initialize all module debug levels to a default value (e.g., DBG_ERROR)
  for (int i = 0; i < ARDUINO_DEBUG_UTILS_MAX_MODULES+1; ++i) {
    _moduleDebugLevel[i] = DBG_ERROR;
  }
  // Set module label for the global module (index 0) to "GLOBAL"
  strncpy(_moduleLabel[0], "GLOBAL", ARDUINO_DEBUG_UTILS_MODULE_LABEL_SIZE);
  // Set all other module labels to "NA"
  for (int i = 1; i < ARDUINO_DEBUG_UTILS_MAX_MODULES+1; ++i) {
    strncpy(_moduleLabel[i], "NA", ARDUINO_DEBUG_UTILS_MODULE_LABEL_SIZE);
  }
}

/******************************************************************************
  PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

/**
 * @brief Sets the debug input/output stream for Arduino_DebugUtils.
 *
 * This function sets the debug input/output and input stream for Arduino_DebugUtils. 
 * The debug output stream is used to send debug messages or logs from the Arduino_DebugUtils library,
 * while the debug input stream is used to receive debug commands.
 * 
 * Note: the function name is retained for backward compatibility with the previous version of the library.
 *
 * @param stream A pointer to the Stream object that will be used as the debug input/output stream.
 */
void Arduino_DebugUtils::setDebugOutputStream(Stream * stream) {
  _debug_io_stream = stream;
  // _debug_io_stream->flush();  // Ensure any buffered data is written
}

/**
 * @brief Sets the debug input/output stream for Arduino_DebugUtils.
 *
 * This function sets the debug input/output and input stream for Arduino_DebugUtils. 
 * The debug output stream is used to send debug messages or logs from the Arduino_DebugUtils library,
 * while the debug input stream is used to receive debug commands.
 * 
 * Note: the function name is retained for backward compatibility with the previous version of the library.
 *
 * @param stream A pointer to the Stream object that will be used as the debug input/output stream.
 */
void Arduino_DebugUtils::setDebugIOStream(Stream * stream) {
  _debug_io_stream = stream;
  // _debug_io_stream->flush();  // Ensure any buffered data is written
}

/**
 * @brief Sets the global debug level (module index 0).
 *
 * This function sets the global debug level (module 0) for Arduino_DebugUtils .
 *
 * @param debug_level The debug level to be set.
 */
void Arduino_DebugUtils::setDebugLevel(int const debug_level) {
  _moduleDebugLevel[0] = debug_level;
}

/**
 * @brief Sets the debug level for a specific module.
 * 
 * This function sets the debug level for the module specified by the module_id parameter.
 * The debug level determines the amount of debug information that will be printed.
 * 
 * @param module_id The ID of the module for which to set the debug level.
 * @param debug_level The debug level to set for the module.
 * 
 * @note If the module is unused, an error message will be printed.
 * @note If the module_id is invalid, an error message will be printed.
 */
void Arduino_DebugUtils::setDebugLevel(int const module_id, int const debug_level) {

    if (module_id != -1) {
        if (strcmp(_moduleLabel[module_id], "NA") == 0) {
            // If the module is unused, print an error message
            _debug_io_stream->println("Error: Attempting to set debug level for an unused module " + String(module_id) + ".");
        } else {
            _moduleDebugLevel[module_id] = debug_level;
        }
    } else {
        // If the module_id is invalid, print an error message
        _debug_io_stream->println("Error: Invalid module_id " + String(module_id) + ". Must be between 1 and ARDUINO_DEBUG_UTILS_MAX_MODULES (" + String(ARDUINO_DEBUG_UTILS_MAX_MODULES) + ").");
    }
}

/**
 * @brief Sets the same debug level for all modules
 *
 * This function sets the same debug level for all modules for Arduino_DebugUtils .
 *
 * @param debug_level The debug level to be set.
 */
void Arduino_DebugUtils::setDebugLevelAll(int const debug_level) {
  // Loop through the array and set each element to the debug_level
  for (int i = 0; i < ARDUINO_DEBUG_UTILS_MAX_MODULES+1; ++i) {
    _moduleDebugLevel[i] = debug_level;
  }
}

/**
 * @brief Gets the global debug level (module index 0).
 *
 * @return The debug level of the module.
 */
int Arduino_DebugUtils::getDebugLevel() const {
  return _moduleDebugLevel[0];
}

/**
 * @brief Retrieves the debug level for a specific module.
 *
 * This function returns the debug level associated with the given module ID.
 *
 * @param module_id The ID of the module for which to retrieve the debug level.
 * @return The debug level of the specified module.
 */
int Arduino_DebugUtils::getDebugLevel(int const module_id) const {
  return _moduleDebugLevel[module_id];
}

/**
 * @brief Converts a debug level to a string label.
 *
 * This function converts a debug level, as defined in Arduino_DebugUtils.h, to a string label.
 *
 * @param debug_level The debug level to be converted.
 * @return A string label that corresponds to the given debug level.
 */
String Arduino_DebugUtils::getDebugLevelLabel(int const debug_level) const {
  switch (debug_level) {
    case DBG_NONE:
      return "NONE";
    case DBG_ERROR:
      return "ERROR";
    case DBG_WARNING:
      return "WARNING";
    case DBG_INFO:
      return "INFO";
    case DBG_DEBUG:
      return "DEBUG";
    case DBG_VERBOSE:
      return "VERBOSE";
    default:
      return "UNKNOWN";
  }
}

/**
 * @brief Sets the label for a module.
 *
 * This function sets the label for the module specified by the module_id parameter.
 * The label is a string that can be used to identify the module in debug messages.
 *
 * @param module_id The ID of the module for which to set the label.
 * @param label The label to be set for the module.
 *
 * @note If the module is unused, an error message will be printed.
 * @note If the module_id is invalid, an error message will be printed.
 */
void Arduino_DebugUtils::setModuleLabel(int const module_id, const char* label) {
  if (module_id >= 0 && module_id < ARDUINO_DEBUG_UTILS_MAX_MODULES+1) {
    // Set the module label if the module_id is valid
    strncpy(_moduleLabel[module_id], label, ARDUINO_DEBUG_UTILS_MODULE_LABEL_SIZE - 1);
    _moduleLabel[module_id][ARDUINO_DEBUG_UTILS_MODULE_LABEL_SIZE - 1] = '\0';  // Ensure null termination
  } else {
    _debug_io_stream->println("Error: Invalid module ID " + String(module_id) + "(" + label + "). Must be between 1 and ARDUINO_DEBUG_UTILS_MAX_MODULES (" + String(ARDUINO_DEBUG_UTILS_MAX_MODULES) + ").");}
}

/**
 * @brief Retrieves the label for a module.
 *
 * This function returns the label associated with the given module ID.
 *
 * @param module_id The ID of the module for which to retrieve the label.
 * @return The label of the specified module.
 */
String Arduino_DebugUtils::getModuleLabel(int const module_id) {
  return _moduleLabel[module_id];
} 

/**
 * @brief Enable printing of newline after each debug message.
 * 
 * This function enables the printing of a newline character after each debug message.
 * By default, this is enabled. To disable it, use the newlineOff() function.
 */
void Arduino_DebugUtils::newlineOn() {
    _newline_on = true;
}

/**
 * @brief Disable printing of newline after each debug message.
 * 
 * This function disables the printing of a newline character after each debug message.
 * To enable it, use the newlineOn() function.
 */
void Arduino_DebugUtils::newlineOff() {
    _newline_on = false;
}

/**
 * @brief Enable printing of debug labels.
 * 
 * This function enables the printing of the debug label before each debug message.
 * By default, this is disabled. To enable it, use the debugLabelOn() function.
 */
void Arduino_DebugUtils::debugLabelOn() {
  _print_debug_level_label = true;
}

/**
 * @brief Disable printing of debug labels.
 * 
 * This function disables the printing of the debug label before each debug message.
 * To enable it, use the debugLabelOn() function.
 */
void Arduino_DebugUtils::debugLabelOff() {
  _print_debug_level_label = false;
}

/**
 * @brief Enable printing of module labels.
 * 
 * This function enables the printing of the module label before each debug message.
 * By default, this is disabled. To enable it, use the moduleLabelsOn() function.
 */
void Arduino_DebugUtils::moduleLabelsOn() {
  _print_module_labels = true;
}

/**
 * @brief Disable printing of module labels.
 * 
 * This function disables the printing of the module label before each debug message.
 * To enable it, use the moduleLabelsOn() function.
 */
void Arduino_DebugUtils::moduleLabelsOff() {
  _print_module_labels = false;
}

/**
 * @brief Enable printing of timestamp.
 * 
 * This function enables the printing of a timestamp before each debug message.
 * By default, this is disabled. To enable it, use the timestampOn() function.
 */
void Arduino_DebugUtils::formatTimestampOn() {
  _format_timestamp_on = true;
}

/**
 * @brief Disable printing of timestamp.
 * 
 * This function disables the printing of a timestamp before each debug message.
 * To enable it, use the timestampOn() function.
 */
void Arduino_DebugUtils::formatTimestampOff() {
  _format_timestamp_on = false;
}

/**
 * @brief Enable printing of timestamp.
 * 
 * This function enables the printing of a timestamp before each debug message.
 * By default, this is disabled. To enable it, use the timestampOn() function.
 */
void Arduino_DebugUtils::timestampOn() {
  _timestamp_on = true;
}

/**
 * @brief Disable printing of timestamp.
 * 
 * This function disables the printing of a timestamp before each debug message.
 * To enable it, use the timestampOn() function.
 */
void Arduino_DebugUtils::timestampOff() {
  _timestamp_on = false;
}

// void Arduino_DebugUtils::moduleLabelOn(const int module_id) {
//   _moduleLabelOn[module_id] = true;
// }

// void Arduino_DebugUtils::moduleLabelOff(const int module_id) {
//   _moduleLabelOn[module_id] = false;
// }

/**
 * @brief Print a formatted debug message.
 * 
 * This function prints a formatted debug message to the debug output stream.
 * The message includes the debug level, module label, timestamp, and the formatted message.
 * 
 * @param debug_level The debug level of the message.
 * @param fmt A format string for the message.
 * @param ... Additional arguments for the format string.
 */
void Arduino_DebugUtils::print(int const debug_level, const char * fmt, ...){
  if (!shouldPrint(debug_level))
    return;

  if (_print_debug_level_label)
    printDebugLabel(debug_level);

  if (_timestamp_on)
    printTimestamp();

  va_list args;
  va_start(args, fmt);
  vPrint(fmt, args);
  va_end(args);

  // _debug_io_stream->flush();  // Ensure any buffered data is written
}

/**
 * @brief Print a formatted debug message from PROGMEM.
 * 
 * This function prints a formatted debug message to the debug output stream.
 * The message includes the debug level, module label, timestamp, and the formatted message.
 * 
 * @param debug_level The debug level of the message.
 * @param fmt A format string for the message stored in PROGMEM.
 * @param ... Additional arguments for the format string.
 */
void Arduino_DebugUtils::print(int const debug_level, const __FlashStringHelper * fmt, ...)
{
  if (!shouldPrint(debug_level))
    return;

  if (_print_debug_level_label)
    printDebugLabel(debug_level);

  if (_timestamp_on)
    printTimestamp();

  String fmt_str(fmt);

  va_list args;
  va_start(args, fmt);
  vPrint(fmt_str.c_str(), args);
  va_end(args);

  // _debug_io_stream->flush();  // Ensure any buffered data is written

}

/**
 * @brief Print a formatted debug message for a specific module.
 * 
 * This function prints a formatted debug message to the debug output stream.
 * The message includes the module label, debug level, timestamp, and the formatted message.
 * 
 * @param module_id The ID of the module associated with the message.
 * @param debug_level The debug level of the message.
 * @param fmt A format string for the message.
 * @param ... Additional arguments for the format string.
 */
void Arduino_DebugUtils::print(int const module_id, int const debug_level, const char * fmt, ...){
  if (!shouldPrint(module_id, debug_level))
    return;

  if (_print_module_labels)
    _debug_io_stream->print("[" + String(_moduleLabel[module_id]) + "] ");
    
  if (_print_debug_level_label)
    printDebugLabel(debug_level);

  if (_timestamp_on)
    printTimestamp();

  va_list args;
  va_start(args, fmt);
  vPrint(fmt, args);
  va_end(args);
  // _debug_io_stream->flush();  // Ensure any buffered data is written
}

/**
 * @brief Print a formatted debug message for a specific module from PROGMEM.
 * 
 * This function prints a formatted debug message to the debug output stream.
 * The message includes the module label, debug level, timestamp, and the formatted message.
 * 
 * @param module_id The ID of the module associated with the message.
 * @param debug_level The debug level of the message.
 * @param fmt A format string for the message stored in PROGMEM.
 * @param ... Additional arguments for the format string.
 */
void Arduino_DebugUtils::print(int const module_id, int const debug_level, const __FlashStringHelper * fmt, ...)
{
  if (!shouldPrint(module_id, debug_level))
    return;

  if (_print_module_labels)
    _debug_io_stream->print("[" + String(_moduleLabel[module_id]) + "] ");
    
  if (_print_debug_level_label)
    printDebugLabel(debug_level);

  if (_timestamp_on)
    printTimestamp();

  String fmt_str(fmt);

  va_list args;
  va_start(args, fmt);
  vPrint(fmt_str.c_str(), args);
  va_end(args);

  // _debug_io_stream->flush();  // Ensure any buffered data is written
}

/**
 * @brief Process a debug command from the serial input stream.
 * 
 * This function checks if there is a new character in the serial input stream
 * and processes it according to the command syntax. The command syntax is as
 * follows:
 * 
 * - `V` or `v`: Set global debug level to VERBOSE.
 * - `D` or `d`: Set global debug level to DEBUG.
 * - `I` or `i`: Set global debug level to INFO.
 * - `W` or `w`: Set global debug level to WARNING.
 * - `E` or `e`: Set global debug level to ERROR.
 * - `N` or `n`: Set global debug level to NONE.
 * - `?`: Print help message with available commands.
 * - `S` or `s`: Print debug status.
 * - `T` or `t`: Toggle timestamps on or off.
 * - `C` or `c`: Toggle new line on or off.
 * - `L` or `l`: Toggle debug level label on or off.
 * - `M` or `m`: Toggle module labels on or off.
 * - `Ax` or `ax`: Set debug level to `x` for all modules.
 * - `x` or `0x` followed by a number between 1 and
 *   `ARDUINO_DEBUG_UTILS_MAX_MODULES`: Set debug level to `x` for the specified
 *   module.
 */
void Arduino_DebugUtils::processDebugConfigCommand()
{
  static size_t bufferIndex = 0; // Index to track buffer position

  // _debug_io_stream->write("."); // Send a dot to the terminal to indicate that the command is being processed

  // Check if the stream is available and has data
  if (_debug_io_stream && _debug_io_stream->available())
  {
    // Read each character from the stream
    char incomingChar = _debug_io_stream->read();
    
    // // Echo the character back to the terminal if it's printable
    // if (incomingChar >= 32 && incomingChar <= 126) { 
    //   _debug_io_stream->write(incomingChar);
    // }
    
    // If it's a newline or carriage return, process the command
    if (incomingChar == '\n' || incomingChar == '\r')
    {
      // Only process if the buffer isn't empty
      if (bufferIndex > 0) {
        // Null-terminate the string
        _commandBuffer[bufferIndex] = '\0'; 
        
      //   // Print a clear message with the command
      //   _debug_io_stream->println();
      //   _debug_io_stream->print("Command: ");
      //   _debug_io_stream->println(_commandBuffer);
      }

      // Check if the command starts with digits followed by a letter
      if (strlen(_commandBuffer) >= 2 && isdigit(_commandBuffer[0]))
      {
        // Extract the digits first (module ID)
        int module_id = atoi(_commandBuffer); // Convert the leading digits to an integer

        // Find the position of the first non-digit character (the command letter)
        size_t i = 0;
        while (isdigit(_commandBuffer[i]))
        {
          i++;
        }

        // Now `i` points to the first non-digit character, which should be the command letter
        char commandChar = tolower(_commandBuffer[i]); // Convert the first non-digit char to lowercase

        // Ensure the module_id is valid (between 0 and ARDUINO_DEBUG_UTILS_MAX_MODULES)
        if (module_id >= 0 && module_id < ARDUINO_DEBUG_UTILS_MAX_MODULES+1)
        {
          if (strcmp(_moduleLabel[module_id], "NA") == 0)
          {
            _debug_io_stream->println("Error: Attempting to set debug level for an unused module.");
          }
          else
          {
            const char *moduleName = _moduleLabel[module_id]; // Get module name
            switch (commandChar)
            {
            case 'v': // VERBOSE
              setDebugLevel(module_id, DBG_VERBOSE);
              _debug_io_stream->print("Module " + String(module_id) + " (" + String(moduleName) + ") debug level set to VERBOSE.");
              break;
            case 'd': // DEBUG
              setDebugLevel(module_id, DBG_DEBUG);
              _debug_io_stream->print("Module " + String(module_id) + " (" + String(moduleName) + ") debug level set to DEBUG.");
              break;
            case 'i': // INFO
              setDebugLevel(module_id, DBG_INFO);
              _debug_io_stream->print("Module " + String(module_id) + " (" + String(moduleName) + ") debug level set to INFO.");
              break;
            case 'w': // WARNING
              setDebugLevel(module_id, DBG_WARNING);
              _debug_io_stream->print("Module " + String(module_id) + " (" + String(moduleName) + ") debug level set to WARNING.");
              break;
            case 'e': // ERROR
              setDebugLevel(module_id, DBG_ERROR);
              _debug_io_stream->print("Module " + String(module_id) + " (" + String(moduleName) + ") debug level set to ERROR.");
              break;
            case 'n': // NONE
              setDebugLevel(module_id, DBG_NONE);
              _debug_io_stream->print("Module " + String(module_id) + " (" + String(moduleName) + ") debug level set to NONE.");
              break;
            default:
              _debug_io_stream->println("Invalid Debug command. Enter ? for Help.");
              break;
            }
          }
        }
        else
        {
          _debug_io_stream->println("Error: Invalid module ID. Must be between 0 and ARDUINO_DEBUG_UTILS_MAX_MODULES.");
        }
      }

      // Check if the command is 2 or more characters long with the first character 'A'. Tries to use second character as the command character.
      else if (strlen(_commandBuffer) >= 2 && tolower(_commandBuffer[0]) == 'a')
      {
        char commandChar = tolower(_commandBuffer[1]);     // Second character is the command
        switch (commandChar)
        {
          case 'v': // VERBOSE
            setDebugLevelAll(DBG_VERBOSE);
            _debug_io_stream->print("All Modules: debug level set to VERBOSE.");
            break;
          case 'd': // DEBUG
            setDebugLevelAll(DBG_DEBUG);
            _debug_io_stream->print("All Modules: debug level set to DEBUG.");
            break;
          case 'i': // INFO
            setDebugLevelAll(DBG_INFO);
            _debug_io_stream->print("All Modules:  debug level set to INFO.");
            break;
          case 'w': // WARNING
            setDebugLevelAll(DBG_WARNING);
            _debug_io_stream->print("All Modules: debug level set to WARNING.");
            break;
          case 'e': // ERROR
            setDebugLevelAll(DBG_ERROR);
            _debug_io_stream->print("All Modules: debug level set to ERROR.");
            break;
          case 'n': // NONE
            setDebugLevelAll(DBG_NONE);
            _debug_io_stream->print("All Modules: debug level set to NONE.");
            break;
          default:
            _debug_io_stream->println("Invalid Debug command. Enter ? for Help.");
            break;
        }
      }
      // Check if the command is exactly 1 character long
      else if (strlen(_commandBuffer) == 1)
      {
        char commandChar = tolower(_commandBuffer[0]);     // First character is the command
        int  module_id = 0; // 

        const char *globalName = _moduleLabel[module_id]; // Get Global module name
        switch (commandChar)
        {
          case 'v': // VERBOSE
            setDebugLevel(DBG_VERBOSE);
            _debug_io_stream->print("Module " + String(module_id) + " (" + String(globalName) + ") debug level set to VERBOSE.");
            break;
          case 'd': // DEBUG
            setDebugLevel(DBG_DEBUG);
            _debug_io_stream->print("Module " + String(module_id) + " (" + String(globalName) + ") debug level set to DEBUG.");
            break;
          case 'i': // INFO
            setDebugLevel(DBG_INFO);
            _debug_io_stream->print("Module " + String(module_id) + " (" + String(globalName) + ") debug level set to INFO.");
            break;
          case 'w': // WARNING
            setDebugLevel(DBG_WARNING);
            _debug_io_stream->print("Module " + String(module_id) + " (" + String(globalName) + ") debug level set to WARNING.");
            break;
          case 'e': // ERROR
            setDebugLevel(DBG_ERROR);
            _debug_io_stream->print("Module " + String(module_id) + " (" + String(globalName) + ") debug level set to ERROR.");
            break;
          case 'n': // NONE
            setDebugLevel(DBG_NONE);
            _debug_io_stream->print("Module " + String(module_id) + " (" + String(globalName) + ") debug level set to NONE.");
            break;

          case '?':
            _debug_io_stream->println("Debug Options:");
            _debug_io_stream->println(" - Set Level: V (Verbose), D (Debug), I (Info), W (Warning), E (Error), N (None)");
            _debug_io_stream->println("   - Single char x or 0x to update global level (module 0)");
            _debug_io_stream->println("   - Add module number 1x to " + String(ARDUINO_DEBUG_UTILS_MAX_MODULES) + "x to update debug level for that module.");
            _debug_io_stream->println("   - Add A (Ax) to set that debug level x to all modules.");
            _debug_io_stream->println(" - Toggle Display Options in debug output:");
            _debug_io_stream->println("   - L (Label)  debug level label");
            _debug_io_stream->println("   - M (Module) module name");
            _debug_io_stream->println("   - T (Timestamps)");
            _debug_io_stream->println("   - C (New Line - think Carriage Return)");
            break;

          case 's':
            printDebugStatus();
            break;

          case 't':
            if (_timestamp_on)
            {
              timestampOff();
              _debug_io_stream->println("TIMESTAMPS set to OFF.");
            }
            else
            {
              timestampOn();
              _debug_io_stream->println("TIMESTAMPS set to ON.");
            }
            break;

          case 'c':
            if (_newline_on)
            {
              newlineOff();
              _debug_io_stream->println("NEWLINE set to OFF.");
            }
            else
            {
              newlineOn();
              _debug_io_stream->println("NEWLINE set to ON.");
            }
            break;

          case 'l':
            if (_print_debug_level_label)
            {
              debugLabelOff();
              _debug_io_stream->println("DEBUG LEVEL LABEL set to OFF.");
            }
            else
            {
              debugLabelOn();
              _debug_io_stream->println("DEBUG LEVEL LABEL set to ON.");
            }
            break;

          case 'm':
            if (_print_module_labels)
            {
              moduleLabelsOff();
              _debug_io_stream->println("MODULE LABELS set to OFF.");
            }
            else
            {
              moduleLabelsOn();
              _debug_io_stream->println("MODULE LABELS set to ON.");
            }
            break;

          default:
            _debug_io_stream->println("Invalid command. Use V (Verbose), D (Debug), I (Info), W (Warning), E (Error), N (None), ? (Help), S (Status), T (Timestamp), C (New Line), L (Level), M (Module).");

        }
      }

      // Clear the buffer for the next command
      bufferIndex = 0;
      _commandBuffer[0] = '\0';
    }
    else if (incomingChar != '\r')
    {
      // Add the character to the buffer if it's not a carriage return
      if (bufferIndex < COMMAND_BUFFER_SIZE - 1)
      {
        _commandBuffer[bufferIndex++] = incomingChar;
      }
    }
  }
}

/******************************************************************************
  PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

/**
 * @brief Format a message and print it to the selected debug output stream
 * 
 * This function is a variant of the standard printf function. It takes a format
 * string and a variable number of arguments, formats the arguments according to
 * the format string, and prints the result to the selected debug output stream.
 * 
 * The vPrint function is useful for printing debug messages because it allows
 * you to pass a variable number of arguments to the function. For example, you
 * can use it to print a string with a variable number of arguments like this:
 * 
 * @code
 * char const * fmt = "The value of x is %d and the value of y is %d";
 * int x = 1;
 * int y = 2;
 * vPrint(fmt, x, y);
 * @endcode
 * 
 * This would print the string "The value of x is 1 and the value of y is 2" to
 * the selected debug output stream.
 * 
 * @param fmt The format string to use when printing the message. This string
 * should contain format specifiers for the arguments that are passed to the
 * function.
 * @param args A variable number of arguments to format according to the format
 * string. The types of the arguments should match the format specifiers in the
 * format string.
 */
void Arduino_DebugUtils::vPrint(char const * fmt, va_list args) {

  va_list args_copy;
  va_copy(args_copy, args);

  // calculate required buffer length
  int msg_buf_size = vsnprintf(nullptr, 0, fmt, args) + 1; // add one for null terminator
#if __STDC_NO_VLA__ == 1
  // in the rare case where VLA is not allowed by compiler, fall back on heap-allocated memory
  char * msg_buf = new char[msg_buf_size];
#else
  char msg_buf[msg_buf_size];
#endif

  vsnprintf(msg_buf, msg_buf_size, fmt, args_copy);
  va_end(args_copy);

  if (_newline_on) {
    _debug_io_stream->println(msg_buf);
  } else {
    _debug_io_stream->print(msg_buf);
  }

#if __STDC_NO_VLA__ == 1
  // remember to clean up memory
  delete[] msg_buf;
#endif
}

/**
 * @brief Print a timestamp to the selected debug output stream.
 * 
 * This function prints a timestamp to the selected debug output stream. The
 * timestamp is formatted as a string in the format "HH:MM:SS.mmm" where:
 * 
 * - HH is the hour (00-23)
 * - MM is the minute (00-59)
 * - SS is the second (00-59)
 * - mmm is the millisecond (000-999)
 * 
 * The timestamp is only printed if the format_timestamp_on() function has been
 * called. This function is useful for adding timestamps to debug messages to
 * help with debugging and tracing the flow of the program.
 */
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

  _debug_io_stream->print(timestamp);
}

/**
 * @brief Print a debug level label to the selected debug output stream.
 * 
 * This function prints a debug level label to the selected debug output stream.
 * The label is formatted as a string in the format "[ DBG_ERROR ]" where:
 * 
 * - DBG_ERROR is the debug level
 * 
 * The label is only printed if the print_debug_level_label() function has been
 * called. This function is useful for adding debug level labels to debug
 * messages to help with debugging and tracing the flow of the program.
 */
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

  _debug_io_stream->print(DEBUG_MODE_STRING[debug_level]);
}


/**
 * @brief Checks if the debug level is valid and should be printed.
 * 
 * This function checks if the debug level is valid and should be printed.
 * The debug level is valid if it is between DBG_ERROR and DBG_VERBOSE
 * (inclusive). The function also checks if the debug level is less than
 * or equal to the global debug level. If both conditions are true, the
 * function returns true, otherwise it returns false.
 * 
 * @param debug_level The debug level to check.
 * @return true if the debug level is valid and should be printed, otherwise false.
 */
bool Arduino_DebugUtils::shouldPrint(int const debug_level) const
{
  int _debug_global_level = _moduleDebugLevel[0];
  
  return ((debug_level >= DBG_ERROR) && (debug_level <= DBG_VERBOSE) && (debug_level <= _debug_global_level));
}

/**
 * @brief Checks if the debug level is valid and should be printed for a specific module.
 * 
 * This function checks if the debug level is valid and should be printed for a
 * specific module. The debug level is valid if it is between DBG_ERROR and
 * DBG_VERBOSE (inclusive). The function also checks if the debug level is less
 * than or equal to the module's debug level. If both conditions are true, the
 * function returns true, otherwise it returns false.
 * 
 * @param module_id The module ID to check.
 * @param debug_level The debug level to check.
 * @return true if the debug level is valid and should be printed for the module, otherwise false.
 */
bool Arduino_DebugUtils::shouldPrint(int const module_id, int const debug_level) const
{
  int _debug_module_level = _moduleDebugLevel[module_id];

  return ((_debug_module_level >= DBG_ERROR) && (_debug_module_level <= DBG_VERBOSE) && (debug_level <= _debug_module_level));
}

/**
 * @brief Print the current debug status to the selected debug output stream.
 * 
 * This function prints the current debug status to the selected debug output
 * stream. The status includes the module number, module label, and debug level
 * for each module. The output is formatted in a tabular format with the following
 * columns:
 * 
 * - Module: The module number
 * - Label: The module label
 * - Level: The debug level
 * 
 * The function is useful for debugging and tracing the flow of the program.
 */
void Arduino_DebugUtils::printDebugStatus() {
    _debug_io_stream->println("Debug Status:");
    _debug_io_stream->println("Module\t\tLabel\t\tLevel");

    // Iterate through all the modules
    for (int i = 0; i < ARDUINO_DEBUG_UTILS_MAX_MODULES+1; ++i) {
        String currModuleLabel = getModuleLabel(i);
        if (currModuleLabel == "NA") continue; // Skip unused modules

        // Calculate extra tabs based on the length of the module label
        String output = String(i) + "\t\t" + currModuleLabel;

        int labelLength = currModuleLabel.length();
        if (labelLength < 8) {  // If the label is short, add extra tabs
            output += "\t\t";
        } else if (labelLength < 16) {  // If the label is medium length, add one extra tab
            output += "\t";
        }

        // Append the debug level to the output
        output += getDebugLevelLabel(getDebugLevel(i));

        // Print the output
        _debug_io_stream->println(output);
    }

    // Print the toggles for additional settings
    _debug_io_stream->println("Show Level: " + String(_print_debug_level_label ? "on" : "off") + " (toggle with L)");
    _debug_io_stream->println("Timestamps: " + String(_timestamp_on ? "on" : "off") + " (toggle with T)");
    _debug_io_stream->println("New Lines : " + String(_newline_on ? "on" : "off") + " (toggle with C)");
}
/******************************************************************************
  CLASS INSTANTIATION
 ******************************************************************************/

Arduino_DebugUtils Debug;

/**
 * @brief Set the global debug level.
 * 
 * This function sets the global debug level to the specified value. The
 * debug level is an integer value that determines the level of detail of
 * debug messages that will be printed. The valid values are:
 * 
 * - DBG_ERROR: Error messages
 * - DBG_WARNING: Warning messages
 * - DBG_INFO: Informational messages
 * - DBG_DEBUG: Debug messages
 * - DBG_VERBOSE: Verbose debug messages
 * 
 * @param debug_level The new global debug level to set.
 */
void setDebugMessageLevel(int const debug_level) {
  Debug.setDebugLevel(debug_level);
}

/**
 * @brief Get the current global debug level.
 * 
 * This function returns the current global debug level. The debug level is an
 * integer value that determines the level of detail of debug messages that will
 * be printed. The valid values are:
 * 
 * - DBG_ERROR: Error messages
 * - DBG_WARNING: Warning messages
 * - DBG_INFO: Informational messages
 * - DBG_DEBUG: Debug messages
 * - DBG_VERBOSE: Verbose debug messages
 * 
 * @return The current global debug level.
 */
int getDebugMessageLevel() {
  return Debug.getDebugLevel();
}
