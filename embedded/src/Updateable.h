#pragma once

#include "Types.h"

#include "IUpdateable.h"

#include <vector>
#include <memory>

class CUpdateable : public IUpdateable
{
  public:
    CUpdateable();

    void Update() override;

    void AddChild(IUpdateable_ptr child) override;
    void SetChildren(IUpdateable_ptr_vec children) override;

  private:
    IUpdateable_ptr_vec _children;
};
