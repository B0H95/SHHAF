#pragma once

#include <string>

namespace SHH
{
    namespace UI
    {
	namespace Components
	{
	    class Console
	    {
	    public:
		bool Init();
		void Deinit();
		void ProcessInputs();
		void Draw();

	    private:
		bool visible;
		std::string input;
		int scroll;
	    };
	}
    }
}
