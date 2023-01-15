#pragma once

#include "../Types.h"

struct SSmartPumpPins
{
    SSmartPumpPins(byte pump_pin_,
                   byte sensor_pin_)
        : pump_pin(pump_pin_)
        , sensor_pin(sensor_pin_)
    {}

    byte pump_pin{0};
    byte sensor_pin{0};
};
