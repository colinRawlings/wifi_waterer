#pragma once

#include "../Types.h"
#include "../Updateable.h"

#include "../model/interfaces/IRealTimeClock.h"
#include "../model/interfaces/ISmartPump.h"

#include <string>

//

class CFBSettings;
using CFBSettings_ptr = std::shared_ptr<CFBSettings>;

//

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CPresenter : public CUpdateable
{
  public:
    static CPresenter_ptr Create();

    // Current Time
    void SetRealTimeClock(IRealTimeClock_ptr rtc);

    std::string GetCurrentTime();

    void SetCurrentTimeHour(int hour);
    void SetCurrentTimeMinute(int minute);

    void IncCurrentTimeHour();
    void IncCurrentTimeMinute();

    // FB Settings
    void SetFBSettings(CFBSettings_ptr fb_settings);

    std::string GetFBTime();
    std::string GetFBHour();
    void SetFBTime(int hour);
    void IncFBTime();
    void DecFBTime();

    std::string GetFBHumidityV(bool add_unit = true);
    void SetFBHumidityV(float value);
    void IncFBHumidityV();
    void DecFBHumidityV();

    std::string GetFBPumpDurationS(bool add_unit = true);
    long GetFBPumpDurationMs();
    void SetFBPumpDurationMs(long duration_ms);
    void IncFBPumpDurationMs();
    void DecFBPumpDurationMs();

    void LoadFBSettingsFromFlash();
    void SaveFBSettingsToFlash();

    // Smart Pump
    void SetSmartPump(ISmartPump_ptr pump);

    bool GetPumpStatus();
    std::string GetHumidityV(bool add_unit = true);
    void TurnOnPumpForMs(long duration_ms);
    std::string RemainingPumpOnTimeS();
    void TurnOffPump();

    //
    void Update() override;

  protected:
    CPresenter();
    virtual ~CPresenter() = default;

    IRealTimeClock_ptr _rtc;
    CFBSettings_ptr _fb_settings;
    ISmartPump_ptr _pump;
};
