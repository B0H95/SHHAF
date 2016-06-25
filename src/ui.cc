#include "ui.hh"

#include "ui_components_console.hh"

#include "log.hh"
#include "window.hh"
#include "programcontroller.hh"
#include "simulation.hh"
#include "units.hh"
#include "messagehandler.hh"
#include "networkcontroller.hh"

static SHH::UI::Components::Console console;

static void drawFrameLoadBar();
static void drawSimulation();

bool SHH::UI::Init()
{
    SHH::Log::Log("UI::Init(): Started.");

    if (!SHH::Window::LoadFont("res/fonts/default.ttf", 24))
    {
	SHH::Log::Error("UI::Init(): Could not load default font.");
	return false;
    }

    if (!console.Init())
    {
	SHH::Log::Error("UI::Init(): Could not init console.");
	return false;
    }

    SHH::Log::Log("UI::Init(): Ended successfully.");
    return true;
}

void SHH::UI::Deinit()
{
    SHH::Log::Log("UI::Deinit(): Started.");
    SHH::Log::Log("UI::Deinit(): Ended successfully.");
}

void SHH::UI::Draw()
{
    SHH::Window::SetColor(0x00, 0x00, 0x00, 0xFF);
    SHH::Window::ClearScreen();

    drawSimulation();
    drawFrameLoadBar();
    
    console.Draw();

    SHH::Window::UpdateScreen();
}

void SHH::UI::ProcessInputs()
{
    message_ctrl msg;
    msg.sender = 0;
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

    console.ProcessInputs();
}

void SHH::UI::SendMessage(std::string msg)
{
    SHH::Log::Log(msg);

    //TODO: Check for parsing errors

    std::size_t separatorpos = msg.find(" ");
    std::string cmd = msg.substr(0, separatorpos);
    msg.erase(0, separatorpos + 1);
    std::string param = msg;

    if (cmd == "MODE")
    {
	if (param == "OFFLINE")
	{
	    SHH::Simulation::SetMessagingMode(MM_OFFLINE);
	    SHH::MessageHandler::SetMessagingMode(MM_OFFLINE);
	    SHH::NetworkController::SetMessagingMode(MM_OFFLINE);
	}
	else if (param == "CLIENT")
	{
	    SHH::Simulation::SetMessagingMode(MM_CLIENT);
	    SHH::MessageHandler::SetMessagingMode(MM_CLIENT);
	    SHH::NetworkController::SetMessagingMode(MM_CLIENT);
	}
	else if (param == "SERVER")
	{
	    SHH::Simulation::SetMessagingMode(MM_SERVER);
	    SHH::MessageHandler::SetMessagingMode(MM_SERVER);
	    SHH::NetworkController::SetMessagingMode(MM_SERVER);
	}
    }
    else if (cmd == "CLIENTPORT")
    {
	SHH::NetworkController::SetClientPort(std::stoi(param));
    }
    else if (cmd == "SERVERPORT")
    {
	SHH::NetworkController::SetServerPort(std::stoi(param));
    }
    else if (cmd == "SERVERADDRESS")
    {
	SHH::NetworkController::SetServerAddress(param);
    }
    else if (cmd == "SETUPCONNECTION")
    {
	SHH::NetworkController::SetupConnection();
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
		if (objectList[i].owner == 0)
		{
		    SHH::Window::SetColor(0x00, 0xFF, 0x00, 0xFF);
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
