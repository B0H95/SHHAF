#include "ui_components_console.hh"

#include "window.hh"
#include "log.hh"
#include "programcontroller.hh"
#include "ui.hh"

bool SHH::UI::Components::Console::Init()
{
    visible = false;
    input = "";
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
	SHH::Window::DrawText(SHH::Log::GetLogEntry(amountOfLines - i - 1), 0, i * fontHeight);
    }

    std::string inputLine = "> " + input + " <";
    SHH::Window::DrawText(inputLine, 0, amountOfLines * fontHeight);
}
