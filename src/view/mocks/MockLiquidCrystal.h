#pragma once

#include <gmock/gmock.h>

#include "../interfaces/ILiquidCrystal.h"

class MockLiquidCrystal : public ILiquidCrystal
{
  public:
    MOCK_METHOD(void, Print, (std::string msg), (override));
    MOCK_METHOD(void, Clear, (), (override));
    MOCK_METHOD(void, SetCursor, (int column, int row), (override));

    MOCK_METHOD(bool, Backlight, (), (override));
    MOCK_METHOD(void, SetBacklight, (bool is_on), (override));
};
