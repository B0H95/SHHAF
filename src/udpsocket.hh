#pragma once

#include <string>
#include <cstdint>

namespace SHH
{
    namespace UDP
    {
	bool Init(unsigned int packetsize);
	void Deinit();
	bool Open(uint32_t portnumber);
	int Send(std::string data, std::string destination, uint32_t portnumber);
	std::string Recv(std::string from, uint32_t portnumber);
    }
}
