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

    void Init();

    // Give the status of the network connection (n.b. not the status of the _server(s) or any _client connected to this _server)
    uint8_t ConnectionStatus();

  private:
    CWifiServer(CPresenter_ptr presenter,
                CDisplay_ptr display);

    void HandleClient();
    void HandleRequest(const std::string & line);
    void SendResponse();

    // Periodically (kReconnectInterval_ms) try to re-establish connection in case of disconnect
    void UpdateConnection();

    void SendKeyValue(std::string key, std::string value, bool add_comma);

    WiFiServer _server;
    WiFiClient _client;

    CPresenter_ptr _presenter;
    CDisplay_ptr _display;
    bool _first_run = true;
    long _last_connection_attempt_ms = 0;
};
