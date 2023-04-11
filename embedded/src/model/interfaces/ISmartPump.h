#pragma once

#include "../../IUpdateable.h"

#include <memory>

class ISmartPump;
using ISmartPump_ptr = std::shared_ptr<ISmartPump>;

enum class OutputStates : byte {
    OFF = false,
    ON = true,
};

enum class Statuses : int { NOT_READY = 0,
                            READY,
                            RUNNING };

class ISmartPump : public IUpdateable
{
  public:
    // sensor
    virtual float GetHumidityV() = 0;

    // pump
    virtual bool GetStatus() = 0;
    virtual void TurnOff() = 0;
    virtual void TurnOnForMs(long activation_duration_ms) = 0;
    virtual long RemainingOnTimeMs() = 0;

    //

    virtual ~ISmartPump() = default;
};
