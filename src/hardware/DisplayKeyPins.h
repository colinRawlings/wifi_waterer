#pragma once

#include "../Types.h"

struct SDisplayKeyPins
{
    SDisplayKeyPins(byte pin_tab_left_,
                    byte pin_tab_right_,
                    byte pin_func_left_,
                    byte pin_func_right_)
        : pin_tab_left(pin_tab_left_)
        , pin_tab_right(pin_tab_right_)
        , pin_func_left(pin_func_left_)
        , pin_func_right(pin_func_right_)
    {}

    byte pin_tab_left{0};
    byte pin_tab_right{0};
    byte pin_func_left{0};
    byte pin_func_right{0};
};
