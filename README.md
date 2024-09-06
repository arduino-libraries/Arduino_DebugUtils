Arduino_DebugUtils
==================

[![Check Arduino status](https://github.com/robroypt/Arduino_DebugUtils/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/robroypt/Arduino_DebugUtils/actions/workflows/check-arduino.yml)
[![Compile Examples status](https://github.com/robroypt/Arduino_DebugUtils/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/robroypt/Arduino_DebugUtils/actions/workflows/compile-examples.yml)
[![Spell Check status](https://github.com/robroypt/Arduino_DebugUtils/actions/workflows/spell-check.yml/badge.svg)](https://github.com/robroypt/Arduino_DebugUtils/actions/workflows/spell-check.yml)

This class provides functionality useful for debugging sketches via `printf`-style statements.

# How-To-Use Basic
Arduino_DebugUtils has 6 different debug levels (described descending from highest to lowest priority):
* `DBG_NONE` - no debug output is shown
* `DBG_ERROR` - critical errors
* `DBG_WARNING` - non-critical errors
* `DBG_INFO` - information
* `DBG_DEBUG` - more information
* `DBG_VERBOSE` - most information

The desired debug level can be set in code via `setDebugLevel(DBG_WARNING)` or while the code is running using the Serial Monitor.

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
## Code setup

### Include the Library

Include the Arduino_DebugUtils library by adding
```C++
#include "Arduino_DebugUtils.h"
```
at the top of your code.

### Setup the Serial Port

By default, the Output Stream and input port for printing debug messages and receiving config commands is `Serial`. 

For minimal setup, just create the Serial object with a specified baud rate.
```C++
  Serial.begin(9600); // Set baud rate here. Make sure you match the same baud rate in your terminal or monitor
```
In advanced cases you can use other serial ports (if available), or a Software Serial object. You can also (optionally) set a different Debug Level, turn on timestamps and debug labels.

```C++
  mySerial.begin(9600);  // Set baud rate here. Make sure you match the same baud rate in your terminal or monitor
  Debug.setDebugOutputStream(&mySerial);
  Debug.setDebugLevel(DBG_VERBOSE);
  Debug.timestampOn();
```

### Debug: object
Arduino_DebugUtils Object that will be used for calling member functions is automatically instantiated when you include the library.

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

### Debug.debugLabelOn() :
Calling this function switches on the Debug Label in the `Debug.print()` function call;
By default, printing the debug label is off, unless turned on using this function call.

Return type: void.

Example:
```C++
Debug.debugLabelOn();
```

### Debug.debugLabelOff() :
Calling this function switches off the Debug Label in the `Debug.print()` function call;
By default, printing the debug label is off, unless turned on using this function call.

Return type: void.

Example:
```C++
Debug.debugLabelOff();
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

### Debug.print(int const debug_level, const char * fmt, ...);
This function prints the message if parameter `debug_level` in the `Debug.print(debug_level, ...)` function call belongs to the range: DBG_ERROR <= debug_level <= (<DBG_LEVEL> that has been set using `setDebugLevel()` function).

Return type: void.

Example:
```C++
Debug.setDebugLevel(DBG_VERBOSE);
int i = 0;
DEBUG_VERBOSE("DBG_VERBOSE i = %d", i);
```
## Set Debug Settings at Runtime

While your code is running you can 
- set the current debug level
- toggle the display of the prefixed Timestamp at the beginning of the `Debug.print()` function call
- toggle the display of the debug label at the beginning of the `Debug.print()` function call
- toggle the sending of a newline at the end of the `Debug.print()` function call

### Setup

Modify your `loop()` function to call `Debug.processDebugConfigCommand()` each time. 

Example:
```C++
int i = 0;

void loop() {
  DEBUG_VERBOSE("i = %d", i);
  i++;
  Debug.processDebugConfigCommand();
  delay(1000); // See note on timing below
}
```

### Timing
- If you have a delay in your `loop()` (as is the case with the example code) this will also delay action on any entered commands, so for faster response call `processDebugConfigCommand()` more often.

### Trouble-shooting
Check your monitor or terminal is configured:
- to send a LF or CRLF Line Ending at the end of a message sent via the serial port. 
- to use the same baud rate specified when setting up the Serial object (9600 in the examples)