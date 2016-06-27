#include "udpsocket.hh"

#include <string>
#include "log.hh"

static UDPsocket socket;
static bool opened = false;
static UDPpacket* packet = nullptr;
static unsigned int psize;

void ipaddr::Reset()
{
    ip.host = 0;
    ip.port = 0;
}

bool ipaddr::InUse()
{
    return ip.host != 0 && ip.port != 0;
}

bool ipaddr::Equals(ipaddr const& other)
{
    return ip.host == other.ip.host && ip.port == other.ip.port; 
}

bool SHH::UDP::Init(unsigned int packetsize)
{
    SHH::Log::Log("UDP::Init(): Started.");

    if (SDLNet_Init() == -1)
    {
	SHH::Log::Error("UDP::Init(): SDLNet_Init error.");
	return false;
    }
    packet = SDLNet_AllocPacket(packetsize);
    if (packet == nullptr)
    {
	SHH::Log::Error("UDP::Init(): SDLNet_AllocPacket error.");
	return false;
    }
    psize = packetsize;

    SHH::Log::Log("UDP::Init(): Ended successfully.");
    return true;
}

void SHH::UDP::Deinit()
{
    SHH::Log::Log("UDP::Deinit(): Started.");

    if (opened)
    {
	SDLNet_UDP_Close(socket);
    }

    SDLNet_FreePacket(packet);
    SDLNet_Quit();
    SHH::Log::Log("UDP::Deinit(): Ended successfully.");
}

bool SHH::UDP::Open(uint32_t portnumber)
{
    SHH::Log::Log("UDP::Open(): Trying to open port " + std::to_string(portnumber) + ".");

    if (opened)
    {
	SDLNet_UDP_Close(socket);
    }

    socket = SDLNet_UDP_Open(portnumber);
    if (socket == nullptr)
    {
	SHH::Log::Error("UDP::Open(): Could not open port.");
	return false;
    }
    opened = true;
    return true;
}

bool SHH::UDP::GetIPAddress(ipaddr* ip, std::string destination, uint32_t portnumber)
{
    return SDLNet_ResolveHost(&(ip->ip), destination.c_str(), portnumber) != -1;
}

int SHH::UDP::Send(std::string data, ipaddr const& ip)
{
    if (data.length() > psize)
    {
	return 0;
    }
    packet->address.host = ip.ip.host;
    packet->address.port = ip.ip.port;
    memcpy(packet->data, data.c_str(), data.length());
    packet->len = data.length();
    return SDLNet_UDP_Send(socket, -1, packet);
}

std::string SHH::UDP::Recv(ipaddr* sender)
{
    if (SDLNet_UDP_Recv(socket, packet))
    {
	sender->ip.host = packet->address.host;
	sender->ip.port = packet->address.port;
	return std::string((char*)(packet->data), packet->len);
    }
    return "";
}
