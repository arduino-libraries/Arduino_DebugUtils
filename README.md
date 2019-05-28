Arduino_DebugUtils
==================

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
Debug.print(DBG_VERBOSE, "i = %d, pi = %f, i, pi);
```

If desired timestamps can be prefixed to the debug message. Timestamp output can be enabled and disabled via `timestampOn` and `timestampOff`.

# How-To-Use Advanced
Normally all debug output is redirected to the primary serial output of each board (`Serial`). In case you want to redirect the output to another output stream you can make use of `setDebugOutputStream(&Serial2)`.
