#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../src/Display.h"
#include "../mocks/MockLiquidCrystal.h"

#include <iostream>

// Demonstrate some basic assertions.
TEST(DisplaySuite, SetRow) {
  auto display = CDisplay::Create();
  auto lcd = std::make_shared<MockLiquidCrystal>();
  display->SetLcd(lcd);

  std::string message{"hello"};

  //

  EXPECT_CALL(*lcd, clear()).Times(testing::Exactly(1));
  EXPECT_CALL(*lcd, print).Times(testing::Exactly(2));

  ASSERT_TRUE(display);

  display->SetRow0(message);

  display->Print();
}