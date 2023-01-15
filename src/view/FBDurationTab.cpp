#include "../Types.h"

#include "FBDurationTab.h"

#include "Display.h"
#include "Helpers.h"

#include "../presenter/Presenter.h"

CFBDurationTab_ptr CFBDurationTab::Create(CPresenter_ptr presenter,
                                          ITabView_ptr tab_view, CKeys_ptr keys,
                                          CDisplay_ptr display)
{
    auto destroy = [](CFBDurationTab * display) { delete display; };
    return CFBDurationTab_ptr(new CFBDurationTab(presenter, tab_view, keys, display),
                              destroy);
}

CFBDurationTab::CFBDurationTab(CPresenter_ptr presenter, ITabView_ptr tab_view,
                               CKeys_ptr keys, CDisplay_ptr display)
    : CTabBase("FB Duration", presenter, tab_view, keys, display)
{}

void CFBDurationTab::UpdateDisplay()
{
    CTabBase::UpdateDisplay();

    if (!_display || !_presenter)
        return;

    _display->SetRow1(FormatRow("-", _presenter->GetFBPumpDurationS(), "+"));
}

void CFBDurationTab::OnFuncLeftKey()
{
    if (!_presenter)
        return;

    _presenter->IncFBPumpDurationMs();
}

void CFBDurationTab::OnFuncRightKey()
{
    if (!_presenter)
        return;

    _presenter->DecFBPumpDurationMs();
}
