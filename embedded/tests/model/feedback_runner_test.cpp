#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../src/model/mocks/MockRealTimeClock.h"
#include "../../src/model/mocks/MockSmartPump.h"

#include "../../src/model/FBSettings.h"

#include "../../src/model/FBRunner.h"

TEST(ModelSuite, FBRunnerTest_NoFB)
{
    auto fb_settings = CFBSettings::Create();
    fb_settings->SetFBHour(14);

    auto clock = std::make_shared<MockRealTimeClock>();
    EXPECT_CALL(*clock, GetHours).Times(2).WillRepeatedly(testing::Return(11));

    auto pump = std::make_shared<MockSmartPump>();
    EXPECT_CALL(*pump, TurnOnForMs).Times(0);

    auto runner = CFBRunner::Create(clock, pump, fb_settings);

    runner->Update(); // second call to fb should not trigger
}

TEST(ModelSuite, FBRunnerTest_RunFB)
{
    auto fb_settings = CFBSettings::Create();
    fb_settings->SetFBHour(14);
    fb_settings->SetHumidityV(2.0f);

    auto clock = std::make_shared<MockRealTimeClock>();
    EXPECT_CALL(*clock, GetHours)
        .Times(3)
        .WillOnce(testing::Return(13))
        .WillOnce(testing::Return(14))
        .WillOnce(testing::Return(14))
        ;

    auto pump = std::make_shared<MockSmartPump>();
    EXPECT_CALL(*pump, GetHumidityV).Times(1).WillOnce(testing::Return(2.2f));
    EXPECT_CALL(*pump, TurnOnForMs).Times(1);

    auto runner = CFBRunner::Create(clock, pump, fb_settings);

    runner->Update(); // second call to fb should trigger
    runner->Update(); // third call to fb should not trigger

}

TEST(ModelSuite, FBRunnerTest_TooWet)
{
    auto fb_settings = CFBSettings::Create();
    fb_settings->SetFBHour(14);
    fb_settings->SetHumidityV(2.0f);

    auto clock = std::make_shared<MockRealTimeClock>();
    EXPECT_CALL(*clock, GetHours)
        .Times(2)
        .WillOnce(testing::Return(13))
        .WillOnce(testing::Return(14))
        ;

    auto pump = std::make_shared<MockSmartPump>();
    EXPECT_CALL(*pump, GetHumidityV).Times(1).WillOnce(testing::Return(1.8f));
    EXPECT_CALL(*pump, TurnOnForMs).Times(0);

    auto runner = CFBRunner::Create(clock, pump, fb_settings);

    runner->Update(); // second call to fb should trigger
}
