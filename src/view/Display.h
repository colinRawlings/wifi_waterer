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

  void SetRow0(String msg);
  void SetRow1(String msg);

  void Print();

private:
  CDisplay() = default;

  void SetRow(int row_index, String msg);

  ILiquidCrystal_uptr _lcd;
  std::array<String, 2> _rows;
};