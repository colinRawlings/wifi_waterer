#include "../Types.h"

#include "FBHumidityTab.h"

#include "Display.h"
#include "Helpers.h"

#include "../presenter/Presenter.h"

CFBHumdityTab_ptr CFBHumdityTab::Create(CPresenter_ptr presenter,
                                        ITabView_ptr tab_view, CDisplayKeys_ptr keys,
                                        CDisplay_ptr display)
{
    auto destroy = [](CFBHumdityTab * display) { delete display; };
    return CFBHumdityTab_ptr(new CFBHumdityTab(presenter, tab_view, keys, display),
                             destroy);
}

CFBHumdityTab::CFBHumdityTab(CPresenter_ptr presenter, ITabView_ptr tab_view,
                             CDisplayKeys_ptr keys, CDisplay_ptr display)
    : CTabBase("FB Humidity", presenter, tab_view, keys, display)
{}

void CFBHumdityTab::UpdateDisplay()
{
    CTabBase::UpdateDisplay();

    if (!_display || !_presenter)
        return;

    _display->SetRow1(FormatRow("-", _presenter->GetFBHumidityV(), "+"));
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
