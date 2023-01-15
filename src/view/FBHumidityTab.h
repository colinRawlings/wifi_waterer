#pragma once

#include "../Types.h"

#include "TabBase.h"

class CFBHumdityTab;
using CFBHumdityTab_ptr = std::shared_ptr<CFBHumdityTab>;

class CFBHumdityTab : public CTabBase
{
  public:
    static CFBHumdityTab_ptr Create(CPresenter_ptr presenter, ITabView_ptr tab_view,
                                    CKeys_ptr keys, CDisplay_ptr display);

  protected:
    CFBHumdityTab(CPresenter_ptr presenter, ITabView_ptr tab_view, CKeys_ptr keys,
                  CDisplay_ptr display);

    void UpdateDisplay() override;

    void OnFuncLeftKey() override;
    void OnFuncRightKey() override;
};
