#pragma once

#include <gmock/gmock.h>

#include "../interfaces/IPushSwitch.h"

class MockPushSwitch : public IPushSwitch {
public:
  MOCK_METHOD(bool, GetState, (), (override));
};