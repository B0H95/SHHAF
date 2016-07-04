#pragma once

namespace SHH
{
    namespace UI
    {
	namespace Components
	{
	    class PerformanceMonitor
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
