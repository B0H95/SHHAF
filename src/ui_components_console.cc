#include "ui_components_console.hh"

#include "window.hh"
#include "log.hh"
#include "programcontroller.hh"
#include "ui.hh"

bool SHH::UI::Components::Console::Init()
{
    visible = false;
    input = "";
    scroll = 0;
    return true;
}

void SHH::UI::Components::Console::Deinit()
{
}

void SHH::UI::Components::Console::ProcessInputs()
{
    std::string currentKey = SHH::Window::GetCurrentKey();
    if (currentKey != "")
    {
	if (currentKey == "1" && SHH::Window::IsKeyDown("Right Shift"))
	{
	    visible = !visible;
	    return;
	}
	if (visible)
	{
	    if (SHH::Window::IsKeyPressed("Backspace") && input.length() > 0)
	    {
		input.pop_back();
		return;
	    }
	    if (SHH::Window::IsKeyPressed("Space"))
	    {
		input += " ";
		return;
	    }
	    if (SHH::Window::IsKeyPressed("Return") && input.length() > 0)
	    {
		SHH::UI::SendMessage(input);
		input = "";
		return;
	    }
	    if (SHH::Window::IsKeyPressed("PageUp"))
	    {
		++scroll;
		return;
	    }
	    if (SHH::Window::IsKeyPressed("PageDown") && scroll > 0)
	    {
		--scroll;
		return;
	    }
	    std::string currentKey = SHH::Window::GetCurrentKey();
	    if (currentKey.length() == 1)
	    {
		input += currentKey;
	    }
	}
    }
}

void SHH::UI::Components::Console::Draw()
{
    if (!visible)
    {
	return;
    }
    const int fontHeight = 16;
    const int amountOfLines = (SHH::Window::GetWindowHeight() / (fontHeight * 2)) - 2;

    SHH::Window::SetFontCharSize(9, fontHeight);
    SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);

    for (int i = 0; i < amountOfLines; ++i)
    {
	SHH::Window::DrawText("res/fonts/default.ttf", SHH::Log::GetLogEntry(amountOfLines - i - 1 + scroll), 0, i * fontHeight);
    }

    std::string inputLine = "> " + input + " <";
    SHH::Window::DrawText("res/fonts/default.ttf", inputLine, 0, amountOfLines * fontHeight);
}

bool SHH::UI::Components::Console::Visible()
{
    return visible;
}
