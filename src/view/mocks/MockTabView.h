#pragma once

#include <gmock/gmock.h>

#include "../interfaces/ITabView.h"

class MockTabView : public ITabView
{
  public:
    MOCK_METHOD(void, OnNextTab, (), (override));
    MOCK_METHOD(void, OnPreviousTab, (), (override));
};
