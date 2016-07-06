#include "ui_components_listselection.hh"

#include "log.hh"
#include "window.hh"

bool SHH::UI::Components::ListSelection::Init()
{
    if (!SHH::Window::LoadFont("res/fonts/default.ttf", 24))
    {
	SHH::Log::Error("UI::Components::ListSelection::Init(): Could not load font.");
	return false;
    }

    x = 0;
    y = 0;
    fontw = 14;
    fonth = 24;
    focus = true;
    selection = 0;
    listsize = 0;
    return true;
}

void SHH::UI::Components::ListSelection::Deinit()
{
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
    SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);
    SHH::Window::SetFontCharSize(fontw, fonth);

    int xmargin = fontw / 3;
    int ymargin = fonth / 3;
    int height = fonth + (ymargin * 2);

    for (unsigned int i = 0; i < listsize; ++i)
    {
	if (focus && i == selection)
	{
	    SHH::Window::SetColor(0x7F, 0xFF, 0x7F, 0x0F);
	    SHH::Window::DrawFilledRectangle(x, y + (height * i), x + (xmargin * 2) + (menuItems[i].length() * fontw), y + height + (height * i));
	    SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);
	}
	SHH::Window::DrawText("res/fonts/default.ttf", menuItems[i], x + xmargin, y + ymargin + (height * i));
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
    fontw = w;
    fonth = h;
}
