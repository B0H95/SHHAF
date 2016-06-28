#pragma once

#include <cstdint>
#include <string>
#include "units.hh"

namespace SHH
{
    namespace NetworkController
    {
	bool Init();
	void Deinit();
	bool Run();
	void Stop();
	bool OpenPort(uint32_t portnumber);
	bool ConnectToServer(std::string destination, uint32_t portnumber);
	void Disconnect();
	void SetMessagingMode(messaging_mode mmode);
    }
}
