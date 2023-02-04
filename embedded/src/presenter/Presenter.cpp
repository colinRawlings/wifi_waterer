#include "Presenter.h"

#include "../view/Helpers.h"

#include "../model/FBSettings.h"

#include <string>

std::string FmtTimeGroup(byte value)
{
    std::string str;
    if (value < 10)
        str += "0";

    str += std::to_string(value);

    return str;
}

CPresenter_ptr CPresenter::Create()
{
    auto destroy = [](CPresenter * display) { delete display; };
    return CPresenter_ptr(new CPresenter, destroy);
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

// FB Settings

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

std::string CPresenter::GetFBHour()
{
    if (!_fb_settings)
        return "";

    return std::to_string(_fb_settings->FBHour());
}

void CPresenter::SetFBTime(int hour)
{
    if (!_fb_settings)
        return;

    if (hour < 0 || hour > 23)
        return;

    _fb_settings->SetFBHour(hour);
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

std::string CPresenter::GetFBHumidityV(bool add_unit)
{
    if (!_fb_settings)
        return "<err>";

    return FormatHumidityV(_fb_settings->HumidityV(), add_unit);
}

void CPresenter::SetFBHumidityV(float value)
{
    if (!_fb_settings)
        return;

    if (value < 0 || value > 3.3)
        return;

    _fb_settings->SetHumidityV(value);
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

std::string CPresenter::GetFBPumpDurationS(bool add_unit)
{
    if (!_fb_settings)
        return "<err>";

    if (add_unit)
        return std::to_string(_fb_settings->PumpDurationMs() / 1000) + "s";

    return std::to_string(_fb_settings->PumpDurationMs() / 1000);
}

long CPresenter::GetFBPumpDurationMs()
{
    if (!_fb_settings)
        return 0;

    return _fb_settings->PumpDurationMs();
}

void CPresenter::SetFBPumpDurationMs(long duration_ms)
{
    if (!_fb_settings)
        return;

    return _fb_settings->SetPumpDurationMs(duration_ms);
}

void CPresenter::IncFBPumpDurationMs()
{
    if (!_fb_settings)
        return;
    _fb_settings->SetPumpDurationMs(_fb_settings->PumpDurationMs() + 10000);
}
void CPresenter::DecFBPumpDurationMs()
{
    if (!_fb_settings)
        return;

    _fb_settings->SetPumpDurationMs(_fb_settings->PumpDurationMs() - 10000);
}

void CPresenter::LoadFBSettingsFromFlash()
{
    if (!_fb_settings)
        return;

    _fb_settings->LoadFromFlash();
};

void CPresenter::SaveFBSettingsToFlash()
{
    if (!_fb_settings)
        return;

    _fb_settings->SaveToFlash();
}

// Smart Pump

void CPresenter::SetSmartPump(ISmartPump_uptr pump)
{
    _pump = std::move(pump);
}

bool CPresenter::GetPumpStatus()
{
    if (!_pump)
        return false;

    return _pump->GetStatus();
}

std::string CPresenter::GetHumidityV(bool add_unit)
{
    if (!_pump)
        return "err";

    return FormatHumidityV(_pump->GetHumidityV(), add_unit);
}

void CPresenter::TurnOnPumpFor(long duration_ms)
{
    if (!_pump)
        return;

    _pump->TurnOnFor(duration_ms);
}

std::string CPresenter::RemainingPumpOnTimeS()
{
    if (!_pump)
        return "err";

    return std::to_string(_pump->RemainingOnTimeMs() / 1000) + "s";
}

void CPresenter::TurnOffPump()
{
    if (!_pump)
        return;

    _pump->TurnOff();
}

//

void CPresenter::Update()
{
    CUpdateable::Update();

    if (_pump)
    {
        _pump->Update();
    }
}
