#include "Presenter.h"

#include "../model/FBSettings.h"

namespace {
String FmtTimeGroup(byte value) {
  String str;
  if (value < 10)
    str += "0";

  str += std::to_string(value);

  return str;
}
} // namespace

CPresenter_ptr CPresenter::Create() {
  auto destroy = [](CPresenter *display) { delete display; };
  return CPresenter_ptr(new CPresenter, destroy);
}

// Current Time
void CPresenter::SetRealTimeClock(IRealTimeClock_uptr rtc) {
  _rtc = std::move(rtc);
}

String CPresenter::GetCurrentTime() {
  if (!_rtc)
    return "";

  return FmtTimeGroup(_rtc->getHours()) + ":" +
         FmtTimeGroup(_rtc->getMinutes());
}

void CPresenter::IncCurrentTimeHour() {
  if (!_rtc)
    return;

  _rtc->setHours(_rtc->getHours() + 1);
}

void CPresenter::IncCurrentTimeMinute() {
  if (!_rtc)
    return;

  _rtc->setMinutes(_rtc->getMinutes() + 1);
}

void CPresenter::SetFBSettings(CFBSettings_uptr fb_settings) {
  _fb_settings = std::move(fb_settings);
}

String CPresenter::GetFBTime() {
  if (!_fb_settings)
    return "";

  return FmtTimeGroup(_fb_settings->FBHour()) + ":" + FmtTimeGroup(0);
}
void CPresenter::IncFBTime() {
  if (!_fb_settings)
    return;

  _fb_settings->SetFBHour(_fb_settings->FBHour() + 1);
}
void CPresenter::DecFBTime() {
  if (!_fb_settings)
    return;

  _fb_settings->SetFBHour(_fb_settings->FBHour() - 1);
}

String CPresenter::GetFBHumidityV() {
  if (!_fb_settings)
    return "";

  return std::to_string(_fb_settings->HumidityV());
}
void CPresenter::IncFBHumidityV() {
  if (!_fb_settings)
    return;
  _fb_settings->SetHumidityV(_fb_settings->HumidityV() + 0.05);
}
void CPresenter::DecFBHumidityV() {
  if (!_fb_settings)
    return;

  _fb_settings->SetHumidityV(_fb_settings->HumidityV() - 0.05);
}