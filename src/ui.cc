#include "ui.hh"

#include "ui_commands.hh"
#include "ui_components_console.hh"

#include "log.hh"
#include "window.hh"
#include "programcontroller.hh"
#include "simulation.hh"
#include "units.hh"
#include "messagehandler.hh"
#include "networkcontroller.hh"
#include "util.hh"

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

    if (!SHH::Window::LoadTexture("res/textures/example1.png"))
    {
	SHH::Log::Error("UI:Init(): Could not load default texture.");
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
}

void SHH::UI::SendMessage(std::string msg)
{
    SHH::Log::Log(msg);

    //TODO: Check for parsing errors

    std::size_t separatorpos = msg.find(" ");
    std::string cmd = msg.substr(0, separatorpos);
    msg.erase(0, separatorpos + 1);
    std::string param = msg;

    static uint32_t serverport = 0;

    if (cmd == "LOCALPORT")
    {
	SHH::UI::Commands::SetLocalPort(SHH::Util::StringToInt(param));
    }
    else if (cmd == "SERVERPORT")
    {
	serverport = SHH::Util::StringToInt(param);
    }
    else if (cmd == "STARTSERVER")
    {
	SHH::UI::Commands::StartServer();
    }
    else if (cmd == "CONNECT")
    {
	if (serverport == 0)
	{
	    SHH::Log::Warning("UI::SendMessage(): Must set SERVERPORT first.");
	    return;
	}
	SHH::UI::Commands::Connect(param, serverport);
    }
    else if (cmd == "MAP")
    {
	SHH::UI::Commands::Map(param);
    }
    else if (cmd == "DISCONNECT")
    {
	SHH::UI::Commands::Disconnect();
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
		    //SHH::Window::SetColor(0x00, 0xFF, 0x00, 0xFF);
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
