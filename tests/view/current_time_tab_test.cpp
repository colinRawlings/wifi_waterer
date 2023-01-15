#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../src/model/mocks/MockRealTimeClock.h"

#include "../../src/presenter/Presenter.h"

#include "../../src/view/Display.h"
#include "../../src/view/mocks/MockLiquidCrystal.h"

#include "../../src/view/Key.h"
#include "../../src/view/mocks/MockPushSwitch.h"

#include "../../src/view/CurrentTimeTab.h"
#include "../../src/view/DisplayKeys.h"

#include "../../src/view/mocks/MockTabView.h"

TEST(TabSuite, NextTabTest)
{
    auto display = CDisplay::Create();
    auto lcd = std::make_unique<MockLiquidCrystal>();

    //

    auto rtc = std::make_unique<MockRealTimeClock>();
    EXPECT_CALL(*rtc, GetHours).Times(1).WillOnce(testing::Return(11));
    EXPECT_CALL(*rtc, GetMinutes).Times(1).WillOnce(testing::Return(42));
    testing::Mock::AllowLeak(rtc.get());

    auto presenter = CPresenter::Create();
    presenter->SetRealTimeClock(std::move(rtc));

    //

    auto tab_view = std::make_shared<MockTabView>();
    EXPECT_CALL(*tab_view, OnNextTab).Times(1);

    //

    auto next_tab_key = CKey::Create("next_tab");
    auto next_tab_push_switch = std::make_unique<MockPushSwitch>();

    EXPECT_CALL(*next_tab_push_switch, GetState)
        .Times(2)
        .WillOnce(testing::Return(PushSwitchState::UNPRESSED)) // on set
        .WillOnce(testing::Return(PushSwitchState::PRESSED))   // first call (press)
        ;

    testing::Mock::AllowLeak(next_tab_push_switch.get());

    next_tab_key->SetPushSwitch(std::move(next_tab_push_switch));

    //

    auto keys = CDisplayKeys::Create(CKey_ptr(), next_tab_key, CKey_ptr(), CKey_ptr());

    //

    auto current_time_tab =
        CCurrentTimeTab::Create(presenter, tab_view, keys, display);

    current_time_tab->Update();

    display->Print();
}

TEST(TabSuite, IncHourTest)
{
    auto display = CDisplay::Create();
    auto lcd = std::make_unique<MockLiquidCrystal>();

    //

    auto rtc = std::make_unique<MockRealTimeClock>();
    EXPECT_CALL(*rtc, GetHours).Times(2).WillRepeatedly(testing::Return(11));
    EXPECT_CALL(*rtc, GetMinutes).Times(1).WillOnce(testing::Return(42));
    EXPECT_CALL(*rtc, SetHours(12)).Times(1);
    testing::Mock::AllowLeak(rtc.get());

    auto presenter = CPresenter::Create();
    presenter->SetRealTimeClock(std::move(rtc));

    //

    auto tab_view = std::make_shared<MockTabView>();

    //

    auto left_func_key = CKey::Create("left_func");
    auto left_func_push_switch = std::make_unique<MockPushSwitch>();

    EXPECT_CALL(*left_func_push_switch, GetState)
        .Times(2)
        .WillOnce(testing::Return(PushSwitchState::UNPRESSED)) // on set
        .WillOnce(testing::Return(PushSwitchState::PRESSED))   // first call (press)
        ;

    testing::Mock::AllowLeak(left_func_push_switch.get());

    left_func_key->SetPushSwitch(std::move(left_func_push_switch));

    //

    auto keys = CDisplayKeys::Create(CKey_ptr(), CKey_ptr(), left_func_key, CKey_ptr());

    //

    auto current_time_tab =
        CCurrentTimeTab::Create(presenter, tab_view, keys, display);

    current_time_tab->Update();

    display->Print();
}
