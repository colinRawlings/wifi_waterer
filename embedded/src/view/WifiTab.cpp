#include "../Types.h"

#include "WifiTab.h"

#include "Display.h"
#include "Helpers.h"

#include "../model/WifiUtils.h"

CWifiTab_ptr CWifiTab::Create(CPresenter_ptr presenter,
                              ITabView_ptr tab_view, CDisplayKeys_ptr keys,
                              CDisplay_ptr display)
{
    auto destroy = [](CWifiTab * display) { delete display; };
    return CWifiTab_ptr(new CWifiTab(presenter, tab_view, keys, display),
                        destroy);
}

CWifiTab::CWifiTab(CPresenter_ptr presenter, ITabView_ptr tab_view,
                   CDisplayKeys_ptr keys, CDisplay_ptr display)
    : CTabBase("WiFi", presenter, tab_view, keys, display)
{}

void CWifiTab::UpdateDisplay()
{
#ifndef ARDUINO_TESTING // TODO
    if (!_display || !_presenter)
        return;

    std::string ssid{WiFi.SSID()};
    if (ssid.size() > 10)
        ssid = ssid.substr(0, 10);

    _display->SetRow0(FormatRow("<", ssid, ">"));

    IPAddress ip = WiFi.localIP();

    std::string ip_str;

    for (int p{0}; p < 3; ++p)
    {
        ip_str += std::to_string(ip[p]) + ".";
    }
    ip_str += std::to_string(ip[3]);

    _display->SetRow1(FormatRow("", ip_str, ""));
#endif
}

void CWifiTab::OnFuncLeftKey()
{
#ifndef ARDUINO_TESTING // TODO
    LogConnectInfo();
#endif
}

void CWifiTab::OnFuncRightKey()
{
    // todo read network details from serial
    // restart network
}
