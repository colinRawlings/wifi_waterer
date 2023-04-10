#pragma once

#include "../Types.h"

#include "../model/interfaces/ISmartPump.h"
#include "../Updateable.h"

#include <memory>

class CDigitalOutput;
using CDigitalOutput_ptr = std::shared_ptr<CDigitalOutput>;

class CDigitalOutput : public CUpdateable
{
  public:
    static CDigitalOutput_ptr Create(byte pin, bool inverted_output);

    // Turn on the pin - this cancels any timed activation
    void
    TurnOn();

    // Turn off the pin - this cancels any timed activation
    void TurnOff();

    // Enter a timed activation of the pin (us) - requires polling of the event
    // loop via Update
    void TurnOnFor(long activation_duration_ms);

    // Returns 0 if not active
    long RemainingOnTime();

    // Get the status
    Statuses GetStatus();

    // Get the current pin state
    OutputStates GetOutputState();

    //
    void Update();

  private:
    CDigitalOutput(byte pin, bool inverted_output);

    void SetState(OutputStates new_state);

  private:
    byte _pin;
    bool _inverted_output;

    long _time_activated_ms;
    long _activation_duration_ms;

    OutputStates _output_state;
    Statuses _status;
};
