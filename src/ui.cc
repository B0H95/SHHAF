#include "ui.hh"

#include "ui_commands.hh"
#include "ui_components_main.hh"

#include "log.hh"
#include "window.hh"
#include "util.hh"

static SHH::UI::Components::Main mainComponent;

bool SHH::UI::Init()
{
    SHH::Log::Log("UI::Init(): Started.");

    if (!mainComponent.Init())
    {
	SHH::Log::Error("UI::Init(): Could not init main component.");
	return false;
    }

    SHH::Log::Log("UI::Init(): Ended successfully.");
    return true;
}

void SHH::UI::Deinit()
{
    SHH::Log::Log("UI::Deinit(): Started.");

    mainComponent.Deinit();
    
    SHH::Log::Log("UI::Deinit(): Ended successfully.");
}

void SHH::UI::Draw()
{
    SHH::Window::SetColor(0x00, 0x00, 0x00, 0xFF);
    SHH::Window::ClearScreen();

    mainComponent.Draw();
    
    SHH::Window::UpdateScreen();
}

void SHH::UI::ProcessInputs()
{
    mainComponent.ProcessInputs();
}

void SHH::UI::SendMessage(std::string msg)
{
    SHH::Log::Log(msg);

    //TODO: Check for parsing errors

    std::size_t separatorpos = msg.find(" ");
    std::string cmd = msg.substr(0, separatorpos);
    msg.erase(0, separatorpos + 1);
    std::string param = msg;

    if (cmd == "LOCALPORT")
    {
	SHH::UI::Commands::SetLocalPort(SHH::Util::StringToInt(param));
    }
    if (cmd == "MAP")
    {
	SHH::UI::Commands::Map(param);
    }
    else if (cmd == "DISCONNECT")
    {
	SHH::UI::Commands::Disconnect();
    }
    else if (cmd == "MMODE")
    {
	SHH::UI::Commands::GetMessagingMode();
    }
}
