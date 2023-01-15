#include "TabBase.h"

#include "Key.h"
#include "DisplayKeys.h"
#include "Helpers.h"

#include "Display.h"

#include <functional>

CTabBase::CTabBase(std::string name, CPresenter_ptr presenter,
                   ITabView_ptr tab_view, CKeys_ptr keys, CDisplay_ptr display)
    : CUpdateable({})
    , _name(name)
    , _presenter(presenter)
    , _tab_view(tab_view)
    , _keys(keys)
    , _display(display)
{}

void CTabBase::OnTabLeftKey()
{
    if (!_tab_view)
        return;

    _tab_view->PreviousTab();
}
void CTabBase::OnTabRightKey()
{
    if (!_tab_view)
        return;

    _tab_view->NextTab();
}

void CTabBase::Update()
{
    CUpdateable::Update();

    UpdateDisplay();
    HandleKeys();
}

void CTabBase::HandleKeys()
{

    if (!_keys)
        return;

    if (auto key = _keys->TabLeft())
    {
        if (key->KeyPressed())
        {
            OnTabLeftKey();
            return;
        }
    }

    if (auto key = _keys->TabRight())
    {
        if (key->KeyPressed())
        {
            OnTabRightKey();
            return;
        }
    }

    if (auto key = _keys->FuncLeft())
    {
        if (key->KeyPressed())
        {
            OnFuncLeftKey();
            return;
        }
    }

    if (auto key = _keys->FuncRight())
    {
        if (key->KeyPressed())
        {
            OnFuncRightKey();
            return;
        }
    }
}

void CTabBase::UpdateDisplay()
{
    if (!_display)
        return;

    _display->SetRow0(FormatRow("<", _name, ">"));
}
