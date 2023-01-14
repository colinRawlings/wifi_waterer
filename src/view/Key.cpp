#include "Arduino.h"

#include "../hardware/PushSwitch.h"

#include "Key.h"

CKey_ptr CKey::Create(std::string name, byte pin, bool pull_up)
{
    auto key = CKey::Create(name);

    auto sw = CPushSwitch::Create(pin, pull_up);
    key->SetPushSwitch(std::move(sw));

    return key;
}

CKey_ptr CKey::Create(std::string name)
{
    auto destroy = [](CKey * display) { delete display; };
    return CKey_ptr(new CKey(name), destroy);
}

CKey::CKey(std::string name)
    : _name(name)
{}

//

void CKey::SetPushSwitch(IPushSwitch_uptr push_switch)
{
    _push_switch = std::move(push_switch);

    if (_push_switch)
        _last_switch_state = _push_switch->GetState();
}

bool CKey::KeyPressed()
{
    if (!_push_switch)
        return false;

    auto new_switch_state = _push_switch->GetState();

    bool pressed = (_last_switch_state == PushSwitchState::UNPRESSED) &&
                   (new_switch_state == PushSwitchState::PRESSED);

    _last_switch_state = new_switch_state;

    return pressed;
}
