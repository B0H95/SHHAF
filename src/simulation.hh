#pragma once

#include <string>
#include "units.hh"

namespace SHH
{
    namespace Simulation
    {
	bool Init();
	bool Postinit();
	void Deinit();
	void Update();
	const object* GetObjectList();
	int GetObjectListSize();
	const environment* GetEnvironmentList();
	int GetEnvironmentListSize();
	bool LoadMap(std::string mapname);
	void SetMessagingMode(messaging_mode sm);
	messaging_mode GetMessagingMode();
	unsigned int GetPlayerId();
	void SetPlayerId(unsigned int pid);
	std::string GetMapName();
    }
}
