#pragma once

#include "units.hh"

namespace SHH
{
    namespace NetworkController
    {
	bool Init();
	void Deinit();
	bool Run();
	void Stop();
    }
}
