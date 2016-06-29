#include "simulation_inputmessages.hh"

#include "simulation.hh"
#include "simulation_behavior.hh"
#include "simulation_map.hh"

#include "log.hh"
#include "units.hh"
#include "messagehandler.hh"

static const unsigned int SIM_MESSAGE_LIST_SIZE = 1000;
static message_sim* simMessageList = nullptr;
static unsigned int simMessageListSize = 0;

static const unsigned int CTRL_MESSAGE_LIST_SIZE = 1000;
static message_ctrl* ctrlMessageList = nullptr;
static unsigned int ctrlMessageListSize = 0;

bool SHH::Simulation::InputMessages::Init()
{
    SHH::Log::Log("Simulation::InputMessages::Init(): Started.");

    simMessageList = new message_sim [SIM_MESSAGE_LIST_SIZE];
    if (simMessageList == nullptr)
    {
	SHH::Log::Error("Simulation::InputMessages::Init(): Could not allocate memory for simMessageList.");
	return false;
    }

    ctrlMessageList = new message_ctrl [CTRL_MESSAGE_LIST_SIZE];
    if (ctrlMessageList == nullptr)
    {
	SHH::Log::Error("Simulation::InputMessages::Init(): Could not allocate memory for ctrlMessageList.");
	delete[] simMessageList;
	simMessageList = nullptr;
	return false;
    }

    for (unsigned int i = 0; i < SIM_MESSAGE_LIST_SIZE; ++i)
    {
	simMessageList[i].messagetype = MS_NOTHING;
    }

    for (unsigned int i = 0; i < CTRL_MESSAGE_LIST_SIZE; ++i)
    {
	ctrlMessageList[i].messagetype = MC_NOTHING;
    }

    SHH::Log::Log("Simulation::InputMessages::Init(): Ended successfully.");
    return true;
}

void SHH::Simulation::InputMessages::Deinit()
{
    SHH::Log::Log("Simulation::InputMessages::Deinit(): Started.");

    if (simMessageList != nullptr)
    {
	delete[] simMessageList;
	simMessageList = nullptr;
	simMessageListSize = 0;
    }

    if (ctrlMessageList != nullptr)
    {
	delete[] ctrlMessageList;
	ctrlMessageList = nullptr;
	ctrlMessageListSize = 0;
    }

    SHH::Log::Log("Simulation::InputMessages::Deinit(): Ended successfully.");
}

void SHH::Simulation::InputMessages::FlushMessages()
{
    simMessageListSize = 0;
    ctrlMessageListSize = 0;
}

void SHH::Simulation::InputMessages::FetchSimMessages()
{
    message_sim msg;
    while ((msg = SHH::MessageHandler::PopSimulationMessage()).messagetype != MS_NOTHING)
    {
	if (simMessageListSize >= SIM_MESSAGE_LIST_SIZE)
	{
	    SHH::Log::Warning("Simulation::InputMessages::FetchSimMessages(): Could not fit any more messages.");
	    return;
	}
	simMessageList[simMessageListSize++] = msg;
    }
}

void SHH::Simulation::InputMessages::FetchCtrlMessages()
{
    message_ctrl msg;
    while ((msg = SHH::MessageHandler::PopIncomingControlMessage()).messagetype != MC_NOTHING)
    {
	if (ctrlMessageListSize >= CTRL_MESSAGE_LIST_SIZE)
	{
	    SHH::Log::Warning("Simulation::InputMessages::FetchCtrlMessages(): Could not fit any more messages.");
	    return;
	}
	ctrlMessageList[ctrlMessageListSize++] = msg;
    }
}

void SHH::Simulation::InputMessages::DistributeMessages()
{
    for (unsigned int i = 0; i < ctrlMessageListSize; ++i)
    {
	if (ctrlMessageList[i].messagetype < MC_MARKER_SIM)
	{
	    SHH::Simulation::Behavior::PushControlMessage(&(ctrlMessageList[i]));
	}
	else if (ctrlMessageList[i].messagetype < MC_MARKER_CTRL)
	{
	    SHH::Simulation::Map::PushControlMessage(&(ctrlMessageList[i]));
	}
    }

    for (unsigned int i = 0; i < simMessageListSize; ++i)
    {
	if (simMessageList[i].messagetype == MS_OBJECTUPDATE)
	{
	    SHH::Simulation::Map::PushSimulationMessage(&(simMessageList[i]));
	}
	else if (simMessageList[i].messagetype == MS_PLAYERIDENTIFICATION)
	{
	    SHH::Simulation::SetPlayerId(simMessageList[i].obj.owner);
	}
    }
}
