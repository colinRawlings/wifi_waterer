#pragma once
#include "../Types.h"

#include "Key.h"

#include "../hardware/PushSwitch.h"

CKey_ptr CreateKey(std::string name, byte pin, bool pull_up)
{
    auto key = CKey::Create(name);

    auto sw = CPushSwitch::Create(pin, pull_up);
    key->SetPushSwitch(std::move(sw));

    return key;
}
