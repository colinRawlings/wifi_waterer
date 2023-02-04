#pragma once

#include "../Types.h"

#include "../model/interfaces/IRealTimeClock.h"

class CRealTimeClock;
using CRealTimeClock_uptr = std::unique_ptr<CRealTimeClock>;

class CRealTimeClock : public IRealTimeClock
{

  public:
    static CRealTimeClock_uptr Create();

    byte GetHours() override;
    byte GetMinutes() override;
    byte GetSeconds() override;

    void SetHours(byte) override;
    void SetMinutes(byte) override;
    void SetSeconds(byte) override;

  private:
    CRealTimeClock();

    std::unique_ptr<RTCZero> _clock;
};
