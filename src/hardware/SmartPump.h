#pragma once

#include "DigitalOutput.h"
#include "AnalogueInput.h"

#include "../model/interfaces/ISmartPump.h"

// Container class composing the IO for a smart pump (digital output + humidity
// sensor input)
class CSmartPump : public ISmartPump
{
  public:
    static ISmartPump_uptr Create(byte pump_pin, byte sensor_pin);

    // sensor
    float GetHumidityV() override;

    // pump
    bool GetStatus() override;
    void TurnOff() override;
    void TurnOnFor(long activation_duration_ms) override;

    //
    void Update() override;

  private:
    CSmartPump(byte pump_pin, byte sensor_pin);

    CDigitalOutput _pump;
    CAnalogueInput _humidity_sensor;
};
