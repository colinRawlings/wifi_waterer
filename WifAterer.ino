#include <memory>
#include <vector>
#include <LiquidCrystal.h>

#include "src/hardware/PushSwitch.h"
#include "src/hardware/LiquidCrystalDisplay.h"

#include "src/view/Key.h"
#include "src/view/Display.h"

// defs

const byte pin_func_right = 2;
auto lcd_pins = SLCDPins({4, 5, 6, 7}, 9, 8);

auto key = CKey::Create("dummy");
auto display = CDisplay::Create();

int counter{0};

// the setup function runs once when you press reset or power the board
void setup()
{
    auto sw = CPushSwitch::Create(2, true);
    key->SetPushSwitch(std::move(sw));

    auto lcd = CLiquidCrystalDisplay::Create(lcd_pins);
    display->SetLcd(std::move(lcd));

    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
    if (key->KeyPressed())
        ++counter;

    display->SetRow0(std::to_string(counter));
}