#pragma once

#include <gmock/gmock.h>

#include "../interfaces/ITabView.h"

class MockTabView : public ITabView
{
  public:
    MOCK_METHOD(bool, OnNextTab, (), (override));
    MOCK_METHOD(bool, OnPreviousTab, (), (override));
    MOCK_METHOD(bool, OnKeyPressed, (), (override));
};
