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
    SHH::Log::Log("SHH::Simulation::Behavior::Init(): Started.");

    behaviorList[OT_NONE].OnUpdate = nullptr;
    behaviorList[OT_NONE].OnMessage = nullptr;

    behaviorList[OT_BASIC].OnUpdate = nullptr;
    behaviorList[OT_BASIC].OnMessage = nullptr;

    behaviorList[OT_PLAYER].OnUpdate = OT_PLAYER_OnUpdate;
    behaviorList[OT_PLAYER].OnMessage = OT_PLAYER_OnMessage;

    controlEventList = new message_ctrl [CONTROL_EVENT_LIST_MAXSIZE];
    if (controlEventList == nullptr)
    {
	SHH::Log::Error("SHH::Simulation::Behavior::Init(): Could not allocate memory for controlEventList.");
	return false;
    }

    for (int i = 0; i < CONTROL_EVENT_LIST_MAXSIZE; ++i)
    {
	controlEventList[i].messagetype = MC_NOTHING;
	controlEventList[i].sender = 0;
	controlEventList[i].strsize = 0;
	controlEventList[i].str = nullptr;
    }

    SHH::Log::Log("SHH::Simulation::Behavior::Init(): Ended successfully.");
    return true;
}

void SHH::Simulation::Behavior::Deinit()
{
    SHH::Log::Log("SHH::Simulation::Behavior::Deinit(): Started.");

    if (controlEventList != nullptr)
    {
	delete[] controlEventList;
	controlEventList = nullptr;
    }

    SHH::Log::Log("SHH::Simulation::Behavior::Deinit(): Ended successfully.");
}

bool SHH::Simulation::Behavior::PushControlMessage(message_ctrl const& msg)
{
    if (controlEventListSize >= CONTROL_EVENT_LIST_MAXSIZE)
    {
	SHH::Log::Warning("SHH::Simulation::Behavior::PushControlMessage(): Not enough space in controlEventList.");
	return false;
    }
    
    controlEventList[controlEventListEnd] = msg;
    controlEventListEnd = (controlEventListEnd + 1) % CONTROL_EVENT_LIST_MAXSIZE;
    controlEventListSize++;
    return true;
}

void SHH::Simulation::Behavior::ApplyBehaviors(object* olist, int olistsize)
{
    for (int i = 0; i < olistsize; ++i)
    {
	otype type = olist[i].type;
	if (!(behaviorList[type].OnUpdate == nullptr))
	{
	    behaviorList[type].OnUpdate(olist[i]);
	}
	if (!(behaviorList[type].OnMessage == nullptr))
	{
	    for (int j = controlEventListStart; j != controlEventListEnd; j = (j + 1) % CONTROL_EVENT_LIST_MAXSIZE)
	    {
		behaviorList[type].OnMessage(controlEventList[j], olist[i]);
	    }
	}
    }
    controlEventListStart = controlEventListEnd;
    controlEventListSize = 0;
}
