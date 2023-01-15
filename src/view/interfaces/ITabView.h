#pragma once

#include "../../Types.h"

#include <memory>

class ITabView;
using ITabView_ptr = std::shared_ptr<ITabView>;

class ITabView
{
  public:
    virtual void OnNextTab() = 0;
    virtual void OnPreviousTab() = 0;

    virtual void OnKeyPressed() = 0;
};
