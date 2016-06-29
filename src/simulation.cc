#include "simulation.hh"

#include "simulation_behavior.hh"
#include "simulation_physics.hh"
#include "simulation_map.hh"

#include "log.hh"
#include "programcontroller.hh"
#include "messagehandler.hh"

struct player_info
{
    bool alive;
};

static unsigned int playerId = 0;
static messaging_mode messagingMode = MM_OFFLINE;

static const int PLAYERINFO_LIST_SIZE = 100;
static player_info playerinfoList [PLAYERINFO_LIST_SIZE];

static void distributeIncomingControlMessages();
static void updateApplyBehaviors();
static void updateApplyPhysics();
static void handleIncomingSimulationMessages();
static bool insertUnsynchedObject(object const& obj);
static void sendObjectUpdates();

bool SHH::Simulation::Init()
{
    SHH::Log::Log("Simulation::Init(): Started.");

    if (!SHH::Simulation::Behavior::Init())
    {
	SHH::Log::Error("Simulation::Init(): Could not init behaviors.");
	return false;
    }

    if (!SHH::Simulation::Physics::Init())
    {
	SHH::Log::Error("Simulation::Init(): Could not init physics.");
	SHH::Simulation::Behavior::Deinit();
	return false;
    }

    if (!SHH::Simulation::Map::Init())
    {
	SHH::Log::Error("Simulation::Init(): Could not init map.");
	SHH::Simulation::Physics::Deinit();
	SHH::Simulation::Behavior::Deinit();
	return false;
    }

    for (int i = 0; i < PLAYERINFO_LIST_SIZE; ++i)
    {
	playerinfoList[i].alive = false;
    }
    playerinfoList[0].alive = true;

    SHH::Log::Log("Simulation::Init(): Ended successfully.");
    return true;
}

bool SHH::Simulation::Postinit()
{
    SHH::Simulation::LoadMap("de_dust2");
    return true;
}

void SHH::Simulation::Deinit()
{
    SHH::Log::Log("Simulation::Deinit(): Started.");

    SHH::Simulation::Map::Deinit();
    SHH::Simulation::Physics::Deinit();
    SHH::Simulation::Behavior::Deinit();

    SHH::Log::Log("Simulation::Deinit(): Ended successfully.");
}

void SHH::Simulation::Update()
{
    distributeIncomingControlMessages();
    if (messagingMode == MM_CLIENT)
    {
	handleIncomingSimulationMessages();
    }
    updateApplyBehaviors();
    updateApplyPhysics();
    if (messagingMode == MM_SERVER)
    {
	sendObjectUpdates();
    }
}

const object* SHH::Simulation::GetObjectList()
{
    return SHH::Simulation::Map::GetObjectList();
}

int SHH::Simulation::GetObjectListSize()
{
    return (int)SHH::Simulation::Map::GetObjectListSize();
}

const environment* SHH::Simulation::GetEnvironmentList()
{
    return SHH::Simulation::Map::GetEnvironmentList();
}

int SHH::Simulation::GetEnvironmentListSize()
{
    return (int)SHH::Simulation::Map::GetEnvironmentListSize();
}

bool SHH::Simulation::LoadMap(std::string mapname)
{
    return SHH::Simulation::Map::LoadMap(mapname);
}

void SHH::Simulation::SetMessagingMode(messaging_mode sm)
{
    messagingMode = sm;
}

unsigned int SHH::Simulation::GetPlayerId()
{
    return playerId;
}

static void distributeIncomingControlMessages()
{
    message_ctrl msg;
    while ((msg = SHH::MessageHandler::PopIncomingControlMessage()).messagetype != MC_NOTHING)
    {
	if (msg.messagetype < MC_MARKER_SIM)
	{
	    SHH::Simulation::Behavior::PushControlMessage(msg);
	}
	else if (msg.messagetype < MC_MARKER_CTRL)
	{
	    if (msg.messagetype == MC_RESPAWN && messagingMode == MM_SERVER)
	    {
		if (playerinfoList[msg.sender].alive) //TODO: Fix a better way to handle player info
		{
		    continue;
		}
		object obj = SHH::Units::CreatePlayerObject(200.0f, 100.0f, 32.0f, 32.0f); //TODO: Add spawn points maybe...
		obj.owner = msg.sender;
		SHH::Simulation::Map::InsertObject(obj);
		playerinfoList[msg.sender].alive = true;
	    }
	    else if (msg.messagetype == MC_DISCONNECT && messagingMode == MM_SERVER)
	    {
		
	    }
	}
    }
}

static void updateApplyBehaviors()
{
    object* objectList = SHH::Simulation::Map::GetObjectList();
    object* objectBehaviorRequests = SHH::Simulation::Map::GetObjectBehaviorRequests();
    int objectListSize = (int)SHH::Simulation::Map::GetObjectListSize();
    SHH::Simulation::Behavior::ApplyBehaviors(objectList, objectBehaviorRequests, objectListSize);
}

static void updateApplyPhysics()
{
    object* objectList = SHH::Simulation::Map::GetObjectList();
    object* objectBehaviorRequests = SHH::Simulation::Map::GetObjectBehaviorRequests();
    int objectListSize = (int)SHH::Simulation::Map::GetObjectListSize();
    environment* environmentList = SHH::Simulation::Map::GetEnvironmentList();
    int environmentListSize = (int)SHH::Simulation::Map::GetEnvironmentListSize();
    SHH::Simulation::Physics::ApplyPhysics(objectList, objectBehaviorRequests, objectListSize, environmentList, environmentListSize);
}

static void handleIncomingSimulationMessages()
{
    message_sim msg;
    object* objectList = SHH::Simulation::Map::GetObjectList();
    int objectListSize = (int)SHH::Simulation::Map::GetObjectListSize();
    while ((msg = SHH::MessageHandler::PopSimulationMessage()).messagetype != MS_NOTHING)
    {
	if (msg.messagetype == MS_OBJECTUPDATE)
	{
	    bool objectFound = false;
	    for (int i = 0; i < objectListSize; ++i)
	    {
		if (objectList[i].syncindex == msg.obj.syncindex)
		{
		    objectFound = true;
		    objectList[i] = msg.obj; //TODO: Fix interpolation
		    break;
		}
	    }
	    
	    /*if (!objectFound)
	    {
		insertUnsynchedObject(msg.obj);
	    }*/
	}
	else if (msg.messagetype == MS_PLAYERIDENTIFICATION)
	{
	    playerId = msg.obj.owner;
	}
    }
}

/*static bool insertUnsynchedObject(object const& obj)
{
    if (objectList == nullptr)
    {
	SHH::Log::Warning("Simulation insertUnsynchedObject(): No memory allocated for objectList.");
	return false;
    }

    if (objectListSize >= OBJECT_LIST_SIZE)
    {
	SHH::Log::Warning("Simulation insertUnsynchedObject(): No space left in objectList.");
	return false;
    }

    objectList[objectListSize] = obj;
    ++objectListSize;
    return true;
}*/

static void sendObjectUpdates()
{
    object* objectList = SHH::Simulation::Map::GetObjectList();
    int objectListSize = (int)SHH::Simulation::Map::GetObjectListSize();
    message_sim msg;
    msg.messagetype = MS_OBJECTUPDATE;
    for (int i = 0; i < objectListSize; ++i)
    {
	msg.obj = objectList[i];
	SHH::MessageHandler::PushSimulationMessage(msg);
    }
}
