#pragma once

#include <gmock/gmock.h>

#include "../../Types.h"

#include "../interfaces/IRealTimeClock.h"

class MockRealTimeClock : public IRealTimeClock {
public:
  MOCK_METHOD(byte, getHours, (), (override));
  MOCK_METHOD(byte, getMinutes, (), (override));
  MOCK_METHOD(byte, getSeconds, (), (override));

  MOCK_METHOD(void, setHours, (byte), (override));
  MOCK_METHOD(void, setMinutes, (byte), (override));
  MOCK_METHOD(void, setSeconds, (byte), (override));
};