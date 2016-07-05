#include "ui_components_gameinput.hh"

#include "window.hh"
#include "units.hh"
#include "messagehandler.hh"
#include "simulation.hh"

bool SHH::UI::Components::GameInput::Init()
{
    return true;
}

void SHH::UI::Components::GameInput::Deinit()
{
}

void SHH::UI::Components::GameInput::ProcessInputs()
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
    if (SHH::Window::IsKeyPressed("r"))
    {
	message_ctrl msg;
	SHH::Units::CreateNoneControlMessage(msg);
	msg.messagetype = MC_RESPAWN;
	SHH::MessageHandler::PushControlMessage(msg);
    }
}

void SHH::UI::Components::GameInput::Draw()
{
}
