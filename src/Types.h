#pragma once

#ifdef ARDUINO_TESTING
using byte = unsigned char;
#else
    #include "Arduino.h"       // NOLINT
    #include <LiquidCrystal.h> // NOLINT
    #include <RTCZero.h>       // NOLINT
#endif
