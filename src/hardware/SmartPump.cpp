#include "SmartPump.h"

ISmartPump_uptr CSmartPump::Create(SSmartPumpPins pins)
{
    return std::unique_ptr<CSmartPump>(new CSmartPump(pins));
}

CSmartPump::CSmartPump(SSmartPumpPins pins)
    : _pump(pins.pump_pin, false)
    , _humidity_sensor(pins.sensor_pin, false)
{}

float CSmartPump::GetHumidityV()
{
    return _humidity_sensor.GetVoltage();
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
}
