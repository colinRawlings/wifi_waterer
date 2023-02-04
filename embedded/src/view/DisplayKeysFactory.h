#pragma once

#include "../Types.h"

#include "../hardware/DisplayKeyPins.h"

#include "KeyFactory.h"

#include "Key.h"
#include "DisplayKeys.h"

CDisplayKeys_ptr CreateDisplayKeys(SDisplayKeyPins pins)
{
    auto left_tab = CreateKey("tab_left", pins.pin_tab_left, true);
    auto right_tab = CreateKey("tab_right", pins.pin_tab_right, true);

    auto left_func = CreateKey("func_left", pins.pin_func_left, true);
    auto right_func = CreateKey("func_right", pins.pin_func_right, true);

    //

    return CDisplayKeys::Create(left_tab, right_tab, left_func, right_func);
}
