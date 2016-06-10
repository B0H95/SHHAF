#pragma once

#include "units.hh"

namespace SHH
{
    namespace Simulation
    {
	bool Init();
	void Deinit();
	void Update();
	const object* GetObjectList();
	int GetObjectListSize();
	bool InsertObject(object const& obj);
    }
}
