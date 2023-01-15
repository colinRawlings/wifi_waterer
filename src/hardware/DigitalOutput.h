#pragma once

#include "../Types.h"

#include "../model/interfaces/ISmartPump.h"

class CDigitalOutput
{
  public:
    CDigitalOutput(byte pin, bool inverted_output);

    // Turn on the pin - this cancels any timed activation
    void TurnOn();

    // Turn off the pin - this cancels any timed activation
    void TurnOff();

    // Enter a timed activation of the pin (us) - requires polling of the event
    // loop via Update
    void TurnOnFor(long activation_duration_ms);

    // Get the status
    Statuses GetStatus();

    // Get the current pin state
    OutputStates GetOutputState();

    //
    void Update();

  private:
    void SetState(OutputStates new_state);

  private:
    byte m_Pin;
    bool m_InvertedOutput;

    long m_TimeActivated_ms;
    long m_ActivationDuration_ms;

    OutputStates m_OutputState;
    Statuses m_Status;
};
