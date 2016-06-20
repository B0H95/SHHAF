#include "networkcontroller.hh"

#include <thread>
#include "log.hh"
#include "messagehandler.hh"
#include "udpsocket.hh"

static bool running = true;
static std::thread* netthread = nullptr;

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
