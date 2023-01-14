#pragma once

#ifdef ARDUINO_TESTING
using byte = unsigned char;
#else
    #include "Arduino.h"
    #include <LiquidCrystal.h>
    #include <RTCZero.h>
#endif