
#include "src/Types.h"
#include "src/Application.h"

#include "src/hardware/LCDPins.h"
#include "src/hardware/DisplayKeyPins.h"
#include "src/hardware/SmartPumpPins.h"

#include <memory>
#include <vector>

// defs

auto display_key_pins = SDisplayKeyPins(15, 21, 3, 20);
// auto display_key_pins = SDisplayKeyPins(15, 14, 3, 2);

auto lcd_pins = SLCDPins({4, 5, 6, 7}, 9, 8, 18);
auto smart_pump_pins = SSmartPumpPins(16, A3);

auto softkey_display_pins = SSoftkeyDisplayPins(display_key_pins, lcd_pins);

//

CApplication_ptr app;

//

void setup()
{
    LogBegin(9600);
    LogLn("Settings up");

    app = CApplication::Create(smart_pump_pins, softkey_display_pins);
}

void loop()
{
    app->Update();
}
