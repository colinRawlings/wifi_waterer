#include "RealTimeClock.h"

CRealTimeClock_uptr CRealTimeClock::Create()
{
    return CRealTimeClock_uptr(new CRealTimeClock);
}

byte CRealTimeClock::GetHours()
{
    if (!_clock)
        return 0;

    return _clock->getHours();
}
byte CRealTimeClock::GetMinutes()
{
    if (!_clock)
        return 0;

    return _clock->getMinutes();
}
byte CRealTimeClock::GetSeconds()
{
    if (!_clock)
        return 0;

    return _clock->getSeconds();
}

void CRealTimeClock::SetHours(byte val)
{
    if (!_clock)
        return;

    _clock->setHours(val);
}
void CRealTimeClock::SetMinutes(byte val)
{
    if (!_clock)
        return;

    _clock->setMinutes(val);
}
void CRealTimeClock::SetSeconds(byte val)
{
    if (!_clock)
        return;

    _clock->setSeconds(val);
}

CRealTimeClock::CRealTimeClock()
{
    _clock = std::unique_ptr<RTCZero>(new RTCZero);
    _clock->begin();

    SetHours(12);
    SetMinutes(0);
    SetSeconds(0);
}