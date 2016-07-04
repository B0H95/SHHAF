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
    
    return true;
}

void SHH::UI::Components::Main::Deinit()
{
    performanceMonitor.Deinit();
    simulationRenderer.Deinit();
    console.Deinit();
}

void SHH::UI::Components::Main::ProcessInputs()
{
    message_ctrl msg;
    msg.sender = SHH::Simulation::GetPlayerId();
    msg.strsize = 0;
    msg.str = "";

    if (SHH::Window::IsKeyDown("left"))
    {
	msg.messagetype = MC_RUNLEFT;
	SHH::MessageHandler::PushControlMessage(msg);
    }
    if (SHH::Window::IsKeyDown("right"))
    {
	msg.messagetype = MC_RUNRIGHT;
	SHH::MessageHandler::PushControlMessage(msg);
    }
    if (SHH::Window::IsKeyDown("up"))
    {
	msg.messagetype = MC_JUMP;
	SHH::MessageHandler::PushControlMessage(msg);
    }
    if (SHH::Window::IsKeyPressed("r") && !console.Visible())
    {
	message_ctrl msg;
	SHH::Units::CreateNoneControlMessage(msg);
	msg.messagetype = MC_RESPAWN;
	SHH::MessageHandler::PushControlMessage(msg);
    }

    console.ProcessInputs();
    simulationRenderer.ProcessInputs();
    performanceMonitor.ProcessInputs();
}

void SHH::UI::Components::Main::Draw()
{
    simulationRenderer.Draw();
    console.Draw();
    performanceMonitor.Draw();
}
