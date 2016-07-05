#include "ui_components_main.hh"

#include "window.hh"
#include "units.hh"
#include "messagehandler.hh"
#include "log.hh"
#include "simulation.hh"

bool SHH::UI::Components::Main::Init()
{
    if (!console.Init())
    {
	SHH::Log::Error("UI::Components::Main::Init(): Could not init console.");
	return false;
    }

    if (!simulationRenderer.Init())
    {
	SHH::Log::Error("UI::Components::Main::Init(): Could not init simulationRenderer.");
	console.Deinit();
	return false;
    }

    if (!performanceMonitor.Init())
    {
	SHH::Log::Error("UI::Components::Main::Init(): Could not init performanceMonitor.");
	simulationRenderer.Deinit();
	console.Deinit();
	return false;
    }

    if (!gameInput.Init())
    {
	SHH::Log::Error("UI::Components::Main::Init(): Could not init gameInput.");
	performanceMonitor.Deinit();
	simulationRenderer.Deinit();
	console.Deinit();
	return false;
    }
    
    return true;
}

void SHH::UI::Components::Main::Deinit()
{
    gameInput.Deinit();
    performanceMonitor.Deinit();
    simulationRenderer.Deinit();
    console.Deinit();
}

void SHH::UI::Components::Main::ProcessInputs()
{
    if (!console.Visible())
    {
	gameInput.ProcessInputs();
    }
    console.ProcessInputs();
    simulationRenderer.ProcessInputs();
    performanceMonitor.ProcessInputs();
}

void SHH::UI::Components::Main::Draw()
{
    gameInput.Draw();
    simulationRenderer.Draw();
    console.Draw();
    performanceMonitor.Draw();
}
