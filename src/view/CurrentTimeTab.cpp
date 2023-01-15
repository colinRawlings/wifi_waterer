#include "../Types.h"

#include "CurrentTimeTab.h"

#include "Display.h"

#include "Helpers.h"

#include "../presenter/Presenter.h"

CCurrentTimeTab_ptr CCurrentTimeTab::Create(CPresenter_ptr presenter,
                                            ITabView_ptr tab_view,
                                            CDisplayKeys_ptr keys,
                                            CDisplay_ptr display)
{
    auto destroy = [](CCurrentTimeTab * display) { delete display; };
    return CCurrentTimeTab_ptr(
        new CCurrentTimeTab(presenter, tab_view, keys, display), destroy);
}

CCurrentTimeTab::CCurrentTimeTab(CPresenter_ptr presenter,
                                 ITabView_ptr tab_view, CDisplayKeys_ptr keys,
                                 CDisplay_ptr display)
    : CTabBase("Time Now", presenter, tab_view, keys, display)
{}

void CCurrentTimeTab::UpdateDisplay()
{
    CTabBase::UpdateDisplay();

    if (!_display || !_presenter)
        return;

    _display->SetRow1(FormatRow("H", _presenter->GetCurrentTime(), "M"));
}

void CCurrentTimeTab::OnFuncLeftKey()
{
    if (!_presenter)
        return;

    _presenter->IncCurrentTimeHour();
}

void CCurrentTimeTab::OnFuncRightKey()
{
    if (!_presenter)
        return;

    _presenter->IncCurrentTimeMinute();
}
