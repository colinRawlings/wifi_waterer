#pragma once

#include "../Types.h"

#include "LCDPins.h"

#include "../view/interfaces/ILiquidCrystal.h"

#include <array>
#include <memory>
#include <string>

class LiquidCrystal;
class CDigitalOutput;

class CLiquidCrystalDisplay;
using CLiquidCrystalDisplay_uptr = std::unique_ptr<CLiquidCrystalDisplay>;

class CLiquidCrystalDisplay : public ILiquidCrystal
{
  public:
    static CLiquidCrystalDisplay_uptr Create(SLCDPins pins);

    void Print(std::string msg) override;
    void Clear() override;
    void SetCursor(int column, int row) override;
    void SetBacklight(bool is_on) override;

  protected:
    CLiquidCrystalDisplay(SLCDPins pins);

    std::unique_ptr<LiquidCrystal> _lcd;
    std::unique_ptr<CDigitalOutput> _backlight_power;
};
