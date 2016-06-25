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
	void SetClientPort(uint32_t portnumber);
	void SetServerPort(uint32_t portnumber);
	void SetServerAddress(std::string ipaddress);
	bool SetupConnection();
	void SetMessagingMode(messaging_mode mmode);
    }
}
