#include "ui_components_textinput.hh"

#include "window.hh"
#include "log.hh"
#include "programcontroller.hh"

bool SHH::UI::Components::TextInput::Init()
{
    if (!SHH::Window::LoadFont("res/fonts/default.ttf", 24))
    {
	SHH::Log::Error("UI::Components::TextInput::Init(): Could not load font.");
	return false;
    }

    flashtimer = 0;
    framerate = SHH::ProgramController::GetFrameRate();
    x = 0;
    y = 0;
    focus = false;
    inputstr = "";
    onenter = nullptr;
    extra = nullptr;
    return true;
}

void SHH::UI::Components::TextInput::Deinit()
{
}

void SHH::UI::Components::TextInput::ProcessInputs()
{
    if (focus)
    {
	std::string currentKey = SHH::Window::GetCurrentKey();
	if (currentKey != "")
	{
	    if (currentKey == "Backspace" && inputstr.length() > 0)
	    {
		inputstr.pop_back();
		return;
	    }
	    if (currentKey == "Space")
	    {
		inputstr += " ";
		return;
	    }
	    if (currentKey == "Return" && onenter != nullptr)
	    {
		onenter(extra);
	    }
	    if (currentKey.length() == 1)
	    {
		inputstr += currentKey;
	    }
	}
    }
}

void SHH::UI::Components::TextInput::Draw()
{
    flashtimer = (flashtimer + 1) % framerate;
    SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);
    SHH::Window::SetFontCharSize(14, 24);
    SHH::Window::DrawText("res/fonts/default.ttf", inputstr, x, y);
    if (focus && flashtimer >= framerate / 2)
    {
	SHH::Window::DrawText("res/fonts/default.ttf", "_", x + (inputstr.length() * 14), y);
    }
}

void SHH::UI::Components::TextInput::SetFocus(bool f)
{
    focus = f;
}

void SHH::UI::Components::TextInput::SetPosition(int nx, int ny)
{
    x = nx;
    y = ny;
}

void SHH::UI::Components::TextInput::OnEnterPressed(void(*callback)(void* data), void* userdata)
{
    onenter = callback;
    extra = userdata;
}
