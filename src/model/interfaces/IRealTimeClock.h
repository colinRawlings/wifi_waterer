#pragma once

#include "../../Types.h"

#include <memory>

class IRealTimeClock;
using IRealTimeClock_uptr = std::unique_ptr<IRealTimeClock>;

class IRealTimeClock
{
  public:
    virtual byte GetHours() = 0;
    virtual byte GetMinutes() = 0;
    virtual byte GetSeconds() = 0;

    virtual void SetHours(byte) = 0;
    virtual void SetMinutes(byte) = 0;
    virtual void SetSeconds(byte) = 0;
};
