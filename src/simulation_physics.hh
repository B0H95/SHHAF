#pragma once

#include "units.hh"

namespace SHH
{
    namespace Simulation
    {
	namespace Physics
	{
	    bool Init();
	    void Deinit();
	    void ApplyPhysics(object* olist, int olistsize, environment* elist, int elistsize);
	}
    }
}
