#include "AnalogueInput.h"

CAnalogueInput_ptr CAnalogueInput::Create(byte pin,
                                          bool pull_up,
                                          float averaging_time_constant_samples /*= 10*/)
{
    return CAnalogueInput_ptr(new CAnalogueInput(pin, pull_up, averaging_time_constant_samples));
}

CAnalogueInput::CAnalogueInput(byte pin, bool pull_up,
                               float averaging_time_constant_samples)
    : m_Pin(pin)
    , m_PullUp(false)
    , m_AveragingTimeConstant_samples(averaging_time_constant_samples)
    , m_AveragedVoltage(0)
    , m_FirstSample(true)
    , kStepResolution(5.0f / 1023)
{
    pinMode(m_Pin, INPUT);

    if (pull_up)
        SetPullUp();
}

void CAnalogueInput::SetPullUp() { digitalWrite(m_Pin, INPUT_PULLUP); }

void CAnalogueInput::Update()
{
    if (m_AveragingTimeConstant_samples <= 1.0f || m_FirstSample)
    {
        m_AveragedVoltage = _GetVoltage();
        m_FirstSample = false;
        return;
    }

    float alpha = 1.0f / m_AveragingTimeConstant_samples;
    m_AveragedVoltage = alpha * _GetVoltage() + (1 - alpha) * m_AveragedVoltage;
}

float CAnalogueInput::_GetVoltage()
{
    return kStepResolution * analogRead(m_Pin);
}

float CAnalogueInput::GetVoltage()
{
    if (m_FirstSample)
    {
        Update();
    }

    return m_AveragedVoltage;
}
