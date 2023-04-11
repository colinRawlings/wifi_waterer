#pragma once

#include "Types.h"

#include <vector>
#include <memory>

class IUpdateable;
using IUpdateable_ptr = std::shared_ptr<IUpdateable>;
using IUpdateable_ptr_vec = std::vector<IUpdateable_ptr>;

class IUpdateable
{
  public:
    virtual void Update() = 0;

    virtual void AddChild(IUpdateable_ptr child) = 0;
    virtual void SetChildren(IUpdateable_ptr_vec children) = 0;

    virtual ~IUpdateable() = default;
};
