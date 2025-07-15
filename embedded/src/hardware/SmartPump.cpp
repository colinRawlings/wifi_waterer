#include "SmartPump.h"

static const int kSmoothedSamples{500};
constexpr float kSmoothingFactor = 1.0f / kSmoothedSamples;

ISmartPump_ptr CSmartPump::Create(SSmartPumpPins pins)
{
    return ISmartPump_ptr(new CSmartPump(pins));
}

CSmartPump::CSmartPump(SSmartPumpPins pins)
    : _pump(CDigitalOutput::Create(pins.pump_pin, false))
    , _humidity_sensor(CAnalogueInput::Create(pins.sensor_pin, false))
{
    _smoothed_humidity_V = _humidity_sensor->GetVoltage();

    SetChildren({_pump, _humidity_sensor});
}

float CSmartPump::GetHumidityV()
{
    return _smoothed_humidity_V;
}

// pump
bool CSmartPump::GetStatus()
{
    return _pump->GetOutputState() == OutputStates::ON;
}

bool CSmartPump::GetPumpRan()
{
    if (pump_ran)
    {
        pump_ran = false;
        return true;
    }

    return false;
}

void CSmartPump::TurnOff()
{
    _pump->TurnOff();
}

void CSmartPump::TurnOnForMs(long activation_duration_ms)
{
    pump_ran = true;
    _pump->TurnOnFor(activation_duration_ms);
}

long CSmartPump::RemainingOnTimeMs()
{
    return _pump->RemainingOnTime();
}

void CSmartPump::Update()
{
    CUpdateable::Update();

    _smoothed_humidity_V = kSmoothingFactor * _humidity_sensor->GetVoltage() + (1 - kSmoothingFactor) * _smoothed_humidity_V;
}
