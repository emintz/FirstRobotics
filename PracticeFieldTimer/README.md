# FIRST Robotics Competition Practice Field Countdown Timer


## CAN Bus Message

CAN Bus traffic consists of the following messages

* Command and colors
* Times

Note that all values are `uint8_t`, unsigned bytes

### Command And Colors

The Command and Colors message provides the pattern to be displayed
and its foreground and background colors.

|  Byte | Contents                                                                    |
|------:|-----------------------------------------------------------------------------|
|     0 | Pattern to display                                                          |
|     1 | Foreground red intensity                                                    |
|     2 | Foreground green intensity                                                  |
|     3 | Foreground blue intensity                                                   |
|     4 | Background red intensity                                                    |
|     5 | Background green intensity                                                  |
|     6 | Background blue intensity                                                   |
|     7 | Reserved, must be zero (0)                                                  |

Note that this is the first of two messages that instruct the recipient to
display a pattern. Pattern display implementations may ignore any field.

### Times

The Times command contains the initial countdown duration and remaining time 
in seconds. Time values are bounded by 99:59, 99 minutes and 59 seconds, or
5999 seconds so they fit nicely in an `int16_t`. Times are encoded
little-endian.

|   Byte | Contents                                                                    |
|-------:|-----------------------------------------------------------------------------|
|      0 | Duration, least significant byte                                            |
|      1 | Duration, most significant byte                                             |
|      2 | Remaining, least significant byte                                           |
|      3 | Remaining, most significant byte                                            |
|      4 | Reserved, must be zero (0)                                                  |
|      5 | Reserved, must be zero (0)                                                  |
|      6 | Reserved, must be zero (0)                                                  |

Note that this is the second of two messages that instruct the recipient to
display a pattern. Pattern display implementations may ignore any field.

## Build Notes

### LCD Module

https://www.amazon.com/dp/B0D145PQDJ

### Libraries

| Library                                                                           | Description                                                          |
|:----------------------------------------------------------------------------------|:---------------------------------------------------------------------|
| [RTCLib-NeiroNX](https://github.com/lab-mathias-claussen/RTCLib-NeiroNx)          | Real-time clock library                                              |
| [LiquidCrystal_I2C](https://github.com/markub3327/LiquidCrystal_I2C)              | Liquid Crystal management over I2C                                   |
| [CANbus](https://github.com/emintz/ArduinoLib/tree/main/CANBus)                   | CAN (Controller Area Network) bus support                            |
| [PropertyWebPage](https://github.com/emintz/ArduinoLib/tree/main/PropertyWebPage) | Configuration web page support                                       |
| [RTOSAid](https://github.com/emintz/ArduinoLib/tree/main/RTOSAid)                 | C++ convenience wrappers over low-level FreeRTOS APIs                |
| [FastLED](https://fastled.io/)                                                    | Individually addressable LED management API for WS2812b among others |
| [LiquidCrystal_I2C](https://github.com/markub3327/LiquidCrystal_I2C)              | Liquid Cystal management over I2C                                    |

