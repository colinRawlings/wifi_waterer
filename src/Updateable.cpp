#include "Updateable.h"

CUpdateable::CUpdateable()
{}

void CUpdateable::SetChildren(CUpdateable_vec children)
{
    _children = children;
}

void CUpdateable::Update()
{
    for (auto & child : _children)
        child->Update();
}
