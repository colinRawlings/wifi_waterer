#pragma once

#include "../Types.h"

#include "../model/interfaces/IRealTimeClock.h"

class CRealTimeClock;

class CRealTimeClock : public IRealTimeClock
{

  public:
    static IRealTimeClock_ptr Create();

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
