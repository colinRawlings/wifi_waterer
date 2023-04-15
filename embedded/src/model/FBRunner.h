#pragma once

#include "../Types.h"
#include "../Updateable.h"

#include "interfaces/IRealTimeClock.h"
#include "interfaces/ISmartPump.h"

#include <memory.h>

class CFBRunner;
using CFBRunner_ptr = std::shared_ptr<CFBRunner>;

class CFBSettings;
using CFBSettings_ptr = std::shared_ptr<CFBSettings>;

class CFBRunner : public CUpdateable, public std::enable_shared_from_this<CFBRunner>
{
  public:
    static CFBRunner_ptr Create(IRealTimeClock_ptr clock,
                                ISmartPump_ptr pump,
                                CFBSettings_ptr settings);

    void Update() override;

    ~CFBRunner() = default;

  private:
    CFBRunner(IRealTimeClock_ptr clock,
              ISmartPump_ptr pump,
              CFBSettings_ptr settings);

    IRealTimeClock_ptr _clock;
    ISmartPump_ptr _pump;
    CFBSettings_ptr _settings;

    byte _last_update_hour = 0;
};
