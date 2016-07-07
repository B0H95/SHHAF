#include "ui_components_menuoverlay.hh"

#include "ui_commands.hh"

#include "log.hh"
#include "window.hh"
#include "programcontroller.hh"
#include "util.hh"

static void JoinPressed(void* data);
static void HostPressed(void* data);
static void QuitPressed(void* data);
static void ConnectPressed(void* data);
static void StartServerPressed(void* data);

bool SHH::UI::Components::MenuOverlay::Init()
{
    if (!titletext.Init() || !menulist.Init() || !serverip.Init() || !serverport.Init() || !joinmenulist.Init() || !hostmenulist.Init() || !mapname.Init())
    {
	SHH::Log::Error("UI::Components::MenuOverlay::Init(): Could not init components.");
	return false;
    }

    menulist.AddMenuOption("Join game", JoinPressed, (void*)&joingroup);
    menulist.AddMenuOption("Host game", HostPressed, (void*)&hostgroup);
    menulist.AddMenuOption("Quit", QuitPressed, nullptr);
    menulist.SetPosition(40, 200);
    menulist.SetFocus(true);

    joinmenulist.AddMenuOption("IP:", nullptr, nullptr);
    joinmenulist.AddMenuOption("Port:", nullptr, nullptr);
    joinmenulist.AddMenuOption("Connect", ConnectPressed, (void*)serveraddress);
    joinmenulist.SetPosition(250, 200);
    joinmenulist.SetFocus(true);

    hostmenulist.AddMenuOption("Map:", nullptr, nullptr);
    hostmenulist.AddMenuOption("Port:", nullptr, nullptr);
    hostmenulist.AddMenuOption("Start server", StartServerPressed, (void*)serverconfig);
    hostmenulist.SetPosition(250, 200);
    hostmenulist.SetFocus(true);

    serverip.SetFocus(true);
    serverip.SetPosition(300, 200);

    serverport.SetFocus(false);
    serverport.SetPosition(328, 236);

    mapname.SetFocus(true);
    mapname.SetPosition(314, 200);

    titletext.SetPosition(20, 100);
    titletext.SetFontCharSize(14, 24);
    titletext.SetText("\"Practice, practice, practice, ...\", Fatal1ty");
    
    visible = true;
    joingroup = false;
    joingroupselection = 0;
    serveraddress[0] = "";
    serveraddress[1] = "";
    serverconfig[0] = "";
    serverconfig[1] = "";
    return true;
}

void SHH::UI::Components::MenuOverlay::Deinit()
{
    titletext.Deinit();
    menulist.Deinit();
}

void SHH::UI::Components::MenuOverlay::ProcessInputs()
{
    if (SHH::Window::IsKeyPressed("Escape"))
    {
	visible = !visible;
	menulist.SetFocus(visible);
    }
    if (joingroup)
    {
	menulist.SetFocus(false);
	
	joinmenulist.ProcessInputs();
	
	serverip.SetFocus(joingroupselection == 0);
	serverip.ProcessInputs();
	serveraddress[0] = serverip.GetString();

	serverport.SetFocus(joingroupselection == 1);
	serverport.ProcessInputs();
	serveraddress[1] = serverport.GetString();

	if (SHH::Window::IsKeyPressed("Up"))
	{
	    unsigned int size = joinmenulist.Size();
	    joingroupselection = (joingroupselection + size - 1) % size;
	}
	else if (SHH::Window::IsKeyPressed("Down"))
	{
	    joingroupselection = (joingroupselection + 1) % joinmenulist.Size();
	}
	else if (SHH::Window::IsKeyPressed("Left"))
	{
	    menulist.SetFocus(true);
	    joingroup = false;
	}
	
	return;
    }
    else if (hostgroup)
    {
	menulist.SetFocus(false);
	
	hostmenulist.ProcessInputs();
	
	mapname.SetFocus(hostgroupselection == 0);
	mapname.ProcessInputs();
	serverconfig[0] = mapname.GetString();

	serverport.SetFocus(hostgroupselection == 1);
	serverport.ProcessInputs();
	serverconfig[1] = serverport.GetString();

	if (SHH::Window::IsKeyPressed("Up"))
	{
	    unsigned int size = hostmenulist.Size();
	    hostgroupselection = (hostgroupselection + size - 1) % size;
	}
	else if (SHH::Window::IsKeyPressed("Down"))
	{
	    hostgroupselection = (hostgroupselection + 1) % joinmenulist.Size();
	}
	else if (SHH::Window::IsKeyPressed("Left"))
	{
	    menulist.SetFocus(true);
	    hostgroup = false;
	}
	
	return;
    }
    menulist.ProcessInputs();
}

void SHH::UI::Components::MenuOverlay::Draw()
{
    if (visible)
    {
	int screenwidth = SHH::Window::GetWindowWidth();
	int screenheight = SHH::Window::GetWindowHeight();
	SHH::Window::SetColor(0x00, 0x00, 0x00, 0xC0);
	SHH::Window::DrawFilledRectangle(0, 0, screenwidth, screenheight);
	titletext.Draw();
	menulist.Draw();

	if (joingroup)
	{
	    joinmenulist.Draw();
	    serverip.Draw();
	    serverport.Draw();
	}
	else if (hostgroup)
	{
	    hostmenulist.Draw();
	    mapname.Draw();
	    serverport.Draw();
	}
    }
}

bool SHH::UI::Components::MenuOverlay::Visible()
{
    return visible;
}

static void JoinPressed(void* data)
{
    *((bool*)data) = true;
}

static void HostPressed(void* data)
{
    *((bool*)data) = true;
}

static void QuitPressed(void* data)
{
    (void)data;
    SHH::ProgramController::Quit();
}

static void ConnectPressed(void* data)
{
    SHH::Log::Log("Connect to: " + ((std::string*)data)[0] + ":" + ((std::string*)data)[1]);
    //SHH::UI::Commands::Connect(((std::string*)data)[0], ((std::string*)data)[1]);
}

static void StartServerPressed(void* data)
{
    if (!SHH::UI::Commands::SetLocalPort(SHH::Util::StringToInt(((std::string*)data)[1])))
    {
	return;
    }
    SHH::Log::Log("Starting server with map " + ((std::string*)data)[0] + " on port " + ((std::string*)data)[1]);
    SHH::UI::Commands::Map(((std::string*)data)[0]);
    SHH::UI::Commands::StartServer();
}
