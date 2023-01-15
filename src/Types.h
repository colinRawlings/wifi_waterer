#pragma once

#ifdef ARDUINO_TESTING
using byte = unsigned char;
long millis();
#else
    #include "Arduino.h"       // NOLINT
    #include <LiquidCrystal.h> // NOLINT
    #include <RTCZero.h>       // NOLINT
    #include <WiFiNINA.h>      // NOLINT
    #include <FlashStorage.h>  // NOLINT
#endif
