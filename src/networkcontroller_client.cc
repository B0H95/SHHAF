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
	SHH::Log::Warning("NetworkController::Client::ConnectToServer(" + destination + ", " + std::to_string(portnumber) + "): Could not resolve address.");
	return false;
    }
    connected = true;
    SHH::UDP::Send("nxc", destip); //TODO: Handle packet loss
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
	    std::string header = received.substr(0, 2);
	    received.erase(0, 2);
	    if (header == "ns")
	    {
		smsg = SHH::Units::DeserializeSimMessage(received);
		SHH::MessageHandler::PushSimulationMessage(smsg);
	    }
	    else if (header == "nc")
	    {
		cmsg = SHH::Units::DeserializeCtrlMessage(received);
		SHH::MessageHandler::PushIncomingControlMessage(cmsg);
	    }
	}
    }
}

void SHH::NetworkController::Client::Disconnect()
{
    if (connected)
    {
	SHH::UDP::Send("nxd", destip);
    }
    connected = false;
}
