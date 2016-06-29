#include "simulation_map.hh"

#include "log.hh"

static unsigned int newObjectIndex = 1;

static const unsigned int OBJECT_LIST_SIZE = 100;
static object* objectList = nullptr;
static object* objectBehaviorRequests = nullptr;
static unsigned int objectListSize = 0;

static const unsigned int ENVIRONMENT_LIST_SIZE = 100;
static environment* environmentList = nullptr;
static unsigned int environmentListSize = 0;

//static bool InsertObject(object const& obj);
//static bool InsertEnvironment(environment const& env);

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

    for (unsigned int i = 0; i < OBJECT_LIST_SIZE; ++i)
    {
	SHH::Units::CreateNoneObject(objectList[i]);
    }

    for (unsigned int i = 0; i < ENVIRONMENT_LIST_SIZE; ++i)
    {
	SHH::Units::CreateNoneEnvironment(environmentList[i]);
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

bool SHH::Simulation::Map::InsertObject(object const& obj)
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

bool SHH::Simulation::Map::InsertEnvironment(environment const& env)
{
    if (environmentList == nullptr)
    {
	SHH::Log::Warning("Simulation::Map InsertEnvironment(): No memory allocated for environmentList.");
	return false;
    }

    unsigned int freeIndex;
    for (freeIndex = 0; freeIndex < ENVIRONMENT_LIST_SIZE; ++freeIndex)
    {
	if (environmentList[freeIndex].type == ET_NONE) break;
    }

    if (freeIndex == ENVIRONMENT_LIST_SIZE)
    {
	SHH::Log::Warning("Simulation::Map InsertEnvironment(): No space left in environmentList.");
	return false;
    }

    environmentList[freeIndex] = env;
    ++environmentListSize;
    return true;
}
