#pragma once

#include "../../Types.h"

#include <memory>
#include <string>

class ILiquidCrystal;
using ILiquidCrystal_uptr = std::unique_ptr<ILiquidCrystal>;

class ILiquidCrystal
{
  public:
    virtual void Print(std::string msg) = 0;
    virtual void Clear() = 0;
    virtual void SetCursor(int column, int row) = 0;

    virtual bool Backlight() = 0;
    virtual void SetBacklight(bool is_on) = 0;

    virtual ~ILiquidCrystal() = default;
};
