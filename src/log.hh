#pragma once

#include <string>

namespace SHH
{
    namespace Log
    {
	bool Init();
	void Deinit();
	void Log(std::string str);
	void Warning(std::string str);
	void Error(std::string str);
	std::string GetLogEntry(int logentry);
    }
}
