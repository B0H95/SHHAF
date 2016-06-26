#pragma once

#include <string>
#include <cstdint>

namespace SHH
{
    namespace NetworkController
    {
	namespace Client
	{
	    bool Init();
	    void Deinit();
	    void Reset();
	    bool ConnectToServer(std::string destination, uint32_t portnumber);
	    void HandleMessages();
	}
    }
}
