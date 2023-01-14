#include "Application.h"

#include "CurrentTimeTab.h"
#include "FBHourTab.h"
#include "FBHumidityTab.h"

CApplication_ptr CApplication::Create(CPresenter_ptr presenter,
                                      CKeys_ptr keys, CDisplay_ptr display)
{
    auto destroy = [](CApplication * display) { delete display; };
    auto tabs = CApplication_ptr(new CApplication(presenter, keys, display),
                                 destroy);

    tabs->OnCreate(presenter, keys, display);

    return tabs;
}

void CApplication::OnCreate(CPresenter_ptr presenter,
                            CKeys_ptr keys, CDisplay_ptr display)
{
    _tabs = {
        CCurrentTimeTab::Create(presenter, shared_from_this(), keys, display),
        CFBHourTab::Create(presenter, shared_from_this(), keys, display),
        CFBHumdityTab::Create(presenter, shared_from_this(), keys, display),
    };
}

void CApplication::Update()
{
    _tabs[_active_tab]->Update();
}

void CApplication::NextTab()
{
    _active_tab = (_active_tab + 1) % _tabs.size();
}
void CApplication::PreviousTab()
{
    if (_active_tab == 0)
    {
        _active_tab = _tabs.size() - 1;
        return;
    }

    --_active_tab;
}

CApplication::CApplication(CPresenter_ptr presenter,
                           CKeys_ptr keys, CDisplay_ptr display)
{
}
