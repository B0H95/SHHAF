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
	unsigned int GetPlayerId();
    }
}
