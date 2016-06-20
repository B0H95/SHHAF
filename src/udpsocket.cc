#include "udpsocket.hh"

#include <SDL2/SDL_net.h>
#include "log.hh"

static UDPsocket socket;
static UDPpacket* packet = nullptr;
static IPaddress ip;
static unsigned int psize;

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
    SDLNet_FreePacket(packet);
    SDLNet_Quit();
    SHH::Log::Log("UDP::Deinit(): Ended successfully.");
}

bool SHH::UDP::Open(uint32_t portnumber)
{
    socket = SDLNet_UDP_Open(portnumber);
    if (socket == nullptr)
    {
	SHH::Log::Error("UDP::Open(): Could not open port " + std::to_string(portnumber) + ".");
	return false;
    }
    return true;
}

int SHH::UDP::Send(std::string data, std::string destination, uint32_t portnumber)
{
    if (data.length() > psize)
    {
	SHH::Log::Error("UDP::Send(): Tried to send a packet larger than " + std::to_string(psize) + " bytes.");
	return 0;
    }
    if (SDLNet_ResolveHost(&ip, destination.c_str(), portnumber) == -1)
    {
	SHH::Log::Error("UDP::Send(): SDLNet_ResolveHost error.");
	return 0;
    }
    packet->address.host = ip.host;
    packet->address.port = ip.port;
    memcpy(packet->data, data.c_str(), data.length());
    packet->len = data.length();
    return SDLNet_UDP_Send(socket, -1, packet);
}

std::string SHH::UDP::Recv(std::string from, uint32_t portnumber)
{
    if (SDLNet_ResolveHost(&ip, from != "" ? from.c_str() : nullptr, portnumber) == -1)
    {
	SHH::Log::Error("UDP::Recv(): SDLNet_ResolveHost error.");
	return "";
    }
    if (SDLNet_UDP_Recv(socket, packet))
    {
	return std::string((char*)(packet->data), packet->len);
    }
    return "";
}
