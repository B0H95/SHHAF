#include "networkcontroller_server.hh"

#include <string>
#include "log.hh"
#include "messagehandler.hh"
#include "units.hh"
#include "udpsocket.hh"

static const int MAX_CLIENTS = 32;
static ipaddr* clients = nullptr;
static int numclients = 0;

static void InsertClientIfNew(ipaddr const& ip);

bool SHH::NetworkController::Server::Init()
{
    SHH::Log::Log("NetworkController::Server::Init(): Started.");

    clients = new ipaddr [MAX_CLIENTS];
    if (clients == nullptr)
    {
	SHH::Log::Error("NetworkController::Server::Init(): Could not allocate memory for iplist.");
	return false;
    }

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
	clients[i].Reset();
    }

    SHH::Log::Log("NetworkController::Server::Init(): Ended successfully.");
    return true;
}

void SHH::NetworkController::Server::Deinit()
{
    SHH::Log::Log("NetworkController::Server::Deinit(): Started.");

    if (clients != nullptr)
    {
	delete[] clients;
	clients = nullptr;
    }
    numclients = 0;

    SHH::Log::Log("NetworkController::Server::Deinit(): Ended successfully.");
}

void SHH::NetworkController::Server::Reset()
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
	clients[i].Reset();
    }
    numclients = 0;
}

void SHH::NetworkController::Server::HandleMessages()
{
    message_ctrl cmsg;
    message_sim smsg;
    std::string received = "";
    ipaddr receivedip;

    while ((smsg = SHH::MessageHandler::PopSimulationMessage()).messagetype != MS_NOTHING)
    {
	std::string serializedMessage = SHH::Units::SerializeSimMessage(smsg);
	for (int i = 0; i < numclients; ++i)
	{
	    SHH::UDP::Send(serializedMessage, clients[i]);
	}
    }
    while ((received = SHH::UDP::Recv(&receivedip)) != "")
    {
	cmsg = SHH::Units::DeserializeCtrlMessage(received);
	SHH::MessageHandler::PushOutgoingControlMessage(cmsg);
	InsertClientIfNew(receivedip);
    }
}

static void InsertClientIfNew(ipaddr const& ip)
{
    for (int i = 0; i < numclients; ++i)
    {
	if (clients[i].Equals(ip))
	{
	    return;
	}
	if (i == numclients - 1)
	{
	    SHH::Log::Warning("NetworkController::Server InsertClientIfNew(): No more room for clients.");
	    return;
	}
    }
    clients[numclients] = ip;
    ++numclients;
}
