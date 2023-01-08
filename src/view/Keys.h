#pragma once

#include <memory>

class CKey;
using CKey_ptr = std::shared_ptr<CKey>;

//

class CKeys;
using CKeys_ptr = std::shared_ptr<CKeys>;

class CKeys {
public:
  static CKeys_ptr Create(CKey_ptr tab_left, CKey_ptr tab_right,
                          CKey_ptr func_left, CKey_ptr func_right);

  CKey_ptr TabLeft();
  CKey_ptr TabRight();
  CKey_ptr FuncLeft();
  CKey_ptr FuncRight();

protected:
  CKeys(CKey_ptr tab_left, CKey_ptr tab_right, CKey_ptr func_left,
        CKey_ptr func_right);

  CKey_ptr _tab_left;
  CKey_ptr _tab_right;
  CKey_ptr _func_left;
  CKey_ptr _func_right;
};