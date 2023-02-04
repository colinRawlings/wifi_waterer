#pragma once

#include "../Types.h"

#include <memory>

class CKey;
using CKey_ptr = std::shared_ptr<CKey>;

//

class CDisplayKeys;
using CDisplayKeys_ptr = std::shared_ptr<CDisplayKeys>;

class CDisplayKeys
{
  public:
    static CDisplayKeys_ptr Create(CKey_ptr tab_left, CKey_ptr tab_right,
                                   CKey_ptr func_left, CKey_ptr func_right);

    CKey_ptr TabLeft();
    CKey_ptr TabRight();
    CKey_ptr FuncLeft();
    CKey_ptr FuncRight();

  protected:
    CDisplayKeys(CKey_ptr tab_left, CKey_ptr tab_right, CKey_ptr func_left,
                 CKey_ptr func_right);

    CKey_ptr _tab_left;
    CKey_ptr _tab_right;
    CKey_ptr _func_left;
    CKey_ptr _func_right;
};
