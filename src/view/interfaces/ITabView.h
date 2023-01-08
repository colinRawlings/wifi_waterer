#pragma once

#include <memory>

class ITabView;
using ITabView_ptr = std::shared_ptr<ITabView>;

class ITabView {
public:
  virtual void NextTab() = 0;
  virtual void PreviousTab() = 0;
};