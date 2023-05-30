#pragma once

#pragma once

#include "../Types.h"

#include "TabBase.h"

class CWifiServer;
using CWifiServer_ptr = std::shared_ptr<CWifiServer>;

class CWifiTab;
using CWifiTab_ptr = std::shared_ptr<CWifiTab>;

class CWifiTab : public CTabBase
{
  public:
    static CWifiTab_ptr Create(CPresenter_ptr presenter,
                               CWifiServer_ptr wifi_server,
                               ITabView_ptr tab_view,
                               CDisplayKeys_ptr keys,
                               CDisplay_ptr display);

  protected:
    CWifiTab(CPresenter_ptr presenter,
             CWifiServer_ptr wifi_server,
             ITabView_ptr tab_view,
             CDisplayKeys_ptr keys,
             CDisplay_ptr display);

    void UpdateDisplay() override;

    void OnFuncLeftKey() override;
    void OnFuncRightKey() override;

  protected:
    CWifiServer_ptr _wifi_server;
};
