#include "Display.h"

#include <iostream>
#include <vector>

static const int kRowLength{16};
static const long kUpdateIntervalMs{1000};

CDisplay_ptr CDisplay::Create()
{
    auto destroy = [](CDisplay * display) { delete display; };
    return CDisplay_ptr(new CDisplay, destroy);
}

CDisplay::CDisplay()
    : _last_row_update{millis(), millis()}
{}

void CDisplay::SetLcd(ILiquidCrystal_uptr lcd) { _lcd = std::move(lcd); }

bool CDisplay::Backlight()
{
    if (!_lcd)
        return false;

    return _lcd->Backlight();
}

void CDisplay::SetBacklight(bool is_on)
{
    if (!_lcd)
        return;

    _lcd->SetBacklight(is_on);
}

void CDisplay::SetRow0(std::string msg) { SetRow(0, msg); }

void CDisplay::SetRow1(std::string msg) { SetRow(1, msg); }

void CDisplay::SetRow(int row_idx, std::string msg)
{
    if (row_idx != 0 && row_idx != 1)
        return;

    if (_rows[row_idx] == msg)
        return;

    if(millis()-_last_row_update[row_idx] < kUpdateIntervalMs)
        return;

    _last_row_update[row_idx] = millis();

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
