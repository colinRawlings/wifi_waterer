#include "../Types.h"

#include "FBHumidityTab.h"

#include "Display.h"

#include "../presenter/Presenter.h"

CFBHumdity_ptr CFBHumdity::Create(CPresenter_ptr presenter,
                                  ITabView_ptr tab_view, CKeys_ptr keys,
                                  CDisplay_ptr display) {
  auto destroy = [](CFBHumdity *display) { delete display; };
  return CFBHumdity_ptr(new CFBHumdity(presenter, tab_view, keys, display),
                        destroy);
}

CFBHumdity::CFBHumdity(CPresenter_ptr presenter, ITabView_ptr tab_view,
                       CKeys_ptr keys, CDisplay_ptr display)
    : CTabBase("FB Humidity (V)", presenter, tab_view, keys, display) {}

void CFBHumdity::UpdateDisplay() {
  CTabBase::UpdateDisplay();

  if (!_display || !_presenter)
    return;

  _display->SetRow1(_presenter->GetFBHumidityV());
}

void CFBHumdity::OnFuncLeftKey() {
  if (!_presenter)
    return;

  _presenter->IncFBHumidityV();
}

void CFBHumdity::OnFuncRightKey() {
  if (!_presenter)
    return;

  _presenter->DecFBHumidityV();
}