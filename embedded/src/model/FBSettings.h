#pragma once

#include "../Types.h"

#include <memory>

//

class CFBSettings;
using CFBSettings_uptr = std::unique_ptr<CFBSettings>;

class CFBSettings
{
  public:
    static CFBSettings_uptr Create();

    byte FBHour();
    float HumidityV();
    long PumpDurationMs();

    void SetFBHour(byte);
    void SetHumidityV(float);
    void SetPumpDurationMs(long);

    void SaveToFlash();
    bool LoadFromFlash();

  protected:
    CFBSettings();

    byte _fb_hour;
    float _humidity_v;
    long _pump_duration_ms;
};
