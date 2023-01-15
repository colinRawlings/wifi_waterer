#pragma once

#include "../Types.h"

#include "interfaces/ILiquidCrystal.h"

#include <array>
#include <memory>

class CDisplay;
using CDisplay_ptr = std::shared_ptr<CDisplay>;

class CDisplay
{
  public:
    static CDisplay_ptr Create();

    void SetLcd(ILiquidCrystal_uptr lcd);

    void SetRow0(std::string msg);
    void SetRow1(std::string msg);

    void SetBacklight(bool is_on);

    void Print();

  private:
    CDisplay() = default;

    void SetRow(int row_index, std::string msg);

    ILiquidCrystal_uptr _lcd;
    std::array<std::string, 2> _rows;
};
