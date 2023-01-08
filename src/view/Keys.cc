#include "Keys.h"

CKeys_ptr CKeys::Create(CKey_ptr tab_left, CKey_ptr tab_right,
                        CKey_ptr func_left, CKey_ptr func_right) {
  auto destroy = [](CKeys *display) { delete display; };
  return CKeys_ptr(new CKeys(tab_left, tab_right, func_left, func_right),
                   destroy);
}

CKeys::CKeys(CKey_ptr tab_left, CKey_ptr tab_right, CKey_ptr func_left,
             CKey_ptr func_right)
    : _tab_left(tab_left), _tab_right(tab_right), _func_left(func_left),
      _func_right(func_right) {}

CKey_ptr CKeys::TabLeft() { return _tab_left; }
CKey_ptr CKeys::TabRight() { return _tab_right; }
CKey_ptr CKeys::FuncLeft() { return _func_left; }
CKey_ptr CKeys::FuncRight() { return _func_right; }
