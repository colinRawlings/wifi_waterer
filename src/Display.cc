#include "Display.h"

#include <iostream>
#include <vector>

static const int kRowLength{16};

CDisplay_ptr CDisplay::Create() {
  auto destroy = [](CDisplay *display) { delete display; };
  return CDisplay_ptr(new CDisplay, destroy);
}

void CDisplay::SetLcd(ILiquidCrystal_ptr lcd) { _lcd = lcd; }
void CDisplay::SetRow0(String msg) { SetRow(0, msg); }
void CDisplay::SetRow1(String msg) { SetRow(1, msg); }

void CDisplay::SetRow(int row_idx, String msg) {
  if (row_idx != 0 && row_idx != 1)
    return;

  if (_rows[row_idx] == msg)
    return;

  _rows[row_idx] = msg;

  auto lcd = _lcd.lock();
  if (!lcd)
    return;

  lcd->clear();
  for (const auto &idx : {0, 1}) {
    lcd->setCursor(0, idx);
    lcd->print(_rows[idx]);
  }
}

//

void CDisplay::Print() {
  std::string sep(kRowLength + 2, '*');
  std::cout << sep << std::endl;

  for (const auto &idx : {0, 1}) {
    std::cout << "*";

    auto row = _rows[idx];

    if (row.size() > kRowLength) {
      std::cout << row.substr(0, kRowLength);
    } else {
      std::cout << row;
      std::cout << std::string(kRowLength - row.size(), ' ');
    }
    std::cout << "*" << std::endl;
  }
  std::cout << sep << std::endl;
}
