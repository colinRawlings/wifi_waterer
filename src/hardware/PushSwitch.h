#pragma once

#include "../Types.h"

#include <memory>

#include "../view/interfaces/IPushSwitch.h"

class CPushSwitch;
using CPushSwitch_uptr = std::unique_ptr<CPushSwitch>;

class CPushSwitch : public IPushSwitch
{

  public:
    static CPushSwitch_uptr Create(uint8_t pin_, bool pullUp_);

    PushSwitchState GetState() override;

    CPushSwitch(uint8_t pin_, bool pullUp_);
    ~CPushSwitch() = default;

  private:
    uint8_t _pin;
    bool _pull_up;

    unsigned long _T_last_read_ms;
    PushSwitchState _last_state;

  private:
    PushSwitchState GetCurrentState();
};