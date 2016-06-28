#include "networkcontroller.hh"

#include "networkcontroller_client.hh"
#include "networkcontroller_server.hh"

#include <thread>
#include <string>
#include "log.hh"
#include "messagehandler.hh"
#include "udpsocket.hh"

static bool running = true;
static std::thread* netthread = nullptr;
static uint32_t clientport = 0;
static messaging_mode messagingmode = MM_OFFLINE;

static void networkThreadMain();

bool SHH::NetworkController::Init()
{
    SHH::Log::Log("NetworkController::Init(): Started.");
    
    if (!SHH::UDP::Init(512))
    {
	SHH::Log::Error("NetworkController::Init(): Could not init UDP.");
	return false;
    }

    if (!SHH::NetworkController::Server::Init())
    {
	SHH::Log::Error("NetworkController::Init(): Could not init server.");
	SHH::UDP::Deinit();
	return false;
    }

    if (!SHH::NetworkController::Client::Init())
    {
	SHH::Log::Error("NetworkController::Init(): Could not init client.");
	SHH::NetworkController::Server::Deinit();
	SHH::UDP::Deinit();
	return false;
    }

    SHH::Log::Log("NetworkController::Init(): Ended successfully.");
    return true;
}

void SHH::NetworkController::Deinit()
{
    SHH::Log::Log("NetworkController::Deinit(): Started.");
    
    running = false;

    if (netthread != nullptr)
    {
	SHH::Log::Log("NetworkController::Deinit(): Waiting for netthread to finish.");
	netthread->join();
	delete netthread;
	netthread = nullptr;
    }

    SHH::NetworkController::Client::Deinit();
    SHH::NetworkController::Server::Deinit();
    SHH::UDP::Deinit();

    SHH::Log::Log("NetworkController::Deinit(): Ended successfully.");
}

bool SHH::NetworkController::Run()
{
    SHH::Log::Log("NetworkController::Run(): Started.");

    running = true;
    netthread = new std::thread (networkThreadMain);
    if (netthread == nullptr)
    {
	SHH::Log::Error("NetworkController::Init(): Could not create network thread.");
	return false;
    }

    SHH::Log::Log("NetworkController::Run(): Ended successfully.");
    return true;
}

void SHH::NetworkController::Stop()
{
    SHH::Log::Log("NetworkController::Stop(): Stop called.");

    running = false;

    if (netthread != nullptr)
    {
	SHH::Log::Log("NetworkController::Stop(): Waiting for netthread to finish.");
	netthread->join();
	delete netthread;
	netthread = nullptr;
    }
}

bool SHH::NetworkController::OpenPort(uint32_t portnumber)
{
    SHH::Log::Log("NetworkController::OpenPort(" + std::to_string(portnumber) + "): Attempting to open port.");
    if (!SHH::UDP::Open(portnumber))
    {
	SHH::Log::Error("NetworkController::OpenPort(" + std::to_string(portnumber) + "): Could not open port.");
	return false;
    }
    clientport = portnumber;
    return true;
}

bool SHH::NetworkController::ConnectToServer(std::string destination, uint32_t portnumber)
{
    SHH::Log::Log("NetworkController::ConnectToServer(" + destination + ", " + std::to_string(portnumber) + "): Attempting to connect.");

    if (messagingmode != MM_CLIENT)
    {
	SHH::Log::Warning("NetworkController::ConnectToServer(): Must be in client mode.");
	return false;
    }
    
    if (!SHH::NetworkController::Client::ConnectToServer(destination, portnumber))
    {
	SHH::Log::Error("NetworkController::Client::ConnectToServer(" + destination + ", " + std::to_string(portnumber) + "): Could not connect.");
	return false;
    }

    return true;
}

void SHH::NetworkController::Disconnect()
{
    if (messagingmode == MM_CLIENT)
    {
	SHH::NetworkController::Client::Disconnect();
    }
}

void SHH::NetworkController::SetMessagingMode(messaging_mode mmode)
{
    messagingmode = mmode;
    if (mmode == MM_CLIENT)
    {
	SHH::NetworkController::Client::Reset();
    }
    else if (mmode == MM_SERVER)
    {
	SHH::NetworkController::Server::Reset();
    }
    if (mmode == MM_OFFLINE)
    {
	SHH::NetworkController::Client::Disconnect();
    }
}

static void networkThreadMain()
{
    SHH::Log::Log("networkThreadMain(): Started.");

    while (running)
    {
	if (messagingmode == MM_CLIENT)
	{		
	    SHH::NetworkController::Client::HandleMessages();
	}
	else if (messagingmode == MM_SERVER)
	{
	    SHH::NetworkController::Server::HandleMessages();
	}
    }
    SHH::Log::Log("networkThreadMain(): Ended successfully.");
}
