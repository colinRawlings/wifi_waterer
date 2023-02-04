#include "Application.h"

#include "presenter/PresenterFactory.h"

#include "view/DisplayFactory.h"
#include "view/DisplayKeysFactory.h"
#include "view/SoftkeyDisplay.h"
#include "view/WifiServer.h"

CApplication_ptr CApplication::Create(SSmartPumpPins smart_pump_pins,
                                      SSoftkeyDisplayPins softkey_display_pins)
{
    auto destroy = [](CApplication * app) { delete app; };
    return CApplication_ptr(new CApplication(smart_pump_pins, softkey_display_pins),
                            destroy);
}

CApplication::CApplication(SSmartPumpPins smart_pump_pins,
                           SSoftkeyDisplayPins softkey_display_pins)
{
    auto display = CreateDisplay(softkey_display_pins.lcd_pins);
    auto keys = CreateDisplayKeys(softkey_display_pins.display_key_pins);

    _presenter = CreatePresenter(smart_pump_pins);

    _wifi_server = CWifiServer::Create(_presenter, display);

    _softkey_display = CSoftkeyDisplay::Create(_presenter, keys, display);

    SetChildren({_presenter, _wifi_server, _softkey_display});
}
