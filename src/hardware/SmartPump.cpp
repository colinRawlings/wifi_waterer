#include "SmartPump.h"

ISmartPump_uptr CSmartPump::Create(byte pump_pin, byte sensor_pin)
{
    return std::unique_ptr<CSmartPump>(new CSmartPump(pump_pin, sensor_pin));
}

CSmartPump::CSmartPump(byte pump_pin, byte sensor_pin)
    : _pump(pump_pin, false)
    , _humidity_sensor(sensor_pin, false)
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

void CSmartPump::Update()
{
    _pump.Update();
    _humidity_sensor.Update();
}
