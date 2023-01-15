#pragma once

#include "../Types.h"

#include "TabBase.h"

class CHomeTab;
using CHomeTab_ptr = std::shared_ptr<CHomeTab>;

class CHomeTab : public CTabBase
{
  public:
    static CHomeTab_ptr Create(CPresenter_ptr presenter, ITabView_ptr tab_view,
                               CKeys_ptr keys, CDisplay_ptr display);

  protected:
    CHomeTab(CPresenter_ptr presenter, ITabView_ptr tab_view, CKeys_ptr keys,
             CDisplay_ptr display);

    void UpdateDisplay() override;

    void OnFuncLeftKey() override;
    void OnFuncRightKey() override;
};
