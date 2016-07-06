#include "ui_components_listselection.hh"

#include "log.hh"
#include "window.hh"

bool SHH::UI::Components::ListSelection::Init()
{
    if (!textdrawer.Init())
    {
	SHH::Log::Error("UI::Components::ListSelection::Init(): Could not init textdrawer.");
	return false;
    }

    x = 0;
    y = 0;
    fontw = 14;
    fonth = 24;
    focus = false;
    selection = 0;
    listsize = 0;
    return true;
}

void SHH::UI::Components::ListSelection::Deinit()
{
    textdrawer.Deinit();
    ClearList();
}

void SHH::UI::Components::ListSelection::ProcessInputs()
{
    if (focus)
    {
	if (SHH::Window::IsKeyPressed("Up"))
	{
	    selection = (selection + listsize - 1) % listsize;
	}
	else if (SHH::Window::IsKeyPressed("Down"))
	{
	    selection = (selection + 1) % listsize;
	}
	else if (SHH::Window::IsKeyPressed("Return"))
	{
	    menuActions[selection](menuActionExtra[selection]);
	}
    }
}

void SHH::UI::Components::ListSelection::Draw()
{
    for (unsigned int i = 0; i < listsize; ++i)
    {
	textdrawer.Highlight(false);
	if (focus && i == selection)
	{
	    textdrawer.Highlight(true);
	}
	textdrawer.SetText(menuItems[i]);
	textdrawer.SetPosition(x, y + (fonth * i * 1.5));
	textdrawer.Draw();
    }    
}

void SHH::UI::Components::ListSelection::AddMenuOption(std::string name, void (*callback)(void* data), void* extra)
{
    menuItems.push_back(name);
    menuActions.push_back(callback);
    menuActionExtra.push_back(extra);
    ++listsize;
}

void SHH::UI::Components::ListSelection::SetFocus(bool f)
{
    focus = f;
}

void SHH::UI::Components::ListSelection::ClearList()
{
    menuItems.clear();
    menuActions.clear();
    menuActionExtra.clear();
    selection = 0;
    listsize = 0;
}

void SHH::UI::Components::ListSelection::SetPosition(int nx, int ny)
{
    x = nx;
    y = ny;
}

void SHH::UI::Components::ListSelection::SetFontSize(int w, int h)
{
    textdrawer.SetFontCharSize(w, h);
    fontw = w;
    fonth = h;
}
