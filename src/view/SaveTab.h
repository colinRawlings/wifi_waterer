#pragma once

#include "../Types.h"

#include "TabBase.h"

class CSaveTab;
using CSaveTab_ptr = std::shared_ptr<CSaveTab>;

class CSaveTab : public CTabBase
{
  public:
    static CSaveTab_ptr Create(CPresenter_ptr presenter,
                               ITabView_ptr tab_view, CKeys_ptr keys,
                               CDisplay_ptr display);

  protected:
    CSaveTab(CPresenter_ptr presenter, ITabView_ptr tab_view,
             CKeys_ptr keys, CDisplay_ptr display);

    void UpdateDisplay() override;

    void OnFuncLeftKey() override;
    void OnFuncRightKey() override;
};
