#include <string>
#include <cstdint>
#include <SDL2/SDL_net.h>

struct ipaddr
{
    IPaddress ip;
};

namespace SHH
{
    namespace UDP
    {
	bool Init(unsigned int packetsize);
	void Deinit();
	bool Open(uint32_t portnumber);
	bool GetIPAddress(ipaddr* ip, std::string destination, uint32_t portnumber);
	int Send(std::string data, ipaddr const& ip);
	std::string Recv(ipaddr* sender);
    }
}
