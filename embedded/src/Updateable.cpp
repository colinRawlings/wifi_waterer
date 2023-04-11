#include "Updateable.h"

CUpdateable::CUpdateable()
{}

void CUpdateable::SetChildren(IUpdateable_ptr_vec children)
{
    _children = children;
}

void CUpdateable::AddChild(IUpdateable_ptr child)
{
    _children.push_back(child);
}

void CUpdateable::Update()
{
    for (auto & child : _children)
    {
        if (child)
            child->Update();
    }
}
