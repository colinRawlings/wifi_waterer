#pragma once

#include "Types.h"

#include <vector>
#include <memory>

class CUpdateable;
using CUpdateable_ptr = std::shared_ptr<CUpdateable>;
using CUpdateable_vec = std::vector<CUpdateable_ptr>;

class CUpdateable
{
  public:
    CUpdateable();

    virtual void Update();

  protected:
    void SetChildren(CUpdateable_vec children);

  private:
    CUpdateable_vec _children;
};
