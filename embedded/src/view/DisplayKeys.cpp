#include "../Types.h"

#include "DisplayKeys.h"

#include "Key.h"

#include "../hardware/PushSwitch.h"

CDisplayKeys_ptr CDisplayKeys::Create(CKey_ptr tab_left, CKey_ptr tab_right,
                                      CKey_ptr func_left, CKey_ptr func_right)
{
    auto destroy = [](CDisplayKeys * display) { delete display; };
    return CDisplayKeys_ptr(new CDisplayKeys(tab_left, tab_right, func_left, func_right),
                            destroy);
}

CDisplayKeys::CDisplayKeys(CKey_ptr tab_left, CKey_ptr tab_right, CKey_ptr func_left,
                           CKey_ptr func_right)
    : _tab_left(tab_left)
    , _tab_right(tab_right)
    , _func_left(func_left)
    , _func_right(func_right)
{}

CKey_ptr CDisplayKeys::TabLeft() { return _tab_left; }
CKey_ptr CDisplayKeys::TabRight() { return _tab_right; }
CKey_ptr CDisplayKeys::FuncLeft() { return _func_left; }
CKey_ptr CDisplayKeys::FuncRight() { return _func_right; }
