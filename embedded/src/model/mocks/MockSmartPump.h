#pragma once

#include <gmock/gmock.h>

#include "../../Types.h"

#include "../interfaces/ISmartPump.h"

class MockSmartPump : public ISmartPump
{
  public:
    MOCK_METHOD(float, GetHumidityV, (), (override));

    MOCK_METHOD(bool, GetStatus, (), (override));
    MOCK_METHOD(void, TurnOff, (), (override));
    MOCK_METHOD(void, TurnOnForMs, (long), (override));
    MOCK_METHOD(long, RemainingOnTimeMs, (), (override));

    MOCK_METHOD(void, Update, (), (override));
};