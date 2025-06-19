#include "LiquidCrystalDisplay.h"

#include "DigitalOutput.h"

#include <LiquidCrystal.h>

CLiquidCrystalDisplay_uptr CLiquidCrystalDisplay::Create(SLCDPins pins)
{
    return CLiquidCrystalDisplay_uptr(new CLiquidCrystalDisplay(pins));
}

CLiquidCrystalDisplay::CLiquidCrystalDisplay(SLCDPins pins)
    : _backlight_power(CDigitalOutput::Create(pins.backlight, false))
{
    _lcd = std::unique_ptr<LiquidCrystal>(new LiquidCrystal(
        pins.Rs,
        pins.E,
        pins.D4_D7[0],
        pins.D4_D7[1],
        pins.D4_D7[2],
        pins.D4_D7[3]));

    _lcd->begin(16, 2);

    _backlight_power->TurnOn();
}

void CLiquidCrystalDisplay::Print(std::string msg)
{
    _lcd->print(String(msg.c_str()));
}

void CLiquidCrystalDisplay::Clear()
{
    _lcd->begin(16, 2); // Temp hack until caps are in place on all boards ...
}

void CLiquidCrystalDisplay::SetCursor(int column, int row)
{
    _lcd->setCursor(column, row);
}

bool CLiquidCrystalDisplay::Backlight()
{
    return _backlight_power->GetOutputState() == OutputStates::ON;
}

void CLiquidCrystalDisplay::SetBacklight(bool is_on)
{
    if (is_on == Backlight())
        return;

    if (is_on)
        _backlight_power->TurnOn();
    else
        _backlight_power->TurnOff();
}
