#pragma once

#include "../../Types.h"

#include <memory>

class ITabView;
using ITabView_ptr = std::shared_ptr<ITabView>;

class ITabView
{
  public:
    // returns true if event handled
    virtual bool OnNextTab() = 0;
    virtual bool OnPreviousTab() = 0;

    virtual bool OnKeyPressed() = 0;
};
