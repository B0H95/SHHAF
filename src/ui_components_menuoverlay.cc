#include "ui_components_menuoverlay.hh"

#include "ui_commands.hh"

#include "log.hh"
#include "window.hh"
#include "programcontroller.hh"

static const char* const menuOptions[] = {"Start game", "Quit"};
static const unsigned int menuOptionsSize = 2;

bool SHH::UI::Components::MenuOverlay::Init()
{
    if (!SHH::Window::LoadFont("res/fonts/default.ttf", 24))
    {
	SHH::Log::Error("UI::Components::MenuOverlay::Init(): Could not load font.");
	return false;
    }
    
    visible = true;
    selection = 0;
    return true;
}

void SHH::UI::Components::MenuOverlay::Deinit()
{
}

void SHH::UI::Components::MenuOverlay::ProcessInputs()
{
    if (SHH::Window::IsKeyPressed("Escape"))
    {
	visible = !visible;
    }
    else if (SHH::Window::IsKeyPressed("Up"))
    {
	selection = (selection + menuOptionsSize - 1) % menuOptionsSize;
    }
    else if (SHH::Window::IsKeyPressed("Down"))
    {
	selection = (selection + 1) % menuOptionsSize;
    }
    else if (SHH::Window::IsKeyPressed("Return"))
    {
	if (selection == 0)
	{
	    SHH::UI::Commands::Map("de_dust2");
	    visible = false;
	}
	else if (selection == 1)
	{
	    SHH::ProgramController::Quit();
	}
    }
}

void SHH::UI::Components::MenuOverlay::Draw()
{
    if (visible)
    {
	int screenwidth = SHH::Window::GetWindowWidth();
	int screenheight = SHH::Window::GetWindowHeight();
	SHH::Window::SetColor(0x00, 0x00, 0x00, 0xC0);
	SHH::Window::DrawFilledRectangle(0, 0, screenwidth, screenheight);
	SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);
	SHH::Window::SetFontCharSize(14, 24);
	SHH::Window::DrawText("res/fonts/default.ttf", "\"Practice, practice, practice, ...\", Fatal1ty", 20, 100);

	for (unsigned int i = 0; i < menuOptionsSize; ++i)
	{
	    if (i == selection)
	    {
		SHH::Window::SetColor(0x7F, 0xFF, 0x7F, 0x0F);
		SHH::Window::DrawFilledRectangle(40, 200 + (40 * i), 210, 240 + (40 * i));
		SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);
	    }
	    SHH::Window::DrawText("res/fonts/default.ttf", menuOptions[i], 45, 208 + (40 * i));
	}
    }
}

bool SHH::UI::Components::MenuOverlay::Visible()
{
    return visible;
}
