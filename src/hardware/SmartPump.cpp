#include "SmartPump.h"

static const int kSmoothedSamples{500};
constexpr float kSmoothingFactor = 1.0f / kSmoothedSamples;

ISmartPump_uptr CSmartPump::Create(SSmartPumpPins pins)
{
    return std::unique_ptr<CSmartPump>(new CSmartPump(pins));
}

CSmartPump::CSmartPump(SSmartPumpPins pins)
    : _pump(pins.pump_pin, false)
    , _humidity_sensor(pins.sensor_pin, false)
{
    _smoothed_humidity_V = _humidity_sensor.GetVoltage();
}

float CSmartPump::GetHumidityV()
{
    return _smoothed_humidity_V;
}

// pump
bool CSmartPump::GetStatus()
{
    return _pump.GetOutputState() == OutputStates::ON;
}

void CSmartPump::TurnOff()
{
    _pump.TurnOff();
}

void CSmartPump::TurnOnFor(long activation_duration_ms)
{
    _pump.TurnOnFor(activation_duration_ms);
}

long CSmartPump::RemainingOnTimeMs()
{
    return _pump.RemainingOnTime();
}

void CSmartPump::Update()
{
    _pump.Update();
    _humidity_sensor.Update();

    _smoothed_humidity_V = kSmoothingFactor * _humidity_sensor.GetVoltage() + (1 - kSmoothingFactor) * _smoothed_humidity_V;
}
