#pragma once

#include "../Types.h"

#include "Presenter.h"

#include "../hardware/RealTimeClock.h"
#include "../model/FBSettings.h"

CPresenter_ptr CreatePresenter()
{
    auto presenter = CPresenter::Create();

    auto rtc = CRealTimeClock::Create();
    presenter->SetRealTimeClock(std::move(rtc));

    auto fb_settings = CFBSettings::Create();
    presenter->SetFBSettings(std::move(fb_settings));

    return presenter;
}
