#pragma once

#include "../Types.h"

#include "Presenter.h"

#include "../hardware/SmartPumpPins.h"

#include "../hardware/RealTimeClock.h"
#include "../hardware/SmartPump.h"
#include "../model/FBSettings.h"

CPresenter_ptr CreatePresenter(SSmartPumpPins smart_pump_pins)
{
    auto presenter = CPresenter::Create();

    auto rtc = CRealTimeClock::Create();
    presenter->SetRealTimeClock(std::move(rtc));

    auto fb_settings = CFBSettings::Create();
    presenter->SetFBSettings(std::move(fb_settings));

    auto pump = CSmartPump::Create(smart_pump_pins);
    presenter->SetSmartPump(std::move(pump));

    return presenter;
}
