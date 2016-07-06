#include "ui_components_console.hh"

#include "window.hh"
#include "log.hh"
#include "programcontroller.hh"
#include "ui.hh"

bool SHH::UI::Components::Console::Init()
{
    if (!SHH::Window::LoadFont("res/fonts/default.ttf", 24))
    {
	SHH::Log::Error("UI::Components::Console::Init(): Could not load font.");
	return false;
    }
    
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
	    if (currentKey == "Backspace" && input.length() > 0)
	    {
		input.pop_back();
		return;
	    }
	    if (currentKey == "Space")
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

    SHH::Window::SetColor(0x10, 0x10, 0x10, 0xFF);
    SHH::Window::DrawFilledRectangle(0, 0, SHH::Window::GetWindowWidth(), fontHeight * amountOfLines);
    SHH::Window::SetColor(0x20, 0x20, 0x20, 0xFF);
    SHH::Window::DrawFilledRectangle(0, fontHeight * amountOfLines, SHH::Window::GetWindowWidth(), fontHeight * (amountOfLines + 1));

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
