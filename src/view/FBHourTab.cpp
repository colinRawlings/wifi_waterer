#include "../Types.h"

#include "FBHourTab.h"

#include "Display.h"
#include "Helpers.h"

#include "../presenter/Presenter.h"

CFBHourTab_ptr CFBHourTab::Create(CPresenter_ptr presenter,
                                  ITabView_ptr tab_view, CKeys_ptr keys,
                                  CDisplay_ptr display)
{
    auto destroy = [](CFBHourTab * display) { delete display; };
    return CFBHourTab_ptr(new CFBHourTab(presenter, tab_view, keys, display),
                          destroy);
}

CFBHourTab::CFBHourTab(CPresenter_ptr presenter, ITabView_ptr tab_view,
                       CKeys_ptr keys, CDisplay_ptr display)
    : CTabBase("FB Time", presenter, tab_view, keys, display)
{}

void CFBHourTab::UpdateDisplay()
{
    CTabBase::UpdateDisplay();

    if (!_display || !_presenter)
        return;

    _display->SetRow1(FormatRow("-", _presenter->GetFBTime(), "+"));
}

void CFBHourTab::OnFuncLeftKey()
{
    if (!_presenter)
        return;

    _presenter->DecFBTime();
}

void CFBHourTab::OnFuncRightKey()
{
    if (!_presenter)
        return;

    _presenter->IncFBTime();
}
