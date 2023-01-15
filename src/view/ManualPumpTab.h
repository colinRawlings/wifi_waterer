#pragma once

#include "../Types.h"

#include "TabBase.h"

class CManualPumpTab;
using CManualPumpTab_ptr = std::shared_ptr<CManualPumpTab>;

class CManualPumpTab : public CTabBase
{
  public:
    static CManualPumpTab_ptr Create(CPresenter_ptr presenter, ITabView_ptr tab_view,
                                     CDisplayKeys_ptr keys, CDisplay_ptr display);

  protected:
    CManualPumpTab(CPresenter_ptr presenter, ITabView_ptr tab_view, CDisplayKeys_ptr keys,
                   CDisplay_ptr display);

    void UpdateDisplay() override;

    void OnFuncLeftKey() override;
    void OnFuncRightKey() override;
};
