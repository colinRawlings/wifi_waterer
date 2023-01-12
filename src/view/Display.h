#pragma once

#include <array>
#include <memory>

#include "../Types.h"
#include "interfaces/ILiquidCrystal.h"

class CDisplay;
using CDisplay_ptr = std::shared_ptr<CDisplay>;

class CDisplay {
public:
  static CDisplay_ptr Create();

  void SetLcd(ILiquidCrystal_uptr lcd);

  void SetRow0(std::string msg);
  void SetRow1(std::string msg);

  void Print();

private:
  CDisplay() = default;

  void SetRow(int row_index, std::string msg);

  ILiquidCrystal_uptr _lcd;
  std::array<std::string, 2> _rows;
};