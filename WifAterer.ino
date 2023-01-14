#include <memory>
#include <vector>
#include <LiquidCrystal.h>

#include "src/hardware/PushSwitch.h"
#include "src/hardware/LiquidCrystalDisplay.h"
#include "src/hardware/RealTimeClock.h"

#include "src/model/FBSettings.h"

#include "src/presenter/Presenter.h"

#include "src/view/Key.h"
#include "src/view/DisplayKeys.h"
#include "src/view/Tabs.h"
#include "src/view/Display.h"

// defs
const byte pin_func_left = 3;
const byte pin_func_right = 2;

const byte pin_tab_left = 15;
const byte pin_tab_right = 14;

auto lcd_pins = SLCDPins({4, 5, 6, 7}, 9, 8);

//

CPresenter_ptr presenter;

CDisplayKeys_ptr keys;
CDisplay_ptr display;
CTabs_ptr tabs;

auto clock = CRealTimeClock::Create();

// the setup function runs once when you press reset or power the board
void setup()
{
    display = CDisplay::Create(lcd_pins);

    keys = CDisplayKeys::Create(pin_tab_left,
                                pin_tab_right,
                                pin_func_left,
                                pin_func_right);

    presenter = CPresenter::Create(true);

    tabs = CTabs::Create(presenter, keys, display);
}

// the loop function runs over and over again forever
void loop()
{

    // if (right_func->KeyPressed())
    //     ++counter;

    // display->SetRow0(std::to_string(counter + clock->GetSeconds()));
    // // display->SetRow1(std::to_string(clock->GetHours()) + ":" + std::to_string(clock->GetMinutes()) + ":" + std::to_string(clock->GetSeconds()) + ":");

    // display->SetRow1("hello");
}