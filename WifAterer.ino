
#include "src/Types.h"

#include "src/hardware/PushSwitch.h"
#include "src/hardware/LiquidCrystalDisplay.h"
#include "src/hardware/RealTimeClock.h"

#include "src/model/FBSettings.h"

#include "src/presenter/Presenter.h"

#include "src/view/Key.h"
#include "src/view/DisplayKeys.h"
#include "src/view/Application.h"
#include "src/view/DisplayFactory.h"
#include "src/view/DisplayKeysFactory.h"
#include "src/presenter/PresenterFactory.h"

#include "ConnectInfo.h"

#include <memory>
#include <vector>

// defs

int status = WL_IDLE_STATUS; // the Wi-Fi radio's status

//

const byte pin_func_left = 3;
const byte pin_func_right = 2;

const byte pin_tab_left = 15;
const byte pin_tab_right = 14;

const byte pump_pin = 16;
const byte sensor_pin = A3;

auto lcd_pins = SLCDPins({4, 5, 6, 7}, 9, 8, 18);

//

CSoftkeyDisplay_ptr softkey_view;

//

void setup()
{
    auto display = CreateDisplay(lcd_pins);

    auto keys = CreateDisplayKeys(pin_tab_left,
                                  pin_tab_right,
                                  pin_func_left,
                                  pin_func_right);

    auto presenter = CreatePresenter(pump_pin, sensor_pin);

    softkey_view = CSoftkeyDisplay::Create(presenter, keys, display);
}

void loop()
{
    softkey_view->Update();
}
