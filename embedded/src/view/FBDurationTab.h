#pragma once

#include "../Types.h"

#include "TabBase.h"

class CFBDurationTab;
using CFBDurationTab_ptr = std::shared_ptr<CFBDurationTab>;

class CFBDurationTab : public CTabBase
{
  public:
    static CFBDurationTab_ptr Create(CPresenter_ptr presenter, ITabView_ptr tab_view,
                                     CDisplayKeys_ptr keys, CDisplay_ptr display);

  protected:
    CFBDurationTab(CPresenter_ptr presenter, ITabView_ptr tab_view, CDisplayKeys_ptr keys,
                   CDisplay_ptr display);

    void UpdateDisplay() override;

    void OnFuncLeftKey() override;
    void OnFuncRightKey() override;
};
