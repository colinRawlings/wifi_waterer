#include "Key.h"

CKey_ptr CKey::Create(String name) {
  auto destroy = [](CKey *display) { delete display; };
  return CKey_ptr(new CKey(name), destroy);
}

CKey::CKey(String name) : _name(name) {}

//

void CKey::SetPushSwitch(IPushSwitch_uptr push_switch) {
  _push_switch = std::move(push_switch);

  if (_push_switch)
    _last_switch_state = _push_switch->GetState();
}

bool CKey::KeyPressed() {
  if (!_push_switch)
    return false;

  if (!_last_switch_state && _push_switch->GetState())
    return true;

  return false;
}
