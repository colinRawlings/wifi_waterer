#pragma once

#include <string>

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

void ErrorLedBegin();
void ErrorLedState(bool is_on);

void LogBegin(int rate);
void Log(std::string msg);
void LogLn(std::string msg);
