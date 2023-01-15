#include "SoftkeyDisplay.h"

#include "HomeTab.h"
#include "CurrentTimeTab.h"
#include "FBHourTab.h"
#include "FBHumidityTab.h"
#include "ManualPumpTab.h"
#include "FBDurationTab.h"
#include "SaveTab.h"
#include "Display.h"

static const long kDisplayOffMs{30000};

CSoftkeyDisplay_ptr CSoftkeyDisplay::Create(CPresenter_ptr presenter,
                                            CKeys_ptr keys, CDisplay_ptr display)
{
    auto destroy = [](CSoftkeyDisplay * raw_app) { delete raw_app; };
    auto app = CSoftkeyDisplay_ptr(new CSoftkeyDisplay(presenter, keys, display), destroy);

    app->OnCreate(presenter, keys, display);

    return app;
}

void CSoftkeyDisplay::OnCreate(CPresenter_ptr presenter,
                               CKeys_ptr keys, CDisplay_ptr display)
{
    _tabs = {
        CHomeTab::Create(presenter, shared_from_this(), keys, display),
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

void CSoftkeyDisplay::OnNextTab()
{
    _active_tab = (_active_tab + 1) % _tabs.size();
}
void CSoftkeyDisplay::OnPreviousTab()
{
    if (_active_tab == 0)
    {
        _active_tab = _tabs.size() - 1;
        return;
    }

    --_active_tab;
}

void CSoftkeyDisplay::OnKeyPressed()
{
    _last_keypress_ms = millis();
    TurnOnDisplay();
}

CSoftkeyDisplay::CSoftkeyDisplay(CPresenter_ptr presenter,
                                 CKeys_ptr keys, CDisplay_ptr display)
    : _presenter(presenter)
    , _display(display)

{
    OnKeyPressed();
}
