#include "Arduino.h"

#include "Presenter.h"

#include "../model/FBSettings.h"
#include "../hardware/RealTimeClock.h"

#include <string>

std::string FmtTimeGroup(byte value)
{
    std::string str;
    if (value < 10)
        str += "0";

    str += std::to_string(value);

    return str;
}

CPresenter_ptr CPresenter::Create(bool create_children)
{
    auto destroy = [](CPresenter * display) { delete display; };
    auto presenter = CPresenter_ptr(new CPresenter, destroy);

    if (!create_children)
        return presenter;

    auto rtc = CRealTimeClock::Create();
    presenter->SetRealTimeClock(std::move(rtc));

    auto fb_settings = CFBSettings::Create();
    presenter->SetFBSettings(std::move(fb_settings));

    return presenter;
}

CPresenter::CPresenter() {}

// Current Time
void CPresenter::SetRealTimeClock(IRealTimeClock_uptr rtc)
{
    _rtc = std::move(rtc);
}

std::string CPresenter::GetCurrentTime()
{
    if (!_rtc)
        return "";

    return FmtTimeGroup(_rtc->GetHours()) + ":" +
           FmtTimeGroup(_rtc->GetMinutes());
}

void CPresenter::IncCurrentTimeHour()
{
    if (!_rtc)
        return;

    _rtc->SetHours(_rtc->GetHours() + 1);
}

void CPresenter::IncCurrentTimeMinute()
{
    if (!_rtc)
        return;

    _rtc->SetMinutes(_rtc->GetMinutes() + 1);
}

void CPresenter::SetFBSettings(CFBSettings_uptr fb_settings)
{
    _fb_settings = std::move(fb_settings);
}

std::string CPresenter::GetFBTime()
{
    if (!_fb_settings)
        return "";

    return FmtTimeGroup(_fb_settings->FBHour()) + ":" + FmtTimeGroup(0);
}
void CPresenter::IncFBTime()
{
    if (!_fb_settings)
        return;

    _fb_settings->SetFBHour(_fb_settings->FBHour() + 1);
}
void CPresenter::DecFBTime()
{
    if (!_fb_settings)
        return;

    _fb_settings->SetFBHour(_fb_settings->FBHour() - 1);
}

std::string CPresenter::GetFBHumidityV()
{
    if (!_fb_settings)
        return "<err>";

    return std::to_string(_fb_settings->HumidityV()).substr(0, 4);
}
void CPresenter::IncFBHumidityV()
{
    if (!_fb_settings)
        return;
    _fb_settings->SetHumidityV(_fb_settings->HumidityV() + 0.05);
}
void CPresenter::DecFBHumidityV()
{
    if (!_fb_settings)
        return;

    _fb_settings->SetHumidityV(_fb_settings->HumidityV() - 0.05);
}