#pragma once

namespace SHH
{
    namespace UI
    {
	namespace Components
	{
	    class GameInput
	    {
	    public:
		bool Init();
		void Deinit();
		void ProcessInputs();
		void Draw();
	    };
	}
    }
}
