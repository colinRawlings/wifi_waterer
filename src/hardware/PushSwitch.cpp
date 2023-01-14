#include "PushSwitch.h"

#include "Arduino.h"

CPushSwitch_uptr CPushSwitch::Create(uint8_t pin, bool pull_up)
{
    return std::unique_ptr<CPushSwitch>(new CPushSwitch(pin, pull_up));
}

CPushSwitch::CPushSwitch(uint8_t pin, bool pull_up)
    : _pin(pin)
    , _pull_up(pull_up)
{
    if (_pull_up)
        pinMode(_pin, INPUT_PULLUP);
    else
        pinMode(_pin, INPUT);

    _T_last_read_ms = millis();
    _last_state = GetCurrentState();
}

PushSwitchState CPushSwitch::GetCurrentState()
{
    uint8_t pushSwitchVal = digitalRead(_pin);

    if ((_pull_up && pushSwitchVal == 0) || (!_pull_up && pushSwitchVal == 1))
        return PushSwitchState::PRESSED;
    else
        return PushSwitchState::UNPRESSED;
}

PushSwitchState CPushSwitch::GetState()
{
    if ((millis() - _T_last_read_ms) < 10)
        return _last_state;

    _T_last_read_ms = millis();
    _last_state = GetCurrentState();

    return _last_state;
}