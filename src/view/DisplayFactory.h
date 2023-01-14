#include "../Types.h"

#include "../hardware/SLCDPins.h"
#include "../hardware/LiquidCrystalDisplay.h"

#include "Display.h"

CDisplay_ptr CreateDisplay(SLCDPins pins)
{
    auto display = CDisplay::Create();

    auto lcd = CLiquidCrystalDisplay::Create(pins);
    display->SetLcd(std::move(lcd));

    return display;
}
