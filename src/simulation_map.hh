#pragma once

#include <string>
#include "units.hh"

namespace SHH
{
    namespace Simulation
    {
	namespace Map
	{
	    bool Init();
	    void Deinit();
	    object* GetObjectList();
	    object* GetObjectBehaviorRequests();
	    unsigned int GetObjectListSize();
	    environment* GetEnvironmentList();
	    unsigned int GetEnvironmentListSize();
	    bool LoadMap(std::string mapname);
	    void FlushEnvironments();
	    void FlushObjects();

	    //TEMP
	    bool InsertObject(object const& obj);
	    bool InsertEnvironment(environment const& env);
	}
    }
}
