#include "ui_components_text.hh"

#include "log.hh"
#include "window.hh"

bool SHH::UI::Components::Text::Init()
{
    if (!SHH::Window::LoadFont("res/fonts/default.ttf", 24))
    {
	SHH::Log::Error("UI::Components::Text::Init(): Could not load font.");
	return false;
    }

    x = 0;
    y = 0;
    fontw = 14;
    fonth = 24;
    str = "";
    highlighted = false;
    return true;
}

void SHH::UI::Components::Text::Deinit()
{
}

void SHH::UI::Components::Text::Draw()
{
    SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);
    SHH::Window::SetFontCharSize(fontw, fonth);

    int xmargin = fontw / 3;
    int ymargin = fonth / 3;

    if (highlighted)
    {
	SHH::Window::SetColor(0x7F, 0xFF, 0x7F, 0x0F);
	SHH::Window::DrawFilledRectangle(x - xmargin, y - ymargin , x + xmargin + (str.length() * fontw), y + ymargin + fonth);
	SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);
    }
    SHH::Window::DrawText("res/fonts/default.ttf", str, x, y);  
}

void SHH::UI::Components::Text::SetPosition(int nx, int ny)
{
    x = nx;
    y = ny;
}

void SHH::UI::Components::Text::SetFontCharSize(int w, int h)
{
    fontw = w;
    fonth = h;
}

void SHH::UI::Components::Text::SetText(std::string text)
{
    str = text;
}

void SHH::UI::Components::Text::Highlight(bool light)
{
    highlighted = light;
}
