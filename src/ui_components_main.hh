#pragma once

#include "ui_components_console.hh"
#include "ui_components_simulationrenderer.hh"
#include "ui_components_performancemonitor.hh"

namespace SHH
{
    namespace UI
    {
	namespace Components
	{
	    class Main
	    {
	    public:
		bool Init();
		void Deinit();
		void ProcessInputs();
		void Draw();

	    private:
		SHH::UI::Components::Console console;
		SHH::UI::Components::SimulationRenderer simulationRenderer;
		SHH::UI::Components::PerformanceMonitor performanceMonitor;
	    };
	}
    }
}
