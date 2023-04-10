#include "Updateable.h"

CUpdateable::CUpdateable()
{}

void CUpdateable::SetChildren(CUpdateable_vec children)
{
    _children = children;
}

void CUpdateable::AddChild(CUpdateable_ptr child)
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
