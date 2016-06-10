#include "ui.hh"

#include "log.hh"
#include "window.hh"
#include "programcontroller.hh"
#include "simulation.hh"
#include "units.hh"

static void drawFrameLoadBar();
static void drawSimulation();

bool SHH::UI::Init()
{
    SHH::Log::Log("SHH::UI::Init(): Started.");
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
    
    SHH::Window::UpdateScreen();
}

void SHH::UI::ProcessInputs()
{
    
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

    SHH::Window::SetColor(0xFF, 0xFF, 0xFF, 0xFF);

    for (int i = 0; i < objectListSize; ++i)
    {
	if (objectList[i].type != NONE)
	{
	    int x1 = (int)objectList[i].x;
	    int y1 = (int)objectList[i].y;
	    int x2 = int(objectList[i].x + objectList[i].width);
	    int y2 = int(objectList[i].y + objectList[i].height);
	    SHH::Window::DrawRectangle(x1, y1, x2, y2);
	}
    }
}
