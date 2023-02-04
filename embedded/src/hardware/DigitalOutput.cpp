#include "DigitalOutput.h"

namespace {
bool _xor(bool A, bool B) { return !A != !B; }
} // namespace

CDigitalOutput::CDigitalOutput(byte pin, bool inverted_output)
    : _pin(pin)
    , _inverted_output(inverted_output)
    , _output_state(OutputStates::OFF)
    , _status(Statuses::READY)
{
    pinMode(pin, OUTPUT);
    SetState(OutputStates::OFF);

    _time_activated_ms = millis();
    _activation_duration_ms = 0;
}

void CDigitalOutput::SetState(OutputStates new_state)
{
    // Cancel Any timed activation

    _status = Statuses::READY;

    _output_state = new_state;

    auto pin_state =
        bool{_xor(static_cast<bool>(new_state), _inverted_output)};
    digitalWrite(_pin, pin_state);
}

OutputStates CDigitalOutput::GetOutputState() { return _output_state; }

void CDigitalOutput::TurnOn()
{
    SetState(OutputStates::ON);
    _status = Statuses::READY;
}
void CDigitalOutput::TurnOff()
{
    SetState(OutputStates::OFF);
    _status = Statuses::READY;
}

void CDigitalOutput::TurnOnFor(long activation_duration_ms)
{
    _time_activated_ms = millis();
    _activation_duration_ms = activation_duration_ms;

    SetState(OutputStates::ON);
    _status = Statuses::RUNNING;
}

long CDigitalOutput::RemainingOnTime()
{
    if (_status != Statuses::RUNNING)
    {
        return 0;
    }

    return _activation_duration_ms - (millis() - _time_activated_ms);
}

void CDigitalOutput::Update()
{
    if (_status != Statuses::RUNNING)
    {
        return;
    }

    if ((millis() - _time_activated_ms) > _activation_duration_ms)
    {
        SetState(OutputStates::OFF);
        _status = Statuses::READY;
    }
}
