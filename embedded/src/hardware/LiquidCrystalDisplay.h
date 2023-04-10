#pragma once

#include "../Types.h"

#include "LCDPins.h"

#include "../view/interfaces/ILiquidCrystal.h"

#include <array>
#include <memory>
#include <string>

class LiquidCrystal;
class CDigitalOutput;
using CDigitalOutput_ptr = std::shared_ptr<CDigitalOutput>;

class CLiquidCrystalDisplay;
using CLiquidCrystalDisplay_uptr = std::unique_ptr<CLiquidCrystalDisplay>;

class CLiquidCrystalDisplay : public ILiquidCrystal
{
  public:
    static CLiquidCrystalDisplay_uptr Create(SLCDPins pins);

    void Print(std::string msg) override;
    void Clear() override;
    void SetCursor(int column, int row) override;

    bool Backlight() override;
    void SetBacklight(bool is_on) override;

  protected:
    CLiquidCrystalDisplay(SLCDPins pins);

    std::unique_ptr<LiquidCrystal> _lcd;
    CDigitalOutput_ptr _backlight_power;
};
