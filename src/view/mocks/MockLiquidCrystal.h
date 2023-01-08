#pragma once

#include <gmock/gmock.h>

#include "../interfaces/ILiquidCrystal.h"

class MockLiquidCrystal : public ILiquidCrystal {
public:
  MOCK_METHOD(void, print, (String msg), (override));
  MOCK_METHOD(void, clear, (), (override));
  MOCK_METHOD(void, setCursor, (int column, int row), (override));
};