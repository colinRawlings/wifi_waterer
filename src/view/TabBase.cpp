#include "TabBase.h"

#include "Key.h"
#include "DisplayKeys.h"
#include "Helpers.h"

#include "Display.h"

#include <functional>
#include <map>

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

    if (!_tab_view)
        return;

    std::map<CKey_ptr, std::function<void()>> action_map{
        {_keys->TabLeft(), [this]() { OnTabLeftKey(); }},
        {_keys->TabRight(), [this]() { OnTabRightKey(); }},
        {_keys->FuncLeft(), [this]() { OnFuncLeftKey(); }},
        {_keys->FuncRight(), [this]() { OnFuncRightKey(); }},
    };

    for (const auto & pair : action_map)
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
