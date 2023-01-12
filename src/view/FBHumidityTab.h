#pragma once

#include "TabBase.h"

class CFBHumdity;
using CFBHumdity_ptr = std::shared_ptr<CFBHumdity>;

class CFBHumdity : public CTabBase {
public:
  static CFBHumdity_ptr Create(CPresenter_ptr presenter, ITabView_ptr tab_view,
                               CKeys_ptr keys, CDisplay_ptr display);

protected:
  CFBHumdity(CPresenter_ptr presenter, ITabView_ptr tab_view, CKeys_ptr keys,
             CDisplay_ptr display);

  void UpdateDisplay() override;

  void OnFuncLeftKey() override;
  void OnFuncRightKey() override;
};