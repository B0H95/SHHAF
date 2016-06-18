#include "ui.hh"

#include <string>
#include "log.hh"
#include "window.hh"
#include "programcontroller.hh"
#include "simulation.hh"
#include "units.hh"
#include "messagehandler.hh"

static void drawFrameLoadBar();
static void drawSimulation();
static void drawConsole();

bool SHH::UI::Init()
{
    SHH::Log::Log("SHH::UI::Init(): Started.");

    if (!SHH::Window::LoadFont("res/fonts/default.ttf", 12))
    {
	SHH::Log::Error("SHH::UI::Init(): Could not load default font.");
	return false;
    }

    SHH::Log::Log("SHH::UI::Init(): Ended successfully.");
    return true;
}

void SHH::UI::Deinit()
{
    SHH::Log::Log("SHH::UI::Deinit(): Started.");
    SHH::Log::Log("SHH::UI::Deinit(): Ended successfully.");
}

void SHH::UI::Draw()
{
    SHH::Window::SetColor(0x00, 0x00, 0x00, 0xFF);
    SHH::Window::ClearScreen();

    drawSimulation();
    drawFrameLoadBar();
    drawConsole();

    SHH::Window::UpdateScreen();
}

void SHH::UI::ProcessInputs()
{
    message_ctrl msg;
    msg.sender = 0;
    msg.strsize = 0;
    msg.str = nullptr;

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
}

static void drawFrameLoadBar()
{
    SHH::Window::SetColor(0x00, 0xFF, 0x00, 0xFF);
    int frameLoadBarWidth = int((float)SHH::Window::GetWindowWidth() * SHH::ProgramController::GetFrameLoad());
    SHH::Window::DrawFilledRectangle(0, 0, frameLoadBarWidth, 32);
}

static void drawSimulation()
{
    const object* objectList = SHH::Simulation::GetObjectList();
    int objectListSize = SHH::Simulation::GetObjectListSize();

    for (int i = 0; i < objectListSize; ++i)
    {
	if (objectList[i].type != OT_NONE)
	{
	    SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);

	    int x1 = (int)objectList[i].x;
	    int y1 = (int)objectList[i].y;
	    int x2 = int(objectList[i].x + objectList[i].width);
	    int y2 = int(objectList[i].y + objectList[i].height);
	    if (objectList[i].type == OT_PLAYER)
	    {
		SHH::Window::SetColor(0x00, 0xFF, 0x00, 0xFF);
	    }
	    SHH::Window::DrawRectangle(x1, y1, x2, y2);
	}
    }

    const environment* environmentList = SHH::Simulation::GetEnvironmentList();
    int environmentListSize = SHH::Simulation::GetEnvironmentListSize();

    SHH::Window::SetColor(0x7F, 0x7F, 0x7F, 0xFF);

    for (int i = 0; i < environmentListSize; ++i)
    {
	if (environmentList[i].type != ET_NONE)
	{
	    int x1 = (int)environmentList[i].x;
	    int y1 = (int)environmentList[i].y;
	    int x2 = int(environmentList[i].x + environmentList[i].width);
	    int y2 = int(environmentList[i].y + environmentList[i].height);
	    SHH::Window::DrawFilledRectangle(x1, y1, x2, y2);
	}
    }
}

static void drawConsole()
{
    const int fontHeight = 16;
    const int amountOfLines = SHH::Window::GetWindowHeight() / (fontHeight * 2);

    SHH::Window::SetFontCharSize(8, fontHeight);
    SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);

    for (int i = 0; i < amountOfLines; ++i)
    {
	SHH::Window::DrawText(SHH::Log::GetLogEntry(amountOfLines - i - 1), 0, i * fontHeight);
    }
}
