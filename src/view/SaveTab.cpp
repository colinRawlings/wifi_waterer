#include "../Types.h"

#include "SaveTab.h"

#include "Display.h"

#include "Helpers.h"

#include "../presenter/Presenter.h"

CSaveTab_ptr CSaveTab::Create(CPresenter_ptr presenter,
                              ITabView_ptr tab_view,
                              CDisplayKeys_ptr keys,
                              CDisplay_ptr display)
{
    auto destroy = [](CSaveTab * display) { delete display; };
    return CSaveTab_ptr(
        new CSaveTab(presenter, tab_view, keys, display), destroy);
}

CSaveTab::CSaveTab(CPresenter_ptr presenter,
                   ITabView_ptr tab_view, CDisplayKeys_ptr keys,
                   CDisplay_ptr display)
    : CTabBase("Save/Load", presenter, tab_view, keys, display)
{}

void CSaveTab::UpdateDisplay()
{
    CTabBase::UpdateDisplay();

    if (!_display || !_presenter)
        return;

    _display->SetRow1(FormatRow("Save", "", "Load"));
}

void CSaveTab::OnFuncLeftKey()
{
    if (!_presenter)
        return;

    _presenter->SaveFBSettingsToFlash();
}

void CSaveTab::OnFuncRightKey()
{
    if (!_presenter)
        return;

    _presenter->LoadFBSettingsFromFlash();
}
