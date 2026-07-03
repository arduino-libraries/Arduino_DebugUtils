Arduino_DebugUtils
==================

[![Check Arduino status](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/check-arduino.yml)
[![Compile Examples status](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/compile-examples.yml)
[![Spell Check status](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/spell-check.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/spell-check.yml)

This class provides functionality useful for debugging sketches via `printf`-style statements with enhanced features including module-based debug levels and runtime configuration.

# How-To-Use Basic
Arduino_DebugUtils has 6 different debug levels (described descending from highest to lowest priority):
* `DBG_NONE` - no debug output is shown
* `DBG_ERROR` - critical errors
* `DBG_WARNING` - non-critical errors
* `DBG_INFO` - information
* `DBG_DEBUG` - more information
* `DBG_VERBOSE` - most information

The desired debug level can be set via `setDebugLevel(DBG_WARNING)`.

Debug messages are written via `print` which supports `printf`-style formatted output.

Example:
```C++
int i = 1;
float pi = 3.1459;
DEBUG_VERBOSE("i = %d, pi = %f", i, pi);
```
**Note**: The output of floating point numbers (`%f`) does NOT work on [ArduinoCore-avr](https://github.com/arduino/ArduinoCore-avr).

If desired, timestamps can be prefixed to the debug message. Timestamp output can be enabled and disabled via `timestampOn` and `timestampOff`.

# New Features

## Module-Based Debug Levels
This enhanced version supports up to 20 separate modules, each with its own debug level. This allows for more granular control over debugging output in complex applications.

```C++
// Set debug level for a specific module (1-20)
Debug.setDebugLevel(1, DBG_VERBOSE);  // Module 1 gets verbose output
Debug.setDebugLevel(2, DBG_WARNING);  // Module 2 only shows warnings and errors

// Set a label for each module
Debug.setModuleLabel(1, "WIFI");
Debug.setModuleLabel(2, "SENSOR");

// Print debug message for a specific module
Debug.print(1, DBG_INFO, "Connected to SSID: %s", ssid);  // Module 1 (WIFI) info
Debug.print(2, DBG_ERROR, "Sensor reading failed: %d", errorCode);  // Module 2 (SENSOR) error
```

## Runtime Configuration via Serial
You can now change debug settings at runtime through the same serial port used for debug output:

```
// Send these commands through serial monitor to change settings
VERBOSE    // or V - Sets global debug level to VERBOSE
DEBUG      // or D - Sets global debug level to DEBUG
INFO       // or I - Sets global debug level to INFO
WARNING    // or W - Sets global debug level to WARNING
ERROR      // or E - Sets global debug level to ERROR
NONE       // or N - Turns off global debug output

// Change debug level for a specific module
3V         // Sets module 3's debug level to VERBOSE
5D         // Sets module 5's debug level to DEBUG
7I         // Sets module 7's debug level to INFO
10W        // Sets module 10's debug level to WARNING
15E        // Sets module 15's debug level to ERROR
20N        // Sets module 20's debug level to NONE

// Toggle configuration settings
LABEL      // or L - Toggle debug level labels
TIMESTAMP  // or T - Toggle timestamps
NEWLINE    // or C - Toggle newlines

// Display current debug status
STATUS     // or S - Display current debug configuration
```

## Enhanced Timestamp Formatting
Timestamps can now be displayed in a more readable format:

```C++
// Format timestamps as [HH:MM:SS.mmm] instead of milliseconds
Debug.formatTimestampOn();
```

# How-To-Use Advanced
Normally all debug output is redirected to the primary serial output of each board (`Serial`). In case you want to redirect the output to another output stream you can make use of `setDebugOutputStream(&Serial2)`.

# Documentation
### Debug :
Arduino_DebugUtils Object that will be used for calling member functions.

## Basic Functions

### Debug.setDebugLevel(int const debug_level) :
Sets the global debug level (module 0).

Parameter debug_level in order of lowest to highest priority are : `DBG_NONE`, `DBG_ERROR`, `DBG_WARNING`, `DBG_INFO` (default), `DBG_DEBUG`, and `DBG_VERBOSE`. 

Return type: void.

Example:
```C++
Debug.setDebugLevel(DBG_VERBOSE);
```

### Debug.getDebugLevel() :
Returns the current global debug level.

Return type: int.

Example:
```C++
int currentLevel = Debug.getDebugLevel();
```

### Debug.setDebugOutputStream(Stream * stream) :
By default, Output Stream is Serial. In advanced cases other objects could be other serial ports (if available), or can be a Software Serial object.

Return type: void.

Example:
```C++
SoftwareSerial mySerial(10, 11); // RX, TX
Debug.setDebugOutputStream(&mySerial);
```

### Debug.setDebugIOStream(Stream * stream) :
Sets both the input and output stream for debug messages. This allows receiving configuration commands on the same stream used for debug output.

Return type: void.

Example:
```C++
Debug.setDebugIOStream(&Serial);
```

### Debug.timestampOn() :
Calling this function switches on the timestamp in the `Debug.print()` function call;
By default, printing timestamp is off, unless turned on using this function call.

Return type: void.

Example:
```C++
Debug.timestampOn();
DBG_VERBOSE("i = %d", i); //Output looks like : [ 21007 ] i = 21
```

### Debug.timestampOff() :
Calling this function switches off the timestamp in the `Debug.print()` function call;

Return type: void.

Example:
```C++
Debug.timestampOff();
DEBUG_VERBOSE("i = %d", i); //Output looks like : i = 21
```

### Debug.formatTimestampOn() :
Calling this function enables formatted timestamps in the format [HH:MM:SS.mmm] instead of milliseconds.

Return type: void.

Example:
```C++
Debug.formatTimestampOn();
DEBUG_VERBOSE("i = %d", i); //Output looks like : [ 00:01:35.421 ] i = 21
```

### Debug.formatTimestampOff() :
Calling this function disables formatted timestamps, reverting to milliseconds display.

Return type: void.

Example:
```C++
Debug.formatTimestampOff();
```

### Debug.newlineOn() :
Calling this function ensures that a newline will be sent at the end of the `Debug.print()` function call;
By default, a newline is sent
Return type: void.

Example:
```C++
Debug.newlineOn();
```

### Debug.newlineOff() :
Calling this function ensure that a newline will NOT be sent at the end of the `Debug.print()` function call;
By default a newline is sent. Call this to shut that functionality off.
Return type: void.

Example:
```C++
Debug.newlineOff();
```

### Debug.debugLabelOn() :
Calling this function enables display of debug level labels (ERROR, WARNING, etc.) in the output.

Return type: void.

Example:
```C++
Debug.debugLabelOn();
```

### Debug.debugLabelOff() :
Calling this function disables display of debug level labels in the output.

Return type: void.

Example:
```C++
Debug.debugLabelOff();
```

### Debug.print(int const debug_level, const char * fmt, ...);
This function prints the message if parameter `debug_level` in the `Debug.print(debug_level, ...)` function call belongs to the range: DBG_ERROR <= debug_level <= (<DBG_LEVEL> that has been set using `setDebugLevel()` function).

Return type: void.

Example:
```C++
Debug.setDebugLevel(DBG_VERBOSE);
int i = 0;
DEBUG_VERBOSE("DBG_VERBOSE i = %d", i);
```

## Module-Based Debug Functions

### Debug.setDebugLevel(int const module_id, int const debug_level) :
Sets the debug level for a specific module.

Parameters:
- module_id: The ID of the module (1-20)
- debug_level: The debug level to set

Return type: void.

Example:
```C++
Debug.setDebugLevel(1, DBG_VERBOSE);  // Set module 1 to verbose level
```

### Debug.setDebugLevelAll(int const debug_level) :
Sets the same debug level for all modules, including the global module.

Parameter:
- debug_level: The debug level to set for all modules

Return type: void.

Example:
```C++
Debug.setDebugLevelAll(DBG_WARNING);  // Set all modules to warning level
```

### Debug.getDebugLevel(int const module_id) :
Returns the current debug level for a specific module.

Parameter:
- module_id: The ID of the module to query

Return type: int.

Example:
```C++
int moduleLevel = Debug.getDebugLevel(1);
```

### Debug.setModuleLabel(int const module_id, const char* label) :
Sets a descriptive label for a specific module.

Parameters:
- module_id: The ID of the module (1-20)
- label: A string label for the module (max 10 characters by default)

Return type: void.

Example:
```C++
Debug.setModuleLabel(1, "WIFI");
Debug.setModuleLabel(2, "SENSOR");
```

### Debug.getModuleLabel(int const module_id) :
Returns the label for a specific module.

Parameter:
- module_id: The ID of the module to query

Return type: String.

Example:
```C++
String label = Debug.getModuleLabel(1);  // Returns "WIFI"
```

### Debug.moduleLabelsOn() :
Enables display of module labels in debug output.

Return type: void.

Example:
```C++
Debug.moduleLabelsOn();
```

### Debug.moduleLabelsOff() :
Disables display of module labels in debug output.

Return type: void.

Example:
```C++
Debug.moduleLabelsOff();
```

### Debug.print(int const module_id, int const debug_level, const char * fmt, ...);
Prints a debug message for a specific module if the module's debug level is high enough.

Parameters:
- module_id: The ID of the module (1-20)
- debug_level: The debug level of this message
- fmt: Printf-style format string
- ...: Variable arguments for the format string

Return type: void.

Example:
```C++
Debug.print(1, DBG_INFO, "Connected to SSID: %s", ssid);
```

## Runtime Configuration

### Debug.processDebugConfigCommand() :
Checks for and processes any pending debug configuration commands from the input stream.

This function should be called regularly in your loop() function to enable runtime configuration. It allows you to change debug settings at runtime by sending commands through the serial monitor, including:

- Change global debug level with commands like `VERBOSE`, `DEBUG`, etc.
- Change module-specific debug levels with commands like `3V` (set module 3 to VERBOSE)
- Toggle configuration settings with commands like `LABEL`, `TIMESTAMP`, etc.
- Display current debug status with the `STATUS` command

Return type: void.

Example:
```C++
void loop() {
  // Process any debug configuration commands
  Debug.processDebugConfigCommand();
  
  // Rest of your code
  // ...
}
```

### Debug.printDebugStatus() :
Displays the current debug configuration status, including module labels, debug levels, and toggle settings.

Return type: void.

Example:
```C++
Debug.printDebugStatus();
// Output looks like:
// Debug Status:
// Module		Label		Level
// 0		GLOBAL		VERBOSE
// 1		WIFI		INFO
// 2		SENSOR		WARNING
// Show Level: on (toggle with L)
// Timestamps: on (toggle with T)
// New Lines : on (toggle with C)
```
