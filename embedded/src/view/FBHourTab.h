#pragma once

#include "../Types.h"

#include "TabBase.h"

class CFBHourTab;
using CFBHourTab_ptr = std::shared_ptr<CFBHourTab>;

class CFBHourTab : public CTabBase
{
  public:
    static CFBHourTab_ptr Create(CPresenter_ptr presenter, ITabView_ptr tab_view,
                                 CDisplayKeys_ptr keys, CDisplay_ptr display);

  protected:
    CFBHourTab(CPresenter_ptr presenter, ITabView_ptr tab_view, CDisplayKeys_ptr keys,
               CDisplay_ptr display);

    void UpdateDisplay() override;

    void OnFuncLeftKey() override;
    void OnFuncRightKey() override;
};
