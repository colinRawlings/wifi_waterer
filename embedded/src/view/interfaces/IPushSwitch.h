#pragma once

#include "../../Types.h"

#include <memory>

enum class PushSwitchState : uint8_t { UNPRESSED = 0,
                                       PRESSED };

class IPushSwitch;
using IPushSwitch_uptr = std::unique_ptr<IPushSwitch>;

class IPushSwitch
{
  public:
    virtual PushSwitchState GetState() = 0;

    virtual ~IPushSwitch() = default;
};
