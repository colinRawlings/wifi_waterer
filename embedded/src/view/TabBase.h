#pragma once

#include "../Types.h"
#include "../Updateable.h"

#include "interfaces/ITabView.h"

#include <memory>
#include <string>
#include <map>
#include <functional>

//

class CKey;
using CKey_ptr = std::shared_ptr<CKey>;

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
    using action_map_t = std::map<CKey_ptr, std::function<void()>>;

  public:
    void Update() override;
    std::string Name() { return _name; }
    void HandleKeys();

  protected:
    CTabBase(std::string name, CPresenter_ptr presenter, ITabView_ptr tab_view,
             CDisplayKeys_ptr keys, CDisplay_ptr display);

    virtual ~CTabBase() = default;

    virtual void UpdateDisplay();

    virtual void OnFuncLeftKey() = 0;
    virtual void OnFuncRightKey() = 0;

    void OnTabLeftKey();
    void OnTabRightKey();

    action_map_t _action_map;

    std::string _name;
    CPresenter_ptr _presenter;
    ITabView_ptr _tab_view;
    CDisplayKeys_ptr _keys;
    CDisplay_ptr _display;
};
