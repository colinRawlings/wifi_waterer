#include "FBSettings.h"

CFBSettings_uptr CFBSettings::Create()
{
    return std::unique_ptr<CFBSettings>(new CFBSettings);
}

CFBSettings::CFBSettings()
    : _fb_hour(8)
    , _humidity_v(1.6)
{}

byte CFBSettings::FBHour() { return _fb_hour; }
float CFBSettings::HumidityV() { return _humidity_v; }

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
