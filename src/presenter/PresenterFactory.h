#pragma once

#include "../Types.h"

#include "Presenter.h"

#include "../hardware/RealTimeClock.h"
#include "../hardware/SmartPump.h"
#include "../model/FBSettings.h"

CPresenter_ptr CreatePresenter(byte pump_pin, byte sensor_pin)
{
    auto presenter = CPresenter::Create();

    auto rtc = CRealTimeClock::Create();
    presenter->SetRealTimeClock(std::move(rtc));

    auto fb_settings = CFBSettings::Create();
    presenter->SetFBSettings(std::move(fb_settings));

    auto pump = CSmartPump::Create(pump_pin, sensor_pin);
    presenter->SetSmartPump(std::move(pump));

    return presenter;
}
