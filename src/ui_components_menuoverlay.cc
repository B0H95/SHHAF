#include "ui_components_menuoverlay.hh"

#include "ui_commands.hh"

#include "log.hh"
#include "window.hh"
#include "programcontroller.hh"

static void JoinPressed(void* data);
static void HostPressed(void* data);
static void QuitPressed(void* data);

bool SHH::UI::Components::MenuOverlay::Init()
{
    if (!titletext.Init())
    {
	SHH::Log::Error("UI::Components::MenuOverlay::Init(): Could not init titletext.");
	return false;
    }

    if (!menulist.Init())
    {
	SHH::Log::Error("UI::Components::MenuOverlay::Init(): Could not init menulist.");
	return false;
    }

    menulist.AddMenuOption("Join game", JoinPressed, (void*)&visible);
    menulist.AddMenuOption("Host game", HostPressed, nullptr);
    menulist.AddMenuOption("Quit", QuitPressed, nullptr);
    menulist.SetPosition(40, 200);
    menulist.SetFocus(true);

    titletext.SetPosition(20, 100);
    titletext.SetFontCharSize(14, 24);
    titletext.SetText("\"Practice, practice, practice, ...\", Fatal1ty");
    
    visible = true;
    return true;
}

void SHH::UI::Components::MenuOverlay::Deinit()
{
    titletext.Deinit();
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
	titletext.Draw();
	menulist.Draw();
    }
}

bool SHH::UI::Components::MenuOverlay::Visible()
{
    return visible;
}

static void JoinPressed(void* data)
{
    SHH::UI::Commands::Map("de_dust2");
    *((bool*)data) = false;
}

static void HostPressed(void* data)
{
    (void)data;
    SHH::Log::Log("HostPressed");
}

static void QuitPressed(void* data)
{
    (void)data;
    SHH::ProgramController::Quit();
}
