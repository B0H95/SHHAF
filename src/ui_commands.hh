#pragma once

#include <cstdint>
#include <string>

namespace SHH
{
    namespace UI
    {
	namespace Commands
	{
	    bool SetLocalPort(uint32_t portnumber);
	    bool StartServer();
	    bool Connect(std::string serverip, uint32_t serverport);
	    bool Disconnect();
	    bool Map(std::string mapname);
	    void GetMessagingMode();
	}
    }
}
