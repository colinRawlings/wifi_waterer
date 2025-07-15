#pragma once

#include "SmartPumpPins.h"

#include "DigitalOutput.h"
#include "AnalogueInput.h"

#include "../model/interfaces/ISmartPump.h"

#include "../Updateable.h"

// Container class composing the IO for a smart pump (digital output + humidity
// sensor input)
class CSmartPump : public CUpdateable, public ISmartPump
{
  public:
    static ISmartPump_ptr Create(SSmartPumpPins pins);

    // sensor
    float GetHumidityV() override;

    // pump
    bool GetStatus() override;

    /// @brief  Reports if pump ran since last poll
    bool GetPumpRan() override;

    void TurnOff() override;
    void TurnOnForMs(long activation_duration_ms) override;
    long RemainingOnTimeMs() override;

    //
    void Update() override;
    void AddChild(IUpdateable_ptr child) override { return CUpdateable::AddChild(child); }
    void SetChildren(IUpdateable_ptr_vec children) override { return CUpdateable::SetChildren(children); }

  private:
    CSmartPump(SSmartPumpPins pins);

    float _smoothed_humidity_V;

    CDigitalOutput_ptr _pump;
    CAnalogueInput_ptr _humidity_sensor;
    bool pump_ran{};
};
