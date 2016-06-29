#include "ui_commands.hh"

#include "networkcontroller.hh"
#include "simulation.hh"
#include "simulation_map.hh"
#include "messagehandler.hh"
#include "log.hh"
#include "units.hh"

static void SetGlobalMessagingMode(messaging_mode mmode);

bool SHH::UI::Commands::SetLocalPort(uint32_t portnumber)
{
    return SHH::NetworkController::OpenPort(portnumber);
}

bool SHH::UI::Commands::StartServer()
{
    if (SHH::MessageHandler::GetMessagingMode() != MM_OFFLINE)
    {
	SHH::Log::Warning("UI::SendMessage(): Must be in offline mode to start a server.");
	return false;
    }
    SetGlobalMessagingMode(MM_SERVER);
    return true;
}

bool SHH::UI::Commands::Connect(std::string serverip, uint32_t serverport)
{
    SHH::Simulation::Map::FlushObjects(); //TODO: Fix something in Simulation instead
    SetGlobalMessagingMode(MM_CLIENT);
    SHH::NetworkController::ConnectToServer(serverip, serverport);
    return true;
}

bool SHH::UI::Commands::Disconnect()
{
    SHH::NetworkController::Disconnect();
    SetGlobalMessagingMode(MM_OFFLINE);
    SHH::Simulation::LoadMap("default");
    return true;
}

bool SHH::UI::Commands::Map(std::string mapname) //TODO: Go to offline mode properly
{
    SetGlobalMessagingMode(MM_OFFLINE);
    SHH::Simulation::LoadMap(mapname);
    return true;
}

static void SetGlobalMessagingMode(messaging_mode mmode)
{
    SHH::MessageHandler::SetMessagingMode(mmode);
    SHH::Simulation::SetMessagingMode(mmode);
    SHH::NetworkController::SetMessagingMode(mmode);
}
