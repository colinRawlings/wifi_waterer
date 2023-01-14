#pragma once

#include <gmock/gmock.h>

#include "../../Types.h"

#include "../interfaces/IRealTimeClock.h"

class MockRealTimeClock : public IRealTimeClock
{
  public:
    MOCK_METHOD(byte, GetHours, (), (override));
    MOCK_METHOD(byte, GetMinutes, (), (override));
    MOCK_METHOD(byte, GetSeconds, (), (override));

    MOCK_METHOD(void, SetHours, (byte), (override));
    MOCK_METHOD(void, SetMinutes, (byte), (override));
    MOCK_METHOD(void, SetSeconds, (byte), (override));
};
