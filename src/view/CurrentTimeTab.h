#pragma once

#include "TabBase.h"

class CCurrentTimeTab;
using CCurrentTimeTab_ptr = std::shared_ptr<CCurrentTimeTab>;

class CCurrentTimeTab : public CTabBase {
public:
  static CCurrentTimeTab_ptr Create(CPresenter_ptr presenter,
                                    ITabView_ptr tab_view, CKeys_ptr keys,
                                    CDisplay_ptr display);

protected:
  CCurrentTimeTab(CPresenter_ptr presenter, ITabView_ptr tab_view,
                  CKeys_ptr keys, CDisplay_ptr display);

  void UpdateDisplay() override;

  void OnFuncLeftKey() override;
  void OnFuncRightKey() override;
};