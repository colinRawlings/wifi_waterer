#pragma once

#include <memory>

class ISmartPump;
using ISmartPump_uptr = std::unique_ptr<ISmartPump>;

enum class OutputStates : byte {
    OFF = false,
    ON = true,
};

enum class Statuses : int { NOT_READY = 0,
                            READY,
                            RUNNING };

class ISmartPump
{
  public:
    // sensor
    virtual float GetHumidityV() = 0;

    // pump
    virtual bool GetStatus() = 0;
    virtual void TurnOff() = 0;
    virtual void TurnOnFor(long activation_duration_ms) = 0;

    //
    virtual void Update() = 0;
};
