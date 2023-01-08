#pragma once

#include "../Types.h"

#include "../model/interfaces/IRealTimeClock.h"

//

class CFBSettings;
using CFBSettings_uptr = std::unique_ptr<CFBSettings>;

//

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CPresenter {
public:
  static CPresenter_ptr Create();

  // Current Time
  void SetRealTimeClock(IRealTimeClock_uptr rtc);

  String GetCurrentTime();
  void IncCurrentTimeHour();
  void IncCurrentTimeMinute();

  // FB Settings
  void SetFBSettings(CFBSettings_uptr fb_settings);

  String GetFBTime();
  void IncFBTime();
  void DecFBTime();

  String GetFBHumidityV();
  void IncFBHumidityV();
  void DecFBHumidityV();

protected:
  CPresenter() = default;

  IRealTimeClock_uptr _rtc;
  CFBSettings_uptr _fb_settings;
};