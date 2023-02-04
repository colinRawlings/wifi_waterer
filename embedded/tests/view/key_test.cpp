#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../src/view/Key.h"
#include "../../src/view/mocks/MockPushSwitch.h"

// Check row cache.
TEST(KeySuite, KeyPress)
{
    auto key = CKey::Create("dummy");
    auto push_switch = std::make_unique<MockPushSwitch>();

    EXPECT_CALL(*push_switch, GetState)
        .Times(4)
        .WillOnce(testing::Return(PushSwitchState::UNPRESSED)) // on set
        .WillOnce(testing::Return(PushSwitchState::PRESSED))   // first call (press)
        .WillOnce(testing::Return(PushSwitchState::UNPRESSED)) // second call (not pressed)
        .WillOnce(testing::Return(PushSwitchState::UNPRESSED)) // third call (not pressed)
        ;

    testing::Mock::AllowLeak(push_switch.get());

    //

    key->SetPushSwitch(std::move(push_switch));

    ASSERT_TRUE(key->KeyPressed());
    ASSERT_FALSE(key->KeyPressed());
    ASSERT_FALSE(key->KeyPressed());
}
