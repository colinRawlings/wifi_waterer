#pragma once

#include "../Types.h"
#include "interfaces/IPushSwitch.h"

class CKey;
using CKey_ptr = std::shared_ptr<CKey>;

class CKey {
public:
  static CKey_ptr Create(String name);

  void SetPushSwitch(IPushSwitch_uptr push_switch);

  bool KeyPressed();

private:
  CKey(String name);

  String _name;
  bool _last_switch_state{false};
  IPushSwitch_uptr _push_switch;
};