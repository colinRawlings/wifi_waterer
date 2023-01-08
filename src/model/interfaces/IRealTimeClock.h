#pragma once

#include "../../Types.h"

#include <memory>

class IRealTimeClock;
using IRealTimeClock_uptr = std::unique_ptr<IRealTimeClock>;

class IRealTimeClock {
public:
  virtual byte getHours() = 0;
  virtual byte getMinutes() = 0;
  virtual byte getSeconds() = 0;

  virtual void setHours(byte) = 0;
  virtual void setMinutes(byte) = 0;
  virtual void setSeconds(byte) = 0;
};