#include "simulation_map.hh"

#include "simulation.hh"
#include "simulation_outputmessages.hh"

#include "log.hh"

static unsigned int newObjectIndex = 1;

static const unsigned int OBJECT_LIST_SIZE = 100;
static object* objectList = nullptr;
static object* objectBehaviorRequests = nullptr;
static unsigned int objectListSize = 0;

static const unsigned int ENVIRONMENT_LIST_SIZE = 100;
static environment* environmentList = nullptr;
static unsigned int environmentListSize = 0;

static const unsigned int CTRL_MESSAGE_LIST_SIZE = 100;
static message_ctrl** ctrlMessageList = nullptr;
static unsigned int ctrlMessageListSize = 0;

static const unsigned int SIM_MESSAGE_LIST_SIZE = 100;
static message_sim** simMessageList = nullptr;
static unsigned int simMessageListSize = 0;

static bool InsertObject(object const& obj);
static bool InsertEnvironment(environment const& env);
static bool InsertUnsynchedObject(object const& obj);
static void DeleteObject(unsigned int listindex);
static void DeleteObjectBySyncindex(unsigned int syncindex);
static void DisconnectPlayer(unsigned int ownerid);

bool SHH::Simulation::Map::Init()
{
    SHH::Log::Log("Simulation::Map::Init(): Started.");

    objectList = new object [OBJECT_LIST_SIZE];
    if (objectList == nullptr)
    {
	SHH::Log::Error("Simulation::Map::Init(): Could not allocate memory for objectList.");
	return false;
    }

    environmentList = new environment [ENVIRONMENT_LIST_SIZE]; 
    if (environmentList == nullptr)
    {
	SHH::Log::Error("Simulation::Map::Init(): Could not allocate memory for environmentList.");
	delete[] objectList;
	objectList = nullptr;
	return false;
    }

    objectBehaviorRequests = new object [OBJECT_LIST_SIZE]; 
    if (objectBehaviorRequests == nullptr)
    {
	SHH::Log::Error("Simulation::Map::Init(): Could not allocate memory for objectBehaviorRequests.");
	delete[] environmentList;
	environmentList = nullptr;
	delete[] objectList;
	objectList = nullptr;
	return false;
    }

    ctrlMessageList = new message_ctrl* [CTRL_MESSAGE_LIST_SIZE];
    if (ctrlMessageList == nullptr)
    {
	SHH::Log::Error("Simulation::Map::Init(): Could not allocate memory for ctrlMessageList.");
	delete[] objectBehaviorRequests;
	objectBehaviorRequests = nullptr;
	delete[] environmentList;
	environmentList = nullptr;
	delete[] objectList;
	objectList = nullptr;
	return false;
    }

    simMessageList = new message_sim* [SIM_MESSAGE_LIST_SIZE];
    if (simMessageList == nullptr)
    {
	SHH::Log::Error("Simulation::Map::Init(): Could not allocate memory for simMessageList.");
	delete[] ctrlMessageList;
	ctrlMessageList = nullptr;
	delete[] objectBehaviorRequests;
	objectBehaviorRequests = nullptr;
	delete[] environmentList;
	environmentList = nullptr;
	delete[] objectList;
	objectList = nullptr;
	return false;
    }

    for (unsigned int i = 0; i < OBJECT_LIST_SIZE; ++i)
    {
	SHH::Units::CreateNoneObject(objectList[i]);
    }

    for (unsigned int i = 0; i < ENVIRONMENT_LIST_SIZE; ++i)
    {
	SHH::Units::CreateNoneEnvironment(environmentList[i]);
    }

    for (unsigned int i = 0; i < CTRL_MESSAGE_LIST_SIZE; ++i)
    {
	ctrlMessageList[i] = nullptr;
    }

    for (unsigned int i = 0; i < SIM_MESSAGE_LIST_SIZE; ++i)
    {
	simMessageList[i] = nullptr;
    }

    SHH::Log::Log("Simulation::Map::Init(): Ended successfully.");
    return true;
}

void SHH::Simulation::Map::Deinit()
{
    SHH::Log::Log("Simulation::Map::Deinit(): Started.");

    if (objectList != nullptr)
    {
	delete[] objectList;
	objectList = nullptr;
    }

    if (objectBehaviorRequests != nullptr)
    {
	delete[] objectBehaviorRequests;
	objectBehaviorRequests = nullptr;
    }

    if (environmentList != nullptr)
    {
	delete[] environmentList;
	environmentList = nullptr;
    }

    if (ctrlMessageList != nullptr)
    {
	delete[] ctrlMessageList;
	ctrlMessageList = nullptr;
    }

    if (simMessageList != nullptr)
    {
	delete[] simMessageList;
	simMessageList = nullptr;
    }

    SHH::Log::Log("Simulation::Map::Deinit(): Ended successfully.");
}

object* SHH::Simulation::Map::GetObjectList()
{
    return objectList;
}

object* SHH::Simulation::Map::GetObjectBehaviorRequests()
{
    return objectBehaviorRequests;
}

unsigned int SHH::Simulation::Map::GetObjectListSize()
{
    return objectListSize;
}

environment* SHH::Simulation::Map::GetEnvironmentList()
{
    return environmentList;
}

unsigned int SHH::Simulation::Map::GetEnvironmentListSize()
{
    return environmentListSize;
}

bool SHH::Simulation::Map::LoadMap(std::string mapname)
{
    SHH::Log::Warning("Simulation::Map::LoadMap(\"" + mapname + "\"): To be implemented, loading default instead.");
    SHH::Simulation::Map::FlushEnvironments();
    SHH::Simulation::Map::FlushObjects();
    InsertEnvironment(SHH::Units::CreateSolidEnvironment(50.0f,500.0f,700.0f,50.0f));
    InsertEnvironment(SHH::Units::CreateSolidEnvironment(50.0f,450.0f,150.0f,50.0f));
    InsertEnvironment(SHH::Units::CreateSolidEnvironment(400.0f,490.0f,300.0f,10.0f));
    InsertObject(SHH::Units::CreatePlayerObject(400.0f,300.0f,32.0f,32.0f));
    return true;
}

void SHH::Simulation::Map::FlushEnvironments()
{
    for (unsigned int i = 0; i < environmentListSize; ++i)
    {
	environmentList[i].type = ET_NONE;
    }
    environmentListSize = 0;
}

void SHH::Simulation::Map::FlushObjects()
{
    for (unsigned int i = 0; i < objectListSize; ++i)
    {
	objectList[i].type = OT_NONE;
    }
    objectListSize = 0;
}

bool SHH::Simulation::Map::PushControlMessage(message_ctrl* msg)
{
    if (ctrlMessageListSize >= CTRL_MESSAGE_LIST_SIZE)
    {
	SHH::Log::Warning("Simulation::Map::PushControlMessage(): No more room for messages.");
	return false;
    }
    ctrlMessageList[ctrlMessageListSize++] = msg;
    return true;
}

bool SHH::Simulation::Map::PushSimulationMessage(message_sim* msg)
{
    if (simMessageListSize >= SIM_MESSAGE_LIST_SIZE)
    {
	SHH::Log::Warning("Simulation::Map::PushSimulationMessage(): No more room for messages.");
	return false;
    }
    simMessageList[simMessageListSize++] = msg;
    return true;
}

void SHH::Simulation::Map::HandleMessages()
{
    for (unsigned int i = 0; i < ctrlMessageListSize; ++i)
    {
	message_ctrl* msg = ctrlMessageList[i];
	if (msg->messagetype == MC_RESPAWN)
	{
	    object obj = SHH::Units::CreatePlayerObject(200.0f, 100.0f, 32.0f, 32.0f); //TODO: Add spawn points maybe...
	    obj.owner = msg->sender;
	    InsertObject(obj);
	}
	else if (msg->messagetype == MC_DISCONNECT && SHH::Simulation::GetMessagingMode() == MM_SERVER)
	{
	    DisconnectPlayer((unsigned int)msg->sender);
	}
    }

    for (unsigned int i = 0; i < simMessageListSize; ++i)
    {
	message_sim* msg = simMessageList[i];
	if (msg->messagetype == MS_OBJECTUPDATE)
	{
	    bool objectFound = false;
	    for (unsigned int i = 0; i < objectListSize; ++i)
	    {
		if (objectList[i].syncindex == msg->obj.syncindex)
		{
		    objectFound = true;
		    objectList[i] = msg->obj; //TODO: Fix interpolation
		    break;
		}
	    }
	    
	    if (!objectFound)
	    {
		InsertUnsynchedObject(msg->obj);
	    }
	}
	else if (msg->messagetype == MS_DELETEOBJECT)
	{
	    DeleteObjectBySyncindex(msg->obj.syncindex);
	}
    }

    ctrlMessageListSize = 0;
    simMessageListSize = 0;
}

bool InsertObject(object const& obj)
{
    if (objectList == nullptr)
    {
	SHH::Log::Warning("Simulation::Map InsertObject(): No memory allocated for objectList.");
	return false;
    }

    if (objectListSize >= OBJECT_LIST_SIZE)
    {
	SHH::Log::Warning("Simulation::Map InsertObject(): No space left in objectList.");
	return false;
    }

    objectList[objectListSize] = obj;
    objectList[objectListSize].syncindex = newObjectIndex++;
    ++objectListSize;
    return true;
}

bool InsertEnvironment(environment const& env)
{
    if (environmentList == nullptr)
    {
	SHH::Log::Warning("Simulation::Map InsertEnvironment(): No memory allocated for environmentList.");
	return false;
    }

    if (environmentListSize >= ENVIRONMENT_LIST_SIZE)
    {
	SHH::Log::Warning("Simulation::Map InsertEnvironment(): No space left in environmentList.");
	return false;
    }

    environmentList[environmentListSize] = env;
    ++environmentListSize;
    return true;
}

static bool InsertUnsynchedObject(object const& obj)
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
}

static void DeleteObject(unsigned int listindex)
{
    if (listindex >= objectListSize || objectListSize == 0)
    {
	return;
    }
    objectListSize--;
    objectList[listindex] = objectList[objectListSize];
}

static void DeleteObjectBySyncindex(unsigned int syncindex)
{
    for (unsigned int i = 0; i < objectListSize; ++i)
    {
	if (objectList[i].syncindex == syncindex)
	{
	    objectListSize--;
	    objectList[i] = objectList[objectListSize];
	    return;
	}
    }
}

static void DisconnectPlayer(unsigned int ownerid)
{
    for (unsigned int i = 0; i < objectListSize; ++i)
    {
	if (objectList[i].type == OT_PLAYER && objectList[i].owner == ownerid)
	{
	    message_sim msg;
	    msg.messagetype = MS_DELETEOBJECT;
	    msg.obj = objectList[i];
	    SHH::Simulation::OutputMessages::PushSimMessage(msg);
	    DeleteObject(i);
	    return;
	}
    }
}
