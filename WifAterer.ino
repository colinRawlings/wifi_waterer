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
#include "src/view/Application.h"
#include "src/view/Display.h"

// defs
const byte pin_func_left = 3;
const byte pin_func_right = 2;

const byte pin_tab_left = 15;
const byte pin_tab_right = 14;

auto lcd_pins = SLCDPins({4, 5, 6, 7}, 9, 8);

//

CApplication_ptr app;

//

void setup()
{
    auto display = CDisplay::Create(lcd_pins);

    auto keys = CDisplayKeys::Create(pin_tab_left,
                                     pin_tab_right,
                                     pin_func_left,
                                     pin_func_right);

    auto presenter = CPresenter::Create(true);

    app = CApplication::Create(presenter, keys, display);
}

void loop()
{
    app->Update();
}