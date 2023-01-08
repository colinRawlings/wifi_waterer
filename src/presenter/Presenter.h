#pragma once

#include "../Types.h"

#include "../model/interfaces/IRealTimeClock.h"

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CPresenter {
public:
  static CPresenter_ptr Create();

  // Current Time
  void SetRealTimeClock(IRealTimeClock_uptr rtc);

  String GetCurrentTime();
  void IncCurrentTimeHour();

protected:
  CPresenter() = default;

  IRealTimeClock_uptr _rtc;
};