#include "LiquidCrystalDisplay.h"

#include <LiquidCrystal.h>

CLiquidCrystalDisplay_uptr CLiquidCrystalDisplay::Create(SLCDPins pins)
{
    return CLiquidCrystalDisplay_uptr(new CLiquidCrystalDisplay(pins));
}

void CLiquidCrystalDisplay::Print(std::string msg)
{
    _lcd->print(String(msg.c_str()));
}

void CLiquidCrystalDisplay::Clear()
{
    _lcd->clear();
}

void CLiquidCrystalDisplay::SetCursor(int column, int row)
{
    _lcd->setCursor(column, row);
}

CLiquidCrystalDisplay::CLiquidCrystalDisplay(SLCDPins pins)
{
    _lcd = std::unique_ptr<LiquidCrystal>(new LiquidCrystal(
        pins.Rs,
        pins.E,
        pins.D4_D7[0],
        pins.D4_D7[1],
        pins.D4_D7[2],
        pins.D4_D7[3]));

    _lcd->begin(16, 2);
}
