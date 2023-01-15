#pragma once

#include "../Types.h"
#include "../Updateable.h"

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

class CSoftkeyDisplay;
using CSoftkeyDisplay_ptr = std::shared_ptr<CSoftkeyDisplay>;

class CSoftkeyDisplay : public ITabView, public std::enable_shared_from_this<CSoftkeyDisplay>, public CUpdateable
{
  public:
    static CSoftkeyDisplay_ptr Create(CPresenter_ptr presenter,
                                      CKeys_ptr keys, CDisplay_ptr display);

    void Update() override;

    void NextTab() override;
    void PreviousTab() override;

  private:
    CSoftkeyDisplay(CPresenter_ptr presenter,
                    CKeys_ptr keys, CDisplay_ptr display);
    virtual ~CSoftkeyDisplay() = default;

    void OnCreate(CPresenter_ptr presenter,
                  CKeys_ptr keys, CDisplay_ptr display);

    std::vector<CTabBase_ptr> _tabs;
    size_t _active_tab{0};

    CPresenter_ptr _presenter;
};
