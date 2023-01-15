#include "LiquidCrystalDisplay.h"

#include "DigitalOutput.h"

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

void CLiquidCrystalDisplay::SetBacklight(bool is_on)
{
    if (is_on == (_backlight_power->GetOutputState() == OutputStates::ON))
        return;

    if (is_on)
        _backlight_power->TurnOn();
    else
        _backlight_power->TurnOff();
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

    _backlight_power = std::unique_ptr<CDigitalOutput>(new CDigitalOutput(pins.backlight, false));
    _backlight_power->TurnOn();
}
