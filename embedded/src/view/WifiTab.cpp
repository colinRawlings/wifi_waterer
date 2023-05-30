#include "../Types.h"

#include "WifiTab.h"
#include "WifiServer.h"

#include "Display.h"
#include "Helpers.h"

#include "../model/WifiUtils.h"

CWifiTab_ptr CWifiTab::Create(CPresenter_ptr presenter,
                              CWifiServer_ptr wifi_server,
                              ITabView_ptr tab_view,
                              CDisplayKeys_ptr keys,
                              CDisplay_ptr display)
{
    auto destroy = [](CWifiTab * display) { delete display; };
    return CWifiTab_ptr(new CWifiTab(presenter,
                                     wifi_server,
                                     tab_view,
                                     keys,
                                     display),
                        destroy);
}

CWifiTab::CWifiTab(CPresenter_ptr presenter,
                   CWifiServer_ptr wifi_server,
                   ITabView_ptr tab_view,
                   CDisplayKeys_ptr keys,
                   CDisplay_ptr display)
    : CTabBase("WiFi", presenter, tab_view, keys, display)
    , _wifi_server(wifi_server)
{}

void CWifiTab::UpdateDisplay()
{
#ifndef ARDUINO_TESTING // TODO
    if (!_display || !_presenter || !_wifi_server)
        return;

    std::string ssid{WiFi.SSID()};
    if (ssid.size() > 10)
        ssid = ssid.substr(0, 10);

    _display->SetRow0(FormatRow("<", ssid, ">"));

    auto status = _wifi_server->ConnectionStatus();

    if (status == WL_CONNECTED)
    {
        IPAddress ip = WiFi.localIP();

        std::string ip_str;

        for (int p{0}; p < 3; ++p)
        {
            ip_str += std::to_string(ip[p]) + ".";
        }
        ip_str += std::to_string(ip[3]);

        _display->SetRow1(FormatRow("", ip_str, ""));
    }
    else if (status == WL_CONNECTION_LOST)
    {
        _display->SetRow1(FormatRow("C", "Conn Lost", "S"));
    }
    else if (status == WL_DISCONNECTED)
    {
        _display->SetRow1(FormatRow("C", "Disconnected", "S"));
    }
    else
    {
        _display->SetRow1(FormatRow("C", "Status: " + std::to_string(status), "S"));
    }

#endif
}

void CWifiTab::OnFuncLeftKey()
{
#ifndef ARDUINO_TESTING // TODO
    if (!_wifi_server)
        return;

    _wifi_server->Init();
#endif
}

void CWifiTab::OnFuncRightKey()
{
    // todo read network details from serial
    // restart network
}
