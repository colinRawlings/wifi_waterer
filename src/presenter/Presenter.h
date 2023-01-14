#pragma once

#include "../Types.h"

#include "../model/interfaces/IRealTimeClock.h"

#include <string>

//

class CFBSettings;
using CFBSettings_uptr = std::unique_ptr<CFBSettings>;

//

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CPresenter
{
  public:
    static CPresenter_ptr Create(bool create_children); // remove once pump is implemented

    // Current Time
    void SetRealTimeClock(IRealTimeClock_uptr rtc);

    std::string GetCurrentTime();
    void IncCurrentTimeHour();
    void IncCurrentTimeMinute();

    // FB Settings
    void SetFBSettings(CFBSettings_uptr fb_settings);

    std::string GetFBTime();
    void IncFBTime();
    void DecFBTime();

    std::string GetFBHumidityV();
    void IncFBHumidityV();
    void DecFBHumidityV();

  protected:
    CPresenter();

    IRealTimeClock_uptr _rtc;
    CFBSettings_uptr _fb_settings;
};