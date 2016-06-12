#include "simulation.hh"

#include "log.hh"
#include "programcontroller.hh"
#include "messagehandler.hh"

static const int OBJECT_LIST_SIZE = 100;
static object* objectList = nullptr;
static int objectListSize = 0;

bool SHH::Simulation::Init()
{
    SHH::Log::Log("SHH::Simulation::Init(): Started.");

    objectList = new object [OBJECT_LIST_SIZE];
    if (objectList == nullptr)
    {
	SHH::Log::Error("SHH::Simulation::Init(): Could not allocate memory for objectList.");
	return false;
    }

    for (int i = 0; i < OBJECT_LIST_SIZE; ++i)
    {
	objectList[i].type = OT_NONE;
	objectList[i].state = OS_IDLE;
	objectList[i].direction = OD_NOWHERE;
	objectList[i].x = 0.0f;
	objectList[i].y = 0.0f;
	objectList[i].xspeed = 0.0f;
	objectList[i].yspeed = 0.0f;
	objectList[i].xaccel = 0.0f;
	objectList[i].yaccel = 0.0f;
	objectList[i].width = 0.0f;
	objectList[i].height = 0.0f;
    }

    SHH::Log::Log("SHH::Simulation::Init(): Ended successfully.");
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

    SHH::Log::Log("SHH::Simulation::Deinit(): Ended successfully.");
}

void SHH::Simulation::Update()
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

const object* SHH::Simulation::GetObjectList()
{
    return objectList;
}

int SHH::Simulation::GetObjectListSize()
{
    return objectListSize;
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
