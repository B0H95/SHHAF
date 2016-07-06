#pragma once

#include <string>

namespace SHH
{
    namespace UI
    {
	namespace Components
	{
	    class TextInput
	    {
	    public:
		bool Init();
		void Deinit();
		void ProcessInputs();
		void Draw();
		void SetFocus(bool f);
		void SetPosition(int nx, int ny);
		void OnEnterPressed(void(*callback)(void* data), void* userdata);

	    private:
		int flashtimer;
		int framerate;
		int x;
		int y;
		bool focus;
		std::string inputstr;
		void (*onenter) (void*);
		void* extra;
	    };
	}
    }
}
