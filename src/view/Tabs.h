#pragma once

#include "../Types.h"

#include "TabBase.h"

#include "interfaces/ITabView.h"

#include <memory>
#include <vector>

class CDisplayKeys;
using CKeys_ptr = std::shared_ptr<CDisplayKeys>;

class CDisplay;
using CDisplay_ptr = std::shared_ptr<CDisplay>;

class CPresenter;
using CPresenter_ptr = std::shared_ptr<CPresenter>;

//

class CTabs;
using CTabs_ptr = std::shared_ptr<CTabs>;

class CTabs : public ITabView, public std::enable_shared_from_this<CTabs>
{
  public:
    static CTabs_ptr Create(CPresenter_ptr presenter,
                            CKeys_ptr keys, CDisplay_ptr display);

    void Update();

    void NextTab() override;
    void PreviousTab() override;

  private:
    CTabs(CPresenter_ptr presenter,
          CKeys_ptr keys, CDisplay_ptr display);

    void OnCreate(CPresenter_ptr presenter,
                  CKeys_ptr keys, CDisplay_ptr display);

    std::vector<CTabBase_ptr> _tabs;
    size_t _active_tab{0};
};