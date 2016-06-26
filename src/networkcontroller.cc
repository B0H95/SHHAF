#include "networkcontroller.hh"

#include <thread>
#include <string>
#include "log.hh"
#include "messagehandler.hh"
#include "udpsocket.hh"

static bool running = true;
static std::thread* netthread = nullptr;
static uint32_t clientport = 0;
static uint32_t serverport = 0;
static std::string serveraddress = "";
static messaging_mode messagingmode = MM_OFFLINE;
static ipaddr destip;
static ipaddr receivedip;

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
    SHH::Log::Log("NetworkController::SetClientPort(): Client port set to " + std::to_string(portnumber) + ".");
    clientport = portnumber;
}

void SHH::NetworkController::SetServerPort(uint32_t portnumber)
{
    SHH::Log::Log("NetworkController::SetServerPort(): Server port set to " + std::to_string(portnumber) + ".");
    serverport = portnumber;
}

void SHH::NetworkController::SetServerAddress(std::string ipaddress)
{
    SHH::Log::Log("NetworkController::SetServerAddress(): Server address set to " + ipaddress + ".");
    serveraddress = ipaddress;
}

bool SHH::NetworkController::SetupConnection()
{
    SHH::Log::Log("NetworkController::SetupConnection(): Connection setup started.");
    if (!SHH::UDP::Open(clientport))
    {
	SHH::Log::Warning("NetworkController::SetupConnection(): Could not open port.");
	return false;
    }
    if (!SHH::UDP::GetIPAddress(&destip, serveraddress, serverport))
    {
	SHH::Log::Warning("NetworkController::SetupConnection(): Could not get ip address.");
	return false;	
    }
    return true;
}

void SHH::NetworkController::SetMessagingMode(messaging_mode mmode)
{
    messagingmode = mmode;
}

static void networkThreadMain()
{
    SHH::Log::Log("networkThreadMain(): Started.");

    message_ctrl cmsg;
    message_sim smsg;
    std::string received = "";

    while (running)
    {
	if (messagingmode != MM_OFFLINE)
	{
	    if (messagingmode == MM_CLIENT)
	    {		
		while ((cmsg = SHH::MessageHandler::PopOutgoingControlMessage()).messagetype != MC_NOTHING)
		{
		    SHH::UDP::Send(SHH::Units::SerializeCtrlMessage(cmsg), destip);
		}
		while ((received = SHH::UDP::Recv(&receivedip)) != "")
		{
		    smsg = SHH::Units::DeserializeSimMessage(received);
		    if (smsg.obj.type == OT_PLAYER)
		    {
			smsg.obj.owner = 1;
			smsg.obj.syncindex = 5000;
		    }
		    SHH::MessageHandler::PushSimulationMessage(smsg);
		}
	    }
	    else if (messagingmode == MM_SERVER)
	    {
		while ((smsg = SHH::MessageHandler::PopSimulationMessage()).messagetype != MS_NOTHING)
		{
		    SHH::UDP::Send(SHH::Units::SerializeSimMessage(smsg), destip);
		}
		while ((received = SHH::UDP::Recv(&receivedip)) != "")
		{
		    cmsg = SHH::Units::DeserializeCtrlMessage(received);
		    SHH::MessageHandler::PushOutgoingControlMessage(cmsg);
		}
	    }
	}
    }
    SHH::Log::Log("networkThreadMain(): Ended successfully.");
}
