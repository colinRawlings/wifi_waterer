#include "../Types.h"

#include "FBHumidityTab.h"

#include "Display.h"

#include "../presenter/Presenter.h"

CFBHumdity_ptr CFBHumdityTab::Create(CPresenter_ptr presenter,
                                     ITabView_ptr tab_view, CKeys_ptr keys,
                                     CDisplay_ptr display)
{
    auto destroy = [](CFBHumdityTab * display) { delete display; };
    return CFBHumdity_ptr(new CFBHumdityTab(presenter, tab_view, keys, display),
                          destroy);
}

CFBHumdityTab::CFBHumdityTab(CPresenter_ptr presenter, ITabView_ptr tab_view,
                             CKeys_ptr keys, CDisplay_ptr display)
    : CTabBase("FB Humidity (V)", presenter, tab_view, keys, display)
{}

void CFBHumdityTab::UpdateDisplay()
{
    CTabBase::UpdateDisplay();

    if (!_display || !_presenter)
        return;

    _display->SetRow1(_presenter->GetFBHumidityV());
}

void CFBHumdityTab::OnFuncLeftKey()
{
    if (!_presenter)
        return;

    _presenter->DecFBHumidityV();
}

void CFBHumdityTab::OnFuncRightKey()
{
    if (!_presenter)
        return;

    _presenter->IncFBHumidityV();
}
