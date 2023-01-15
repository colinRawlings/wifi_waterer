#include "../Types.h"

#include "ManualPumpTab.h"

#include "Display.h"
#include "Helpers.h"

#include "../presenter/Presenter.h"

CManualPumpTab_ptr
CManualPumpTab::Create(CPresenter_ptr presenter,
                       ITabView_ptr tab_view, CKeys_ptr keys,
                       CDisplay_ptr display)
{
    auto destroy = [](CManualPumpTab * display) { delete display; };
    return CManualPumpTab_ptr(new CManualPumpTab(presenter, tab_view, keys, display),
                              destroy);
}

CManualPumpTab::CManualPumpTab(CPresenter_ptr presenter, ITabView_ptr tab_view,
                               CKeys_ptr keys, CDisplay_ptr display)
    : CTabBase("Pump", presenter, tab_view, keys, display)
{}

void CManualPumpTab::UpdateDisplay()
{
    CTabBase::UpdateDisplay();

    if (!_display || !_presenter)
        return;

    auto remaining_time_msg = _presenter->RemainingPumpOnTimeS();

    _display->SetRow1(FormatRow("+", PumpIcon(_presenter->GetPumpStatus()) + " " + remaining_time_msg, "-"));
}

void CManualPumpTab::OnFuncLeftKey()
{
    if (!_presenter)
        return;

    _presenter->TurnOnPumpFor(5000);
}

void CManualPumpTab::OnFuncRightKey()
{
    if (!_presenter)
        return;

    _presenter->TurnOffPump();
}
