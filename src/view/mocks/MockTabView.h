#pragma once

#include <gmock/gmock.h>

#include "../interfaces/ITabView.h"

class MockTabView : public ITabView {
public:
  MOCK_METHOD(void, NextTab, (), (override));
  MOCK_METHOD(void, PreviousTab, (), (override));
};