#include "ui_components_simulationrenderer.hh"

#include "window.hh"
#include "units.hh"
#include "log.hh"
#include "simulation.hh"

bool SHH::UI::Components::SimulationRenderer::Init()
{
    if (!SHH::Window::LoadTexture("res/textures/example1.png"))
    {
	SHH::Log::Error("UI::Components::SimulationRenderer::Init(): Could not load example1.png.");
	return false;
    }
    return true;
}

void SHH::UI::Components::SimulationRenderer::Deinit()
{
}

void SHH::UI::Components::SimulationRenderer::ProcessInputs()
{
}

void SHH::UI::Components::SimulationRenderer::Draw()
{
    const object* objectList = SHH::Simulation::GetObjectList();
    int objectListSize = SHH::Simulation::GetObjectListSize();
    unsigned int playerId = SHH::Simulation::GetPlayerId();

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
		if (objectList[i].owner == playerId)
		{
		    SHH::Window::DrawTexture("res/textures/example1.png", x1, y1, x2-x1, y2-y1);
		    continue;
		}
		else
		{
		    SHH::Window::SetColor(0xFF, 0x00, 0x00, 0xFF);
		}
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
