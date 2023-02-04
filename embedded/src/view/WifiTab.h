#pragma once

#pragma once

#include "../Types.h"

#include "TabBase.h"

class CWifiTab;
using CWifiTab_ptr = std::shared_ptr<CWifiTab>;

class CWifiTab : public CTabBase
{
  public:
    static CWifiTab_ptr Create(CPresenter_ptr presenter, ITabView_ptr tab_view,
                               CDisplayKeys_ptr keys, CDisplay_ptr display);

  protected:
    CWifiTab(CPresenter_ptr presenter, ITabView_ptr tab_view, CDisplayKeys_ptr keys,
             CDisplay_ptr display);

    void UpdateDisplay() override;

    void OnFuncLeftKey() override;
    void OnFuncRightKey() override;
};
