#include "Display.h"

#include "../hardware/LiquidCrystalDisplay.h"

#include <iostream>
#include <vector>

static const int kRowLength{16};

CDisplay_ptr CDisplay::Create(SLCDPins pins)
{
    auto display = CDisplay::Create();

    auto lcd = CLiquidCrystalDisplay::Create(pins);
    display->SetLcd(std::move(lcd));

    return display;
}

CDisplay_ptr CDisplay::Create()
{
    auto destroy = [](CDisplay * display) { delete display; };
    return CDisplay_ptr(new CDisplay, destroy);
}

void CDisplay::SetLcd(ILiquidCrystal_uptr lcd) { _lcd = std::move(lcd); }

void CDisplay::SetRow0(std::string msg) { SetRow(0, msg); }

void CDisplay::SetRow1(std::string msg) { SetRow(1, msg); }

void CDisplay::SetRow(int row_idx, std::string msg)
{
    if (row_idx != 0 && row_idx != 1)
        return;

    if (_rows[row_idx] == msg)
        return;

    _rows[row_idx] = msg;

    if (!_lcd)
        return;

    _lcd->Clear();
    for (const auto & idx : {0, 1})
    {
        _lcd->SetCursor(0, idx);
        _lcd->Print(_rows[idx]);
    }
}

//

void CDisplay::Print()
{
    std::string sep(kRowLength + 2, '*');
    std::cout << sep << std::endl;

    for (const auto & idx : {0, 1})
    {
        std::cout << "*";

        auto row = _rows[idx];

        if (row.size() > kRowLength)
        {
            std::cout << row.substr(0, kRowLength);
        }
        else
        {
            std::cout << row;
            std::cout << std::string(kRowLength - row.size(), ' ');
        }
        std::cout << "*" << std::endl;
    }
    std::cout << sep << std::endl;
}
