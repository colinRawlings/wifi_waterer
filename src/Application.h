#pragma once

#include "Types.h"
#include "Updateable.h"

#include "hardware/SmartPumpPins.h"
#include "hardware/SoftkeyDisplayPins.h"

#include <memory>

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CSoftkeyDisplay;
using CSoftkeyDisplay_ptr = std::shared_ptr<CSoftkeyDisplay>;

class CApplication;
using CApplication_ptr = std::shared_ptr<CApplication>;

class CApplication : public CUpdateable
{
  public:
    static CApplication_ptr Create(SSmartPumpPins smart_pump_pins,
                                   SSoftkeyDisplayPins softkey_display_pins);

    CPresenter_ptr Presenter();

  protected:
    CApplication(SSmartPumpPins smart_pump_pins,
                 SSoftkeyDisplayPins softkey_display_pins);

    CPresenter_ptr _presenter;
    CSoftkeyDisplay_ptr _softkey_display;
};
