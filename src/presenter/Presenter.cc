#include "Presenter.h"

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

  auto fmt_group = [](byte value) -> String {
    String str;
    if (value < 10)
      str += "0";

    str += std::to_string(value);

    return str;
  };

  return fmt_group(_rtc->getHours()) + ":" + fmt_group(_rtc->getMinutes());
}

void CPresenter::IncCurrentTimeHour() {
  if (!_rtc)
    return;

  _rtc->setHours(_rtc->getHours() + 1);
}