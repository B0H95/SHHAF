#pragma once

#include "units.hh"

namespace SHH
{
    namespace Simulation
    {
	namespace OutputMessages
	{
	    bool Init();
	    void Deinit();
	    bool PushSimMessage(message_sim const& msg);
	    void DistributeMessages();
	}
    }
}
