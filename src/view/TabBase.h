#pragma once

#include "../Types.h"

#include "interfaces/ITabView.h"

#include <memory>

//

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CKeys;
using CKeys_ptr = std::shared_ptr<CKeys>;

class CDisplay;
using CDisplay_ptr = std::shared_ptr<CDisplay>;

//

class CTabBase;
using CTabBase_ptr = std::shared_ptr<CTabBase>;

class CTabBase {
public:
  void Update();

protected:
  CTabBase(String name, CPresenter_ptr presenter, ITabView_ptr tab_view,
           CKeys_ptr keys, CDisplay_ptr display);

  virtual ~CTabBase() = default;

  void HandleKeys();
  virtual void UpdateDisplay();

  virtual void OnFuncLeftKey() = 0;
  virtual void OnFuncRightKey() = 0;

  void OnTabLeftKey();
  void OnTabRightKey();

  String _name;
  CPresenter_ptr _presenter;
  ITabView_ptr _tab_view;
  CKeys_ptr _keys;
  CDisplay_ptr _display;
};