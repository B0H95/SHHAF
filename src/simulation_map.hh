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
	    bool PushControlMessage(message_ctrl* msg);
	    bool PushSimulationMessage(message_sim* msg);
	    void HandleMessages();
	}
    }
}
