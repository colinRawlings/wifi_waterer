#pragma once

#include "../Types.h"

#include "Presenter.h"

#include "../hardware/SmartPumpPins.h"

#include "../hardware/RealTimeClock.h"
#include "../hardware/SmartPump.h"
#include "../model/FBRunner.h"

CPresenter_ptr CreatePresenter(SSmartPumpPins smart_pump_pins)
{
    auto presenter = CPresenter::Create();

    auto clock = CRealTimeClock::Create();
    presenter->SetRealTimeClock(clock);

    auto fb_settings = CFBSettings::Create();
    presenter->SetFBSettings(fb_settings);

    auto pump = CSmartPump::Create(smart_pump_pins);
    presenter->SetSmartPump(pump);

    auto fb_runner = CFBRunner::Create(clock, pump, fb_settings);

    return presenter;
}
