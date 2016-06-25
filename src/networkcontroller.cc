#include "networkcontroller.hh"

#include <thread>
#include "log.hh"
#include "messagehandler.hh"
#include "udpsocket.hh"

static bool running = true;
static std::thread* netthread = nullptr;
static uint32_t clientport = 0;
static uint32_t serverport = 0;
static std::string serveraddress = "";
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

void SHH::NetworkController::SetClientPort(uint32_t portnumber)
{
    clientport = portnumber;
}

void SHH::NetworkController::SetServerPort(uint32_t portnumber)
{
    serverport = portnumber;
}

void SHH::NetworkController::SetServerAddress(std::string ipaddress)
{
    serveraddress = ipaddress;
}

void SHH::NetworkController::SetMessagingMode(messaging_mode mmode)
{
    messagingmode = mmode;
}

static void networkThreadMain()
{
    SHH::Log::Log("networkThreadMain(): Started.");
    while (running)
    {
	if (SHH::MessageHandler::GetMessagingMode() != MM_OFFLINE)
	{
	    //TODO: Start sending and receiving stuff
	}
    }
    SHH::Log::Log("networkThreadMain(): Ended successfully.");
}
