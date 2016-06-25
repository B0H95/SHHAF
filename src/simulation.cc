#include "simulation.hh"

#include "simulation_behavior.hh"
#include "simulation_physics.hh"

#include "log.hh"
#include "programcontroller.hh"
#include "messagehandler.hh"

static int newObjectIndex = 1;
static messaging_mode messagingMode = MM_OFFLINE;

static const int OBJECT_LIST_SIZE = 100;
static object* objectList = nullptr;
static object* objectBehaviorRequests = nullptr;
static int objectListSize = 0;

static const int ENVIRONMENT_LIST_SIZE = 100;
static environment* environmentList = nullptr;
static int environmentListSize = 0;

static void distributeIncomingControlMessages();
static void updateApplyBehaviors();
static void updateApplyPhysics();
static void handleIncomingSimulationMessages();
static bool insertUnsynchedObject(object const& obj);
static void sendObjectUpdates();

bool SHH::Simulation::Init()
{
    SHH::Log::Log("Simulation::Init(): Started.");

    objectList = new object [OBJECT_LIST_SIZE];
    if (objectList == nullptr)
    {
	SHH::Log::Error("Simulation::Init(): Could not allocate memory for objectList.");
	return false;
    }

    environmentList = new environment [ENVIRONMENT_LIST_SIZE]; 
    if (environmentList == nullptr)
    {
	SHH::Log::Error("Simulation::Init(): Could not allocate memory for environmentList.");
	delete[] objectList;
	objectList = nullptr;
	return false;
    }

    if (!SHH::Simulation::Behavior::Init())
    {
	SHH::Log::Error("Simulation::Init(): Could not init behaviors.");
	delete[] environmentList;
	environmentList = nullptr;
	delete[] objectList;
	objectList = nullptr;
	return false;
    }

    if (!SHH::Simulation::Physics::Init())
    {
	SHH::Log::Error("Simulation::Init(): Could not init physics.");
	SHH::Simulation::Behavior::Deinit();
	delete[] environmentList;
	environmentList = nullptr;
	delete[] objectList;
	objectList = nullptr;
	return false;
    }

    objectBehaviorRequests = new object [OBJECT_LIST_SIZE]; 
    if (objectBehaviorRequests == nullptr)
    {
	SHH::Log::Error("Simulation::Init(): Could not allocate memory for objectBehaviorRequests.");
	SHH::Simulation::Physics::Deinit();
	SHH::Simulation::Behavior::Deinit();
	delete[] environmentList;
	environmentList = nullptr;
	delete[] objectList;
	objectList = nullptr;
	return false;
    }

    for (int i = 0; i < OBJECT_LIST_SIZE; ++i)
    {
	SHH::Units::CreateNoneObject(objectList[i]);
    }

    for (int i = 0; i < ENVIRONMENT_LIST_SIZE; ++i)
    {
	SHH::Units::CreateNoneEnvironment(environmentList[i]);
    }

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
    return objectList;
}

int SHH::Simulation::GetObjectListSize()
{
    return objectListSize;
}

const environment* SHH::Simulation::GetEnvironmentList()
{
    return environmentList;
}

int SHH::Simulation::GetEnvironmentListSize()
{
    return environmentListSize;
}

bool SHH::Simulation::InsertObject(object const& obj)
{
    if (objectList == nullptr)
    {
	SHH::Log::Warning("Simulation::InsertObject(): No memory allocated for objectList.");
	return false;
    }

    if (objectListSize >= OBJECT_LIST_SIZE)
    {
	SHH::Log::Warning("Simulation::InsertObject(): No space left in objectList.");
	return false;
    }

    if (messagingMode == MM_CLIENT)
    {
	SHH::Log::Warning("Simulation::InsertObject(): Can not add objects in client mode.");
	return false;	
    }

    objectList[objectListSize] = obj;
    objectList[objectListSize].syncindex = newObjectIndex++;
    ++objectListSize;
    return true;
}

bool SHH::Simulation::InsertEnvironment(environment const& env)
{
    if (environmentList == nullptr)
    {
	SHH::Log::Warning("Simulation::InsertEnvironment(): No memory allocated for environmentList.");
	return false;
    }

    int freeIndex;
    for (freeIndex = 0; freeIndex < ENVIRONMENT_LIST_SIZE; ++freeIndex)
    {
	if (environmentList[freeIndex].type == ET_NONE) break;
    }

    if (freeIndex == ENVIRONMENT_LIST_SIZE)
    {
	SHH::Log::Warning("Simulation::InsertEnvironment(): No space left in environmentList.");
	return false;
    }

    environmentList[freeIndex] = env;
    ++environmentListSize;
    return true;
}

bool SHH::Simulation::LoadMap(std::string mapname)
{
    SHH::Log::Warning("Simulation::LoadMap(\"" + mapname + "\"): To be implemented, loading default instead.");
    SHH::Simulation::InsertEnvironment(SHH::Units::CreateSolidEnvironment(50.0f,500.0f,700.0f,50.0f));
    SHH::Simulation::InsertEnvironment(SHH::Units::CreateSolidEnvironment(50.0f,450.0f,150.0f,50.0f));
    SHH::Simulation::InsertEnvironment(SHH::Units::CreateSolidEnvironment(400.0f,490.0f,300.0f,10.0f));
    SHH::Simulation::InsertObject(SHH::Units::CreateBasicObject(100.0f,100.0f,32.0f,32.0f));
    SHH::Simulation::InsertObject(SHH::Units::CreateBasicObject(300.0f,350.0f,32.0f,32.0f));
    SHH::Simulation::InsertObject(SHH::Units::CreateBasicObject(600.0f,450.0f,32.0f,32.0f));
    SHH::Simulation::InsertObject(SHH::Units::CreatePlayerObject(400.0f,300.0f,32.0f,32.0f));
    return true;
}

void SHH::Simulation::SetMessagingMode(messaging_mode sm)
{
    messagingMode = sm;
}

static void distributeIncomingControlMessages()
{
    message_ctrl msg;
    while ((msg = SHH::MessageHandler::PopIncomingControlMessage()).messagetype != MC_NOTHING && msg.messagetype < MC_MARKER_SIM)
    {
	SHH::Simulation::Behavior::PushControlMessage(msg);
    }
    if (messagingMode == MM_SERVER)
    {
	while ((msg = SHH::MessageHandler::PopOutgoingControlMessage()).messagetype != MC_NOTHING && msg.messagetype < MC_MARKER_SIM)
	{
	    SHH::Simulation::Behavior::PushControlMessage(msg);
	}
    }
}

static void updateApplyBehaviors()
{
    SHH::Simulation::Behavior::ApplyBehaviors(objectList, objectBehaviorRequests, objectListSize);
}

static void updateApplyPhysics()
{
    SHH::Simulation::Physics::ApplyPhysics(objectList, objectBehaviorRequests, objectListSize, environmentList, environmentListSize);
}

static void handleIncomingSimulationMessages()
{
    message_sim msg;
    while ((msg = SHH::MessageHandler::PopSimulationMessage()).messagetype != MS_NOTHING)
    {
	//TODO: Fix different actions for different message types (if there will ever be any)
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
	
	if (!objectFound)
	{
	    insertUnsynchedObject(msg.obj);
	}
    }
}

static bool insertUnsynchedObject(object const& obj)
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

static void sendObjectUpdates()
{
    message_sim msg;
    msg.messagetype = MS_OBJECTUPDATE;
    for (int i = 0; i < objectListSize; ++i)
    {
	msg.obj = objectList[i];
	SHH::MessageHandler::PushSimulationMessage(msg);
    }
}
