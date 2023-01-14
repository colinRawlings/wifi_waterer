#pragma once

#include "../Types.h"

#include "KeyFactory.h"

#include "Key.h"
#include "DisplayKeys.h"

CDisplayKeys_ptr CreateDisplayKeys(const byte pin_tab_left,
                                   const byte pin_tab_right,
                                   const byte pin_func_left,
                                   const byte pin_func_right)
{
    auto left_tab = CreateKey("tab_left", pin_tab_left, true);
    auto right_tab = CreateKey("tab_right", pin_tab_right, true);

    auto left_func = CreateKey("func_left", pin_func_left, true);
    auto right_func = CreateKey("func_right", pin_func_right, true);

    //

    return CDisplayKeys::Create(left_tab, right_tab, left_func, right_func);
}
