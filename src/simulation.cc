#include "simulation.hh"

#include "simulation_behavior.hh"

#include "log.hh"
#include "programcontroller.hh"
#include "messagehandler.hh"

static const int OBJECT_LIST_SIZE = 100;
static object* objectList = nullptr;
static int objectListSize = 0;

static const int ENVIRONMENT_LIST_SIZE = 100;
static environment* environmentList = nullptr;
static int environmentListSize = 0;

static void distributeIncomingMessages();
static void updateApplyBehaviors();
static void updateApplyPhysics();

bool SHH::Simulation::Init()
{
    SHH::Log::Log("SHH::Simulation::Init(): Started.");

    objectList = new object [OBJECT_LIST_SIZE];
    if (objectList == nullptr)
    {
	SHH::Log::Error("SHH::Simulation::Init(): Could not allocate memory for objectList.");
	return false;
    }

    environmentList = new environment [ENVIRONMENT_LIST_SIZE];
    if (environmentList == nullptr)
    {
	SHH::Log::Error("SHH::Simulation::Init(): Could not allocate memory for environmentList.");
	delete[] objectList;
	objectList = nullptr;
	return false;
    }

    if (!SHH::Simulation::Behavior::Init())
    {
	SHH::Log::Error("SHH::Simulation::Init(): Could not init behaviors.");
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

    SHH::Log::Log("SHH::Simulation::Init(): Ended successfully.");
    return true;
}

bool SHH::Simulation::Postinit()
{
    SHH::Simulation::LoadMap("de_dust2");
    return true;
}

void SHH::Simulation::Deinit()
{
    SHH::Log::Log("SHH::Simulation::Deinit(): Started.");

    if (objectList != nullptr)
    {
	delete[] objectList;
	objectList = nullptr;
    }

    if (environmentList != nullptr)
    {
	delete[] environmentList;
	environmentList = nullptr;
    }

    SHH::Simulation::Behavior::Deinit();

    SHH::Log::Log("SHH::Simulation::Deinit(): Ended successfully.");
}

void SHH::Simulation::Update()
{
    distributeIncomingMessages();
    updateApplyBehaviors();
    updateApplyPhysics();
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
	SHH::Log::Warning("SHH::Simulation::InsertObject(): No memory allocated for objectList.");
	return false;
    }

    int freeIndex;
    for (freeIndex = 0; freeIndex < OBJECT_LIST_SIZE; ++freeIndex)
    {
	if (objectList[freeIndex].type == OT_NONE) break;
    }

    if (freeIndex == OBJECT_LIST_SIZE)
    {
	SHH::Log::Warning("SHH::Simulation::InsertObject(): No space left in objectList.");
	return false;
    }

    objectList[freeIndex] = obj;
    ++objectListSize;
    return true;
}

bool SHH::Simulation::InsertEnvironment(environment const& env)
{
    if (environmentList == nullptr)
    {
	SHH::Log::Warning("SHH::Simulation::InsertEnvironment(): No memory allocated for environmentList.");
	return false;
    }

    int freeIndex;
    for (freeIndex = 0; freeIndex < ENVIRONMENT_LIST_SIZE; ++freeIndex)
    {
	if (environmentList[freeIndex].type == ET_NONE) break;
    }

    if (freeIndex == ENVIRONMENT_LIST_SIZE)
    {
	SHH::Log::Warning("SHH::Simulation::InsertEnvironment(): No space left in environmentList.");
	return false;
    }

    environmentList[freeIndex] = env;
    ++environmentListSize;
    return true;
}

bool SHH::Simulation::LoadMap(std::string mapname)
{
    SHH::Log::Warning("SHH::Simulation::LoadMap(\"" + mapname + "\"): To be implemented, loading default instead.");
    SHH::Simulation::InsertEnvironment(SHH::Units::CreateSolidEnvironment(50.0f,500.0f,700.0f,50.0f));
    SHH::Simulation::InsertEnvironment(SHH::Units::CreateSolidEnvironment(50.0f,450.0f,150.0f,50.0f));
    SHH::Simulation::InsertEnvironment(SHH::Units::CreateSolidEnvironment(400.0f,490.0f,300.0f,10.0f));
    SHH::Simulation::InsertObject(SHH::Units::CreateBasicObject(100.0f,100.0f,32.0f,32.0f));
    SHH::Simulation::InsertObject(SHH::Units::CreateBasicObject(300.0f,350.0f,32.0f,32.0f));
    SHH::Simulation::InsertObject(SHH::Units::CreateBasicObject(600.0f,450.0f,32.0f,32.0f));
    SHH::Simulation::InsertObject(SHH::Units::CreatePlayerObject(400.0f,300.0f,32.0f,32.0f));
    return true;
}

static void distributeIncomingMessages()
{
    message_ctrl msg;
    while ((msg = SHH::MessageHandler::PopControlMessage()).messagetype != MC_NOTHING && msg.messagetype < MC_MARKER_SIM)
    {
	SHH::Simulation::Behavior::PushControlMessage(msg);
    }
}

static void updateApplyBehaviors()
{
    SHH::Simulation::Behavior::ApplyBehaviors(objectList, objectListSize);
}

static void updateApplyPhysics()
{
    float frameTime = SHH::ProgramController::GetFrameTime();
    for (int i = 0; i < OBJECT_LIST_SIZE; ++i)
    {
	objectList[i].xspeed += objectList[i].xaccel * frameTime;
	objectList[i].yspeed += objectList[i].yaccel * frameTime;
	objectList[i].x += objectList[i].xspeed * frameTime;
	objectList[i].y += objectList[i].yspeed * frameTime;
    }
}
