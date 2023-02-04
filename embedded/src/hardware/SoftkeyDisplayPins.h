#pragma once

#include "../Types.h"

#include "DisplayKeyPins.h"
#include "LCDPins.h"

struct SSoftkeyDisplayPins
{
    SSoftkeyDisplayPins(SDisplayKeyPins display_key_pins_,
                        SLCDPins lcd_pins_)
        : display_key_pins(display_key_pins_)
        , lcd_pins(lcd_pins_)
    {}

    SDisplayKeyPins display_key_pins;
    SLCDPins lcd_pins;
};
