#pragma once

#include "../Types.h"
#include "../Updateable.h"

#include "interfaces/ITabView.h"

#include <memory>
#include <string>

//

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

class CDisplayKeys;
using CDisplayKeys_ptr = std::shared_ptr<CDisplayKeys>;

class CDisplay;
using CDisplay_ptr = std::shared_ptr<CDisplay>;

//

class CTabBase;
using CTabBase_ptr = std::shared_ptr<CTabBase>;

class CTabBase : public CUpdateable
{
  public:
    void Update();

  protected:
    CTabBase(std::string name, CPresenter_ptr presenter, ITabView_ptr tab_view,
             CDisplayKeys_ptr keys, CDisplay_ptr display);

    virtual ~CTabBase() = default;

    void HandleKeys();
    virtual void UpdateDisplay();

    virtual void OnFuncLeftKey() = 0;
    virtual void OnFuncRightKey() = 0;

    void OnTabLeftKey();
    void OnTabRightKey();

    std::string _name;
    CPresenter_ptr _presenter;
    ITabView_ptr _tab_view;
    CDisplayKeys_ptr _keys;
    CDisplay_ptr _display;
};
