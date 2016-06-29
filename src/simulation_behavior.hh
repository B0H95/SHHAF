#pragma once

#include "units.hh"

namespace SHH
{
    namespace Simulation
    {
	namespace Behavior
	{
	    bool Init();
	    void Deinit();
	    bool PushControlMessage(message_ctrl* msg);
	    void ApplyBehaviors(object* olist, object* obrlist, int olistsize);
	}
    }
}
