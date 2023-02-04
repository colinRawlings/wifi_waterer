#include "../Types.h"

#include "HomeTab.h"

#include "Display.h"
#include "Helpers.h"

#include "../presenter/Presenter.h"

CHomeTab_ptr CHomeTab::Create(CPresenter_ptr presenter,
                              ITabView_ptr tab_view, CDisplayKeys_ptr keys,
                              CDisplay_ptr display)
{
    auto destroy = [](CHomeTab * display) { delete display; };
    return CHomeTab_ptr(new CHomeTab(presenter, tab_view, keys, display),
                        destroy);
}

CHomeTab::CHomeTab(CPresenter_ptr presenter, ITabView_ptr tab_view,
                   CDisplayKeys_ptr keys, CDisplay_ptr display)
    : CTabBase("Home", presenter, tab_view, keys, display)
{}

void CHomeTab::UpdateDisplay()
{

    if (!_display || !_presenter)
        return;

    _display->SetRow0(FormatRow("<", _presenter->GetCurrentTime(), ">"));

    _display->SetRow1(FormatRow("", PumpIcon(_presenter->GetPumpStatus()) + " " + _presenter->GetHumidityV(), ""));
}

void CHomeTab::OnFuncLeftKey()
{
    if (!_presenter)
        return;
}

void CHomeTab::OnFuncRightKey()
{
    if (!_presenter)
        return;
}
