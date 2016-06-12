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
	bool InsertObject(object const& obj);
	bool InsertEnvironment(environment const& obj);
	bool LoadMap(std::string mapname);
    }
}
