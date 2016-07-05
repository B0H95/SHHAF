#include "simulation.hh"

#include "simulation_behavior.hh"
#include "simulation_physics.hh"
#include "simulation_map.hh"
#include "simulation_inputmessages.hh"
#include "simulation_ruleset.hh"
#include "simulation_outputmessages.hh"

#include "log.hh"
#include "programcontroller.hh"
#include "messagehandler.hh"

static unsigned int playerId = 0;
static messaging_mode messagingMode = MM_OFFLINE;

static void updateApplyBehaviors();
static void updateApplyPhysics();

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

    if (!SHH::Simulation::InputMessages::Init())
    {
	SHH::Log::Error("Simulation::Init(): Could not init inputmessages.");
	SHH::Simulation::Map::Deinit();
	SHH::Simulation::Physics::Deinit();
	SHH::Simulation::Behavior::Deinit();
	return false;
    }

    if (!SHH::Simulation::Ruleset::Init())
    {
	SHH::Log::Error("Simulation::Init(): Could not init ruleset.");
	SHH::Simulation::InputMessages::Deinit();
	SHH::Simulation::Map::Deinit();
	SHH::Simulation::Physics::Deinit();
	SHH::Simulation::Behavior::Deinit();
	return false;
    }

    if (!SHH::Simulation::OutputMessages::Init())
    {
	SHH::Log::Error("Simulation::Init(): Could not init outputmessages.");
	SHH::Simulation::Ruleset::Deinit();
	SHH::Simulation::InputMessages::Deinit();
	SHH::Simulation::Map::Deinit();
	SHH::Simulation::Physics::Deinit();
	SHH::Simulation::Behavior::Deinit();
	return false;
    }

    SHH::Log::Log("Simulation::Init(): Ended successfully.");
    return true;
}

bool SHH::Simulation::Postinit()
{
    SHH::Simulation::LoadMap("menu");
    return true;
}

void SHH::Simulation::Deinit()
{
    SHH::Log::Log("Simulation::Deinit(): Started.");

    SHH::Simulation::OutputMessages::Deinit();
    SHH::Simulation::Ruleset::Deinit();
    SHH::Simulation::InputMessages::Deinit();
    SHH::Simulation::Map::Deinit();
    SHH::Simulation::Physics::Deinit();
    SHH::Simulation::Behavior::Deinit();

    SHH::Log::Log("Simulation::Deinit(): Ended successfully.");
}

void SHH::Simulation::Update()
{
    SHH::Simulation::InputMessages::FlushMessages();
    if (messagingMode == MM_CLIENT)
    {
	SHH::Simulation::InputMessages::FetchSimMessages();
    }
    SHH::Simulation::InputMessages::FetchCtrlMessages();
    SHH::Simulation::InputMessages::DistributeMessages();
    SHH::Simulation::Map::HandleMessages();
    updateApplyBehaviors();
    updateApplyPhysics();
    if (messagingMode == MM_SERVER)
    {
	SHH::Simulation::OutputMessages::DistributeMessages();
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

messaging_mode SHH::Simulation::GetMessagingMode()
{
    return messagingMode;
}

unsigned int SHH::Simulation::GetPlayerId()
{
    return playerId;
}

void SHH::Simulation::SetPlayerId(unsigned int pid)
{
    playerId = pid;
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
