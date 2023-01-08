#pragma once

#include <memory>

class IPushSwitch;
using IPushSwitch_uptr = std::unique_ptr<IPushSwitch>;

class IPushSwitch {
public:
  virtual bool GetState() = 0;
};