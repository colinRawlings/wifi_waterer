#pragma once

#include "Types.h"

class ILiquidCrystal;
using ILiquidCrystal_ptr = std::shared_ptr<ILiquidCrystal>;
using ILiquidCrystal_wptr = std::weak_ptr<ILiquidCrystal>;

class ILiquidCrystal {
public:
  virtual void print(String msg) = 0;
  virtual void clear() = 0;
  virtual void setCursor(int column, int row) = 0;
};