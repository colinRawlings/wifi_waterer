#pragma once

#include "../Types.h"
#include "../Updateable.h"

#include "TabBase.h"

#include "interfaces/ITabView.h"

#include <memory>
#include <vector>

class CDisplayKeys;
using CDisplayKeys_ptr = std::shared_ptr<CDisplayKeys>;

class CDisplay;
using CDisplay_ptr = std::shared_ptr<CDisplay>;

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CWifiServer;
using CWifiServer_ptr = std::shared_ptr<CWifiServer>;

//

class CSoftkeyDisplay;
using CSoftkeyDisplay_ptr = std::shared_ptr<CSoftkeyDisplay>;

class CSoftkeyDisplay : public ITabView, public std::enable_shared_from_this<CSoftkeyDisplay>, public CUpdateable
{
  public:
    static CSoftkeyDisplay_ptr Create(CPresenter_ptr presenter,
                                      CWifiServer_ptr wifi_server,
                                      CDisplayKeys_ptr keys,
                                      CDisplay_ptr display);

    void Update() override;

    bool OnNextTab() override;
    bool OnPreviousTab() override;

    bool OnKeyPressed() override;

  private:
    CSoftkeyDisplay(CPresenter_ptr presenter,
                    CWifiServer_ptr wifi_server,
                    CDisplayKeys_ptr keys,
                    CDisplay_ptr display);

    virtual ~CSoftkeyDisplay() = default;

    void OnCreate(CPresenter_ptr presenter,
                  CWifiServer_ptr wifi_server,
                  CDisplayKeys_ptr keys,
                  CDisplay_ptr display);

    void TurnOnDisplay();
    void TurnOffDisplay();

    std::vector<CTabBase_ptr> _tabs;
    size_t _active_tab{0};

    CPresenter_ptr _presenter;
    CWifiServer_ptr _wifi_server;
    CDisplayKeys_ptr _keys;
    CDisplay_ptr _display;

    long _last_keypress_ms;
};
