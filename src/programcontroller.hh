#pragma once

namespace SHH
{
    namespace ProgramController
    {
	bool Init(int argc, char* argv[]);
	bool Postinit();
	void Deinit();
	void Run();
	void Quit();
	float GetFrameTime();
	int GetFrameRate();
	float GetFrameLoad();
	bool GetParameter(const char* param);
	void SetFramerate(int rate);
    }
}
