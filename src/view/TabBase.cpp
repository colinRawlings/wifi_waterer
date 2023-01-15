#include "TabBase.h"

#include "Key.h"
#include "DisplayKeys.h"
#include "Helpers.h"

#include "Display.h"

#include <functional>

CTabBase::CTabBase(std::string name, CPresenter_ptr presenter,
                   ITabView_ptr tab_view, CDisplayKeys_ptr keys, CDisplay_ptr display)
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

    _tab_view->OnPreviousTab();
}
void CTabBase::OnTabRightKey()
{
    if (!_tab_view)
        return;

    _tab_view->OnNextTab();
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
            if (_tab_view)
                _tab_view->OnKeyPressed();

            OnTabLeftKey();
            return;
        }
    }

    if (auto key = _keys->TabRight())
    {
        if (key->KeyPressed())
        {
            if (_tab_view)
                _tab_view->OnKeyPressed();

            OnTabRightKey();
            return;
        }
    }

    if (auto key = _keys->FuncLeft())
    {
        if (key->KeyPressed())
        {
            if (_tab_view)
                _tab_view->OnKeyPressed();

            OnFuncLeftKey();
            return;
        }
    }

    if (auto key = _keys->FuncRight())
    {
        if (key->KeyPressed())
        {
            if (_tab_view)
                _tab_view->OnKeyPressed();

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
