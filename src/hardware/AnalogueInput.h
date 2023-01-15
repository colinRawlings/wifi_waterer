#pragma once

#include "../Types.h"

class CAnalogueInput
{
  public:
    CAnalogueInput(byte pin, bool pull_up,
                   float averaging_time_constant_samples = 10);

    /// Returns the exponentially averaged voltage
    float GetVoltage();

    void SetPullUp();

    void Update();

  private:
    float _GetVoltage();

    const float kStepResolution;

    byte m_Pin;
    bool m_PullUp;

    //
    bool m_FirstSample;
    float m_AveragedVoltage;
    float m_AveragingTimeConstant_samples;
};
