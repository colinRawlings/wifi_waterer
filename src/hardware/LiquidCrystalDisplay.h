#pragma once

#include "../Types.h"

#include "../view/interfaces/ILiquidCrystal.h"

#include <array>
#include <memory>
#include <string>

class LiquidCrystal; // the arduino class being wrapped

class SLCDPins
{
  public:
    SLCDPins(std::array<byte, 4> d4_d7, byte Rs_, byte E_)
        : D4_D7(d4_d7)
        , E(E_)
        , Rs(Rs_)
    {}

    std::array<byte, 4> D4_D7{0, 0, 0, 0};

    byte Rs{0};
    byte E{0};
};

class CLiquidCrystalDisplay;
using CLiquidCrystalDisplay_uptr = std::unique_ptr<CLiquidCrystalDisplay>;

class CLiquidCrystalDisplay : public ILiquidCrystal
{
  public:
    static CLiquidCrystalDisplay_uptr Create(SLCDPins pins);

    void Print(std::string msg) override;
    void Clear() override;
    void SetCursor(int column, int row) override;

  protected:
    CLiquidCrystalDisplay(SLCDPins pins);

    std::unique_ptr<LiquidCrystal> _lcd;
};