#include "Types.h"

#ifdef ARDUINO_TESTING
    #include <iostream>

long millis() { return 0; }

void LogBegin(__attribute__((unused)) int rate) {}

void Log(std::string msg)
{
    std::cout << msg;
}
void LogLn(std::string msg)
{
    std::cout << msg << std::endl;
}

void ErrorLedBegin()
{
}

void ErrorLedState(bool is_on)
{
    std::cout << "<ERROR_LED>: " << is_on << std::endl;
}

#else

void ErrorLedBegin()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void ErrorLedState(bool is_on)
{
    digitalWrite(LED_BUILTIN, is_on);
}

void LogBegin(int rate) { Serial.begin(rate); }

void Log(std::string msg)
{
    Serial.print(msg.c_str());
}
void LogLn(std::string msg)
{
    Serial.println(msg.c_str());
}

#endif
