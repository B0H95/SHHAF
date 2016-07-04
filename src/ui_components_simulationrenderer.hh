#pragma once

namespace SHH
{
    namespace UI
    {
	namespace Components
	{
	    class SimulationRenderer
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
