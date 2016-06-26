#include "networkcontroller_client.hh"

#include "log.hh"
#include "udpsocket.hh"
#include "units.hh"
#include "messagehandler.hh"

static ipaddr destip;
static ipaddr receivedip;
static bool connected = false;

bool SHH::NetworkController::Client::Init()
{
    SHH::Log::Log("NetworkController::Client::Init(): Started.");
    
    destip.Reset();

    SHH::Log::Log("NetworkController::Client::Init(): Ended successfully.");
    return true;
}

void SHH::NetworkController::Client::Deinit()
{
    SHH::Log::Log("NetworkController::Client::Deinit(): Started.");
    SHH::Log::Log("NetworkController::Client::Deinit(): Ended successfully.");
}

void SHH::NetworkController::Client::Reset()
{
    destip.Reset();
    connected = false;
}

bool SHH::NetworkController::Client::ConnectToServer(std::string destination, uint32_t portnumber)
{
    if (!SHH::UDP::GetIPAddress(&destip, destination, portnumber))
    {
	SHH::Log::Warning("NetworkController::Client::ConnectToServer(" + destination + ", " + std::to_string(portnumber) + "): Could not connect.");
	return false;
    }
    connected = true;
    return true;
}

void SHH::NetworkController::Client::HandleMessages()
{
    message_ctrl cmsg;
    message_sim smsg;
    std::string received = "";

    if (connected)
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
}
