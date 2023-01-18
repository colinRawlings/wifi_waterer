#include "Types.h"

#ifdef ARDUINO_TESTING
    #include <iostream>

long millis() { return 0; }

void LogBegin(int rate) {}

void Log(std::string msg)
{
    std::cout << msg;
}
void logln(std::string msg)
{
    std::cout << msg << std::endl;
}

#else

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
