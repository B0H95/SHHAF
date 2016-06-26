#pragma once

namespace SHH
{
    namespace NetworkController
    {
	namespace Server
	{
	    bool Init();
	    void Deinit();
	    void Reset();
	    void HandleMessages();
	}
    }
}
