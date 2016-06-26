#include "udpsocket.hh"

#include <iostream>

static UDPsocket socket;
static UDPpacket* packet = nullptr;
static unsigned int psize;

bool SHH::UDP::Init(unsigned int packetsize)
{
    if (SDLNet_Init() == -1)
    {
	return false;
    }
    packet = SDLNet_AllocPacket(packetsize);
    if (packet == nullptr)
    {
	return false;
    }
    psize = packetsize;
    return true;
}

void SHH::UDP::Deinit()
{
    SDLNet_FreePacket(packet);
    SDLNet_Quit();
}

bool SHH::UDP::Open(uint32_t portnumber)
{
    socket = SDLNet_UDP_Open(portnumber);
    if (socket == nullptr)
    {
	return false;
    }
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
