#include "SoftkeyDisplay.h"
#include "WifiServer.h"

#include "HomeTab.h"
#include "CurrentTimeTab.h"
#include "FBHourTab.h"
#include "FBHumidityTab.h"
#include "ManualPumpTab.h"
#include "FBDurationTab.h"
#include "WifiTab.h"
#include "SaveTab.h"
#include "Display.h"

#include "../presenter/Presenter.h"

static const long kDisplayOffMs{120000};

CSoftkeyDisplay_ptr CSoftkeyDisplay::Create(CPresenter_ptr presenter,
                                            CWifiServer_ptr wifi_server,
                                            CDisplayKeys_ptr keys,
                                            CDisplay_ptr display)
{
    auto destroy = [](CSoftkeyDisplay * raw_app) { delete raw_app; };
    auto app = CSoftkeyDisplay_ptr(new CSoftkeyDisplay(presenter,
                                                       wifi_server,
                                                       keys,
                                                       display),
                                   destroy);

    app->OnCreate(presenter, wifi_server, keys, display);

    return app;
}

void CSoftkeyDisplay::OnCreate(CPresenter_ptr presenter,
                               CWifiServer_ptr wifi_server,
                               CDisplayKeys_ptr keys,
                               CDisplay_ptr display)
{
    _tabs = {
        CHomeTab::Create(presenter, shared_from_this(), keys, display),
        CWifiTab::Create(presenter, wifi_server, shared_from_this(), keys, display),
        CManualPumpTab::Create(presenter, shared_from_this(), keys, display),
        CCurrentTimeTab::Create(presenter, shared_from_this(), keys, display),
        CFBHourTab::Create(presenter, shared_from_this(), keys, display),
        CFBDurationTab::Create(presenter, shared_from_this(), keys, display),
        CFBHumdityTab::Create(presenter, shared_from_this(), keys, display),
        CSaveTab::Create(presenter, shared_from_this(), keys, display),
    };
}

void CSoftkeyDisplay::Update()
{
    CUpdateable::Update();
    _tabs[_active_tab]->Update();

    if (millis() - _last_keypress_ms > kDisplayOffMs)
    {
        TurnOffDisplay();
        _active_tab = 0;
    }
}

void CSoftkeyDisplay::TurnOnDisplay()
{
    if (!_display)
        return;

    _display->SetBacklight(true);
}

void CSoftkeyDisplay::TurnOffDisplay()
{
    if (!_display)
        return;

    _display->SetBacklight(false);
}

bool CSoftkeyDisplay::OnNextTab()
{
    _active_tab = (_active_tab + 1) % _tabs.size();
    LogLn("Selected tab: " + _tabs[_active_tab]->Name());

    return true;
}
bool CSoftkeyDisplay::OnPreviousTab()
{
    if (_active_tab == 0)
    {
        _active_tab = _tabs.size() - 1;
        return false;
    }

    --_active_tab;
    LogLn("Selected tab: " + _tabs[_active_tab]->Name());

    return true;
}

bool CSoftkeyDisplay::OnKeyPressed()
{
    _last_keypress_ms = millis();

    if (!_display)
        return false;

    if (!_display->Backlight())
    {
        TurnOnDisplay();
        return true;
    }

    return false;
}

CSoftkeyDisplay::CSoftkeyDisplay(CPresenter_ptr presenter,
                                 CWifiServer_ptr wifi_server,
                                 CDisplayKeys_ptr keys,
                                 CDisplay_ptr display)
    : _presenter(presenter)
    , _wifi_server(wifi_server)
    , _display(display)
    , _keys(keys)
{
    OnKeyPressed();
}
