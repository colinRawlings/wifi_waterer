#pragma once

#include "../Types.h"

#include <memory>

class SLCDPins
{
  public:
    SLCDPins(std::array<byte, 4> d4_d7, byte Rs_, byte E_)
        : D4_D7(d4_d7)
        , Rs(Rs_)
        , E(E_)
    {}

    std::array<byte, 4> D4_D7{0, 0, 0, 0};

    byte Rs{0};
    byte E{0};
};
