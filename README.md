ArduinoDebugUtils
=================

This class provides functionality useful for debugging sketches via `printf`-style statements.

# How-To-Use Basic
ArduinoDebugUtils has 6 different debug levels (described descending from highest to lowest priority):
* `DEBUG_LVL_NONE` - no debug output is shown
* `DEBUG_LVL_ERROR` - critical errors
* `DEBUG_LVL_WARNING` - non-critical errors
* `DEBUG_LVL_INFO` - information
* `DEBUG_LVL_DEBUG` - more information
* `DEBUG_LVL_VERBOSE` - most information

The desired debug level can be set via `setDebugLevel(DEBUG_LVL_WARNING)`.

Debug messages are written via `debugPrint` which supports `printf`-style formatted output.

Example:
```C++
int i = 1;
float pi = 3.1459;
ArduinoDebugUtils.debugPrint(DEBUG_LVL_VERBOSE, "i = %d, pi = %f, i, pi);
```

If desired timestamps can be prefixed to the debug message. Timestamp output can be enabled and disabled via `timestampOn` and `timestampOff`.

# How-To-Use Advanced
Normally all debug output is redirected to the primary serial output of each board (`Serial`). In case you want to redirect the output to another output stream you can make use of `setDebugOutputStream(&Serial2)`.
