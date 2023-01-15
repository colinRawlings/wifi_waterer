#include "SoftkeyDisplay.h"

#include "CurrentTimeTab.h"
#include "FBHourTab.h"
#include "FBHumidityTab.h"
#include "ManualPumpTab.h"

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
        CCurrentTimeTab::Create(presenter, shared_from_this(), keys, display),
        CFBHourTab::Create(presenter, shared_from_this(), keys, display),
        CFBHumdityTab::Create(presenter, shared_from_this(), keys, display),
        CManualPumpTab::Create(presenter, shared_from_this(), keys, display),
    };
}

void CSoftkeyDisplay::Update()
{
    CUpdateable::Update();
    _tabs[_active_tab]->Update();
}

void CSoftkeyDisplay::NextTab()
{
    _active_tab = (_active_tab + 1) % _tabs.size();
}
void CSoftkeyDisplay::PreviousTab()
{
    if (_active_tab == 0)
    {
        _active_tab = _tabs.size() - 1;
        return;
    }

    --_active_tab;
}

CSoftkeyDisplay::CSoftkeyDisplay(CPresenter_ptr presenter,
                                 CKeys_ptr keys, CDisplay_ptr display)

{
}
