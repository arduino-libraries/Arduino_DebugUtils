Arduino_DebugUtils
==================

[![Check Arduino status](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/check-arduino.yml)
[![Compile Examples status](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/compile-examples.yml)
[![Spell Check status](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/spell-check.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_DebugUtils/actions/workflows/spell-check.yml)

This class provides functionality useful for debugging sketches via `printf`-style statements.

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

# How-To-Use Advanced
Normally all debug output is redirected to the primary serial output of each board (`Serial`). In case you want to redirect the output to another output stream you can make use of `setDebugOutputStream(&Serial2)`.

# Documentation
### Debug :
Arduino_DebugUtils Object that will be used for calling member functions.

### Debug.setDebugLevel(int const debug_level) :
Parameter debug_level in order of lowest to highest priority are : `DBG_NONE`, `DBG_ERROR`, `DBG_WARNING`, `DBG_INFO` (default), `DBG_DEBUG`, and `DBG_VERBOSE`. 

Return type: void.

Example:
```C++
Debug.setDebugLevel(DBG_VERBOSE);
```
### Debug.setDebugOutputStream(Stream * stream) :
By default, Output Stream is Serial. In advanced cases other objects could be other serial ports (if available), or can be a Software Serial object.

Return type: void.

Example:
```C++
SoftwareSerial mySerial(10, 11); // RX, TX
Debug.setDebugOutputStream(&mySerial);
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
Debug.timestampOff();
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
