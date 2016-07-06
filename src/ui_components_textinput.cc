#include "ui_components_textinput.hh"

#include "window.hh"
#include "log.hh"
#include "programcontroller.hh"

bool SHH::UI::Components::TextInput::Init()
{
    if (!textdrawer.Init())
    {
	SHH::Log::Error("UI::Components::TextInput::Init(): Could not init textdrawer.");
	return false;
    }

    flashtimer = 0;
    framerate = SHH::ProgramController::GetFrameRate();
    focus = false;
    inputstr = "";
    onenter = nullptr;
    extra = nullptr;
    return true;
}

void SHH::UI::Components::TextInput::Deinit()
{
    textdrawer.Deinit();
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
    /*SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);
    SHH::Window::SetFontCharSize(14, 24);
    SHH::Window::DrawText("res/fonts/default.ttf", inputstr, x, y);*/
    if (focus && flashtimer >= framerate / 2)
    {
	textdrawer.SetText(inputstr + "_");
	//SHH::Window::DrawText("res/fonts/default.ttf", "_", x + (inputstr.length() * 14), y);
    }
    else
    {
	textdrawer.SetText(inputstr);
    }
    textdrawer.Draw();
}

void SHH::UI::Components::TextInput::SetFocus(bool f)
{
    focus = f;
}

void SHH::UI::Components::TextInput::SetPosition(int nx, int ny)
{
    textdrawer.SetPosition(nx, ny);
}

std::string SHH::UI::Components::TextInput::GetString()
{
    return inputstr;
}

void SHH::UI::Components::TextInput::OnEnterPressed(void(*callback)(void* data), void* userdata)
{
    onenter = callback;
    extra = userdata;
}
