#pragma once

#include "units.hh"

namespace SHH
{
    namespace Simulation
    {
	namespace Ruleset
	{
	    bool Init();
	    void Deinit();
	    bool HandleCtrlMessage(message_ctrl const& msg);
	}
    }
}
