#include "Tabs.h"

#include "CurrentTimeTab.h"
#include "FBHourTab.h"
#include "FBHumidityTab.h"

CTabs_ptr CTabs::Create(CPresenter_ptr presenter,
                        CKeys_ptr keys, CDisplay_ptr display)
{
    auto destroy = [](CTabs * display) { delete display; };
    auto tabs = CTabs_ptr(new CTabs(presenter, keys, display),
                          destroy);

    tabs->OnCreate(presenter, keys, display);

    return tabs;
}

void CTabs::OnCreate(CPresenter_ptr presenter,
                     CKeys_ptr keys, CDisplay_ptr display)
{
    _tabs = {
        CCurrentTimeTab::Create(presenter, shared_from_this(), keys, display),
        CFBHourTab::Create(presenter, shared_from_this(), keys, display),
        CFBHumdityTab::Create(presenter, shared_from_this(), keys, display),
    };
}

void CTabs::Update()
{
    _tabs[_active_tab]->Update();
}

void CTabs::NextTab()
{
    _active_tab = (_active_tab + 1) % _tabs.size();
}
void CTabs::PreviousTab()
{
    if (_active_tab == 0)
    {
        _active_tab = _tabs.size() - 1;
        return;
    }

    --_active_tab;
}

CTabs::CTabs(CPresenter_ptr presenter,
             CKeys_ptr keys, CDisplay_ptr display)
{
}
