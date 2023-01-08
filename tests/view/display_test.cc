#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../src/view/Display.h"
#include "../../src/view/mocks/MockLiquidCrystal.h"

TEST(DisplaySuite, SetRow_withCache) {
  auto display = CDisplay::Create();
  auto lcd = std::make_unique<MockLiquidCrystal>();

  EXPECT_CALL(*lcd, clear()).Times(testing::Exactly(1));
  EXPECT_CALL(*lcd, print).Times(testing::Exactly(2));
  EXPECT_CALL(*lcd, setCursor).Times(testing::Exactly(2));

  testing::Mock::AllowLeak(lcd.get());
  ASSERT_TRUE(display);

  //

  display->SetLcd(std::move(lcd));

  std::string message{"hello"};

  //

  display->SetRow0(message);
  display->SetRow0(message); // no extra calls

  // display->Print();
}