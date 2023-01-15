#include "FBSettings.h"

typedef struct
{
    bool valid;
    int fb_hour;
    float humidity_v;
    long pump_duration_ms;
} SPersistentFbSettings;

FlashStorage(flash_store_fb_settings, SPersistentFbSettings);

//

CFBSettings_uptr CFBSettings::Create()
{
    return std::unique_ptr<CFBSettings>(new CFBSettings);
}

CFBSettings::CFBSettings()
    : _fb_hour(8)
    , _humidity_v(1.6)
    , _pump_duration_ms(120000)
{
    LoadFromFlash();
}

byte CFBSettings::FBHour() { return _fb_hour; }
float CFBSettings::HumidityV() { return _humidity_v; }
long CFBSettings::PumpDurationMs() { return _pump_duration_ms; }

void CFBSettings::SetFBHour(byte hour)
{
    _fb_hour = hour;
    if (_fb_hour > 23)
        _fb_hour = 23;
}

void CFBSettings::SetHumidityV(float humidity_v)
{
    _humidity_v = humidity_v;

    if (_humidity_v > 3)
        _humidity_v = 3;

    if (_humidity_v < 0)
        _humidity_v = 0;
}

void CFBSettings::SetPumpDurationMs(long duration_ms)
{
    if (duration_ms < 10000)
        duration_ms = 10000;

    _pump_duration_ms = duration_ms;
}

void CFBSettings::SaveToFlash()
{
    SPersistentFbSettings settings{true, _fb_hour, _humidity_v, _pump_duration_ms};
    flash_store_fb_settings.write(settings);
}

bool CFBSettings::LoadFromFlash()
{
    SPersistentFbSettings settings = flash_store_fb_settings.read();
    if (settings.valid)
    {
        _fb_hour = settings.fb_hour;
        _humidity_v = settings.humidity_v;
        _pump_duration_ms = settings.pump_duration_ms;
    }
}
