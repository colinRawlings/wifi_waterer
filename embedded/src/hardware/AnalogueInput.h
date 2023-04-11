#pragma once

#include "../Types.h"

#include "../Updateable.h"

#include <memory>

class CAnalogueInput;
using CAnalogueInput_ptr = std::shared_ptr<CAnalogueInput>;

class CAnalogueInput : public CUpdateable
{
  public:
    static CAnalogueInput_ptr Create(byte pin, bool pull_up,
                                     float averaging_time_constant_samples = 10);

    /// Returns the exponentially averaged voltage
    float GetVoltage();

    void SetPullUp();

    void Update() override;

  private:
    CAnalogueInput(byte pin, bool pull_up,
                   float averaging_time_constant_samples);

    float _GetVoltage();

    const float kStepResolution;

    byte m_Pin;
    bool m_PullUp;

    //
    bool m_FirstSample;
    float m_AveragedVoltage;
    float m_AveragingTimeConstant_samples;
};
