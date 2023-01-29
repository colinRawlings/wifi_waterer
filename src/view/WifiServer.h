#pragma once

#include "../Types.h"
#include "../Updateable.h"

#include <memory>

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CDisplay;
using CDisplay_ptr = std::shared_ptr<CDisplay>;

class CWifiServer;
using CWifiServer_ptr = std::shared_ptr<CWifiServer>;

class CWifiServer : public CUpdateable, public std::enable_shared_from_this<CWifiServer>
{
  public:
    static CWifiServer_ptr Create(CPresenter_ptr presenter,
                                  CDisplay_ptr display);

    void Update() override;

  private:
    CWifiServer(CPresenter_ptr presenter,
                CDisplay_ptr display);

    void StartServer();

    void HandleClient();

    WiFiServer _server;
    WiFiClient _client;

    CPresenter_ptr _presenter;
    CDisplay_ptr _display;
};
