#include "simulation_behavior.hh"

#include "simulation_behavior_player.hh"

#include "log.hh"

static behavior behaviorList [OT_MAXVALUE];

static const int CONTROL_EVENT_LIST_MAXSIZE = 100;
static message_ctrl* controlEventList = nullptr;
static int controlEventListStart = 0;
static int controlEventListEnd = 0;
static int controlEventListSize = 0;

bool SHH::Simulation::Behavior::Init()
{
    SHH::Log::Log("Simulation::Behavior::Init(): Started.");

    behaviorList[OT_NONE].OnUpdate = nullptr;
    behaviorList[OT_NONE].OnMessage = nullptr;

    behaviorList[OT_BASIC].OnUpdate = nullptr;
    behaviorList[OT_BASIC].OnMessage = nullptr;

    behaviorList[OT_PLAYER].OnUpdate = OT_PLAYER_OnUpdate;
    behaviorList[OT_PLAYER].OnMessage = OT_PLAYER_OnMessage;

    controlEventList = new message_ctrl [CONTROL_EVENT_LIST_MAXSIZE];
    if (controlEventList == nullptr)
    {
	SHH::Log::Error("Simulation::Behavior::Init(): Could not allocate memory for controlEventList.");
	return false;
    }

    for (int i = 0; i < CONTROL_EVENT_LIST_MAXSIZE; ++i)
    {
	SHH::Units::CreateNoneControlMessage(controlEventList[i]);
    }

    SHH::Log::Log("Simulation::Behavior::Init(): Ended successfully.");
    return true;
}

void SHH::Simulation::Behavior::Deinit()
{
    SHH::Log::Log("Simulation::Behavior::Deinit(): Started.");

    if (controlEventList != nullptr)
    {
	delete[] controlEventList;
	controlEventList = nullptr;
    }

    SHH::Log::Log("Simulation::Behavior::Deinit(): Ended successfully.");
}

bool SHH::Simulation::Behavior::PushControlMessage(message_ctrl const& msg)
{
    if (controlEventListSize >= CONTROL_EVENT_LIST_MAXSIZE)
    {
	SHH::Log::Warning("Simulation::Behavior::PushControlMessage(): Not enough space in controlEventList.");
	return false;
    }
    
    controlEventList[controlEventListEnd] = msg;
    controlEventListEnd = (controlEventListEnd + 1) % CONTROL_EVENT_LIST_MAXSIZE;
    controlEventListSize++;
    return true;
}

void SHH::Simulation::Behavior::ApplyBehaviors(object* olist, object* obrlist, int olistsize)
{
    for (int i = 0; i < olistsize; ++i)
    {
	otype type = olist[i].type;
	if (type != OT_NONE)
	{
	    obrlist[i] = olist[i];
        }
	if (behaviorList[type].OnUpdate != nullptr)
	{
	    behaviorList[type].OnUpdate(obrlist[i]);
	}
	if (behaviorList[type].OnMessage != nullptr)
	{
	    for (int j = controlEventListStart; j != controlEventListEnd; j = (j + 1) % CONTROL_EVENT_LIST_MAXSIZE)
	    {
		behaviorList[type].OnMessage(controlEventList[j], obrlist[i]);
	    }
	}
    }
    controlEventListStart = controlEventListEnd;
    controlEventListSize = 0;
}
