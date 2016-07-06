#include "ui_components_menuoverlay.hh"

#include "ui_commands.hh"

#include "log.hh"
#include "window.hh"
#include "programcontroller.hh"

static void StartPressed(void* data);
static void QuitPressed(void* data);

bool SHH::UI::Components::MenuOverlay::Init()
{
    if (!SHH::Window::LoadFont("res/fonts/default.ttf", 24))
    {
	SHH::Log::Error("UI::Components::MenuOverlay::Init(): Could not load font.");
	return false;
    }

    if (!menulist.Init())
    {
	SHH::Log::Error("UI::Components::MenuOverlay::Init(): Could not init menulist.");
	return false;
    }

    menulist.AddMenuOption("Start game", StartPressed, (void*)&visible);
    menulist.AddMenuOption("Quit", QuitPressed, nullptr);
    menulist.SetPosition(40, 200);
    menulist.SetFocus(true);
    
    visible = true;
    return true;
}

void SHH::UI::Components::MenuOverlay::Deinit()
{
    menulist.Deinit();
}

void SHH::UI::Components::MenuOverlay::ProcessInputs()
{
    if (SHH::Window::IsKeyPressed("Escape"))
    {
	visible = !visible;
	menulist.SetFocus(visible);
    }
    menulist.ProcessInputs();
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
	menulist.Draw();
    }
}

bool SHH::UI::Components::MenuOverlay::Visible()
{
    return visible;
}

static void StartPressed(void* data)
{
    SHH::UI::Commands::Map("de_dust2");
    *((bool*)data) = false;
}

static void QuitPressed(void* data)
{
    (void)data;
    SHH::ProgramController::Quit();
}
