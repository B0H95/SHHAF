#pragma once

#include <string>

namespace SHH
{
    namespace UI
    {
	bool Init();
	void Deinit();
	void Draw();
	void ProcessInputs();
	void SendMessage(std::string msg);
    }
}
