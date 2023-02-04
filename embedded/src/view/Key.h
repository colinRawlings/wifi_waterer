#pragma once

#include "../Types.h"

#include "interfaces/IPushSwitch.h"

#include <string>

class CKey;
using CKey_ptr = std::shared_ptr<CKey>;

class CKey
{
  public:
    static CKey_ptr Create(std::string name);

    void SetPushSwitch(IPushSwitch_uptr push_switch);

    bool KeyPressed();

  private:
    CKey(std::string name);

    std::string _name;
    PushSwitchState _last_switch_state{PushSwitchState::UNPRESSED};
    IPushSwitch_uptr _push_switch;
};
