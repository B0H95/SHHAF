#include "networkcontroller_server.hh"

#include <chrono>
#include <string>
#include "log.hh"
#include "messagehandler.hh"
#include "units.hh"
#include "udpsocket.hh"

struct client
{
    ipaddr ip;
    unsigned int id;
    std::chrono::steady_clock::time_point lastupdate;
};

static const float MAX_INACTIVITY_TIME = 30.0f;
static const int MAX_CLIENTS = 32;
static client* clients = nullptr;
static int numclients = 0;
static unsigned int playerid = 1;

static void InsertClientIfNew(ipaddr const& ip);
static void DeleteClientIfExists(ipaddr const& ip);
static int ClientId(ipaddr const& ip);
static int ClientListId(unsigned int uid);
static void HandleIncomingMessage(std::string const& msg, ipaddr const& sender);
static void SendPlayerIdentificationMessage(ipaddr const& ip, unsigned int id);
static void CheckForClientInactivity();

bool SHH::NetworkController::Server::Init()
{
    SHH::Log::Log("NetworkController::Server::Init(): Started.");

    clients = new client [MAX_CLIENTS];
    if (clients == nullptr)
    {
	SHH::Log::Error("NetworkController::Server::Init(): Could not allocate memory for iplist.");
	return false;
    }

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
	clients[i].ip.Reset();
	clients[i].id = 0;
	clients[i].lastupdate = std::chrono::steady_clock::now();
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
	clients[i].ip.Reset();
	clients[i].id = 0;
	clients[i].lastupdate = std::chrono::steady_clock::now();
    }
    numclients = 0;
    playerid = 1;
}

void SHH::NetworkController::Server::HandleMessages()
{
    message_ctrl cmsg;
    message_sim smsg;
    std::string received = "";
    ipaddr receivedip;

    CheckForClientInactivity();
    while ((smsg = SHH::MessageHandler::PopSimulationMessage()).messagetype != MS_NOTHING)
    {
	std::string serializedMessage = SHH::Units::SerializeSimMessage(smsg);
	for (int i = 0; i < numclients; ++i)
	{
	    SHH::UDP::Send(serializedMessage, clients[i].ip);
	}
    }
    while ((received = SHH::UDP::Recv(&receivedip)) != "")
    {
	HandleIncomingMessage(received, receivedip);
    }
}

static void InsertClientIfNew(ipaddr const& ip)
{
    for (int i = 0; i < numclients; ++i)
    {
	if (clients[i].ip.Equals(ip))
	{
	    SendPlayerIdentificationMessage(ip, clients[i].id);
	    return;
	}
	if (i == MAX_CLIENTS - 1)
	{
	    SHH::Log::Warning("NetworkController::Server InsertClientIfNew(): No more room for clients.");
	    return;
	}
    }
    clients[numclients].ip = ip;
    clients[numclients].id = playerid;
    clients[numclients].lastupdate = std::chrono::steady_clock::now();

    SendPlayerIdentificationMessage(ip, playerid);

    ++playerid;
    ++numclients;
}

static void DeleteClientIfExists(ipaddr const& ip)
{
    for (int i = 0; i < numclients; ++i)
    {
	if (clients[i].ip.Equals(ip))
	{
	    message_ctrl dcmsg;
	    SHH::Units::CreateNoneControlMessage(dcmsg);
	    dcmsg.messagetype = MC_DISCONNECT;
	    dcmsg.sender = clients[i].id;
	    SHH::MessageHandler::PushIncomingControlMessage(dcmsg);
	    clients[i].ip.Reset();
	    clients[i].id = 0;
	    clients[i] = clients[numclients - 1];
	    --numclients;
	    return;
	}
    }
}

static int ClientId(ipaddr const& ip)
{
    for (int i = 0; i < numclients; ++i)
    {
	if (clients[i].ip.Equals(ip))
	{
	    return clients[i].id;
	}
    }
    return 0;
}

static int ClientListId(unsigned int uid)
{
    for (int i = 0; i < numclients; ++i)
    {
	if (clients[i].id == uid)
	{
	    return i;
	}
    }
    return -1;
}

static void HandleIncomingMessage(std::string const& msg, ipaddr const& sender)
{
    if (msg.substr(0,3) == "nxc")
    {
	InsertClientIfNew(sender);
	return;
    }
    else if (msg.substr(0,3) == "nxd")
    {
	DeleteClientIfExists(sender);
	return;
    }

    int clientid = 0;
    if ((clientid = ClientId(sender)) != 0)
    {
	int listid = ClientListId(clientid);
	clients[listid].lastupdate = std::chrono::steady_clock::now();
	message_ctrl cmsg;
	cmsg = SHH::Units::DeserializeCtrlMessage(msg);
	cmsg.sender = clientid;
	SHH::MessageHandler::PushIncomingControlMessage(cmsg);
    }
}

static void SendPlayerIdentificationMessage(ipaddr const& ip, unsigned int id)
{
    message_sim playerIdMessage;
    playerIdMessage.messagetype = MS_PLAYERIDENTIFICATION;
    playerIdMessage.obj.owner = id;
    SHH::UDP::Send(SHH::Units::SerializeSimMessage(playerIdMessage), ip);   
}

static void CheckForClientInactivity()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    for (int i = 0; i < numclients; ++i)
    {
	if (std::chrono::duration_cast<std::chrono::duration<double>>(now - clients[i].lastupdate).count() >= MAX_INACTIVITY_TIME)
	{
	    message_ctrl dcmsg;
	    SHH::Units::CreateNoneControlMessage(dcmsg);
	    dcmsg.messagetype = MC_DISCONNECT;
	    dcmsg.sender = clients[i].id;
	    SHH::MessageHandler::PushIncomingControlMessage(dcmsg);
	    clients[i].ip.Reset();
	    clients[i].id = 0;
	    clients[i] = clients[numclients - 1];
	    --numclients;
	    return;
	}
    }
}
