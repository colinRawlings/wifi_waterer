#include "FBRunner.h"

#include "FBSettings.h"

CFBRunner_ptr CFBRunner::Create(IRealTimeClock_ptr clock,
                                ISmartPump_ptr pump,
                                CFBSettings_ptr settings)
{
    return CFBRunner_ptr(new CFBRunner(clock, pump, settings));
}

CFBRunner::CFBRunner(IRealTimeClock_ptr clock,
                     ISmartPump_ptr pump,
                     CFBSettings_ptr settings)
    : _clock(clock)
    , _pump(pump)
    , _settings(settings)
{
    if (clock)
        _last_update_hour = _clock->GetHours();
}

void CFBRunner::Update()
{
    CUpdateable::Update();

    if (!_clock || !_pump || !_settings)
        return;

    auto fb_hour = _settings->FBHour();
    auto this_hour = _clock->GetHours();

    if (this_hour == fb_hour && _last_update_hour != fb_hour)
    {
        if (_pump->GetHumidityV() > _settings->HumidityV()) // nb higher is drier
            _pump->TurnOnForMs(_settings->PumpDurationMs());
    }

    _last_update_hour = this_hour;
}
