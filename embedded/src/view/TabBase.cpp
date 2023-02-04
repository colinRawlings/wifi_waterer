#include "TabBase.h"

#include "Key.h"
#include "DisplayKeys.h"
#include "Helpers.h"

#include "Display.h"

CTabBase::CTabBase(std::string name, CPresenter_ptr presenter,
                   ITabView_ptr tab_view, CDisplayKeys_ptr keys, CDisplay_ptr display)
    : CUpdateable({})
    , _name(name)
    , _presenter(presenter)
    , _tab_view(tab_view)
    , _keys(keys)
    , _display(display)
{
    _action_map = action_map_t{
        {_keys->TabLeft(), [this]() { OnTabLeftKey(); }},
        {_keys->TabRight(), [this]() { OnTabRightKey(); }},
        {_keys->FuncLeft(), [this]() { OnFuncLeftKey(); }},
        {_keys->FuncRight(), [this]() { OnFuncRightKey(); }},
    };
}

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

    if (!_tab_view)
        return;

    for (const auto & pair : _action_map)
    {
        auto key = pair.first;
        auto callback = pair.second;

        if (!key)
            continue;

        if (!key->KeyPressed())
            continue;

        if (_tab_view->OnKeyPressed()) // event handled by base
            return;

        callback();
    }
}

void CTabBase::UpdateDisplay()
{
    if (!_display)
        return;

    _display->SetRow0(FormatRow("<", _name, ">"));
}
