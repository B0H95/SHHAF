#include "simulation_outputmessages.hh"

#include "simulation_ruleset.hh"

#include "log.hh"
#include "messagehandler.hh"

static const unsigned int SIM_MESSAGE_LIST_SIZE = 1000;
static message_sim* simMessageList = nullptr;
static unsigned int simMessageListSize = 0;

bool SHH::Simulation::OutputMessages::Init()
{
    SHH::Log::Log("Simulation::OutputMessages::Init(): Started.");

    simMessageList = new message_sim [SIM_MESSAGE_LIST_SIZE];
    if (simMessageList == nullptr)
    {
	SHH::Log::Error("Simulation::OutputMessages::Init(): Could not allocate memory for simMessageList.");
	return false;
    }
    
    SHH::Log::Log("Simulation::OutputMessages::Init(): Ended successfully.");
    return true;
}

void SHH::Simulation::OutputMessages::Deinit()
{
    SHH::Log::Log("Simulation::OutputMessages::Deinit(): Started.");

    if (simMessageList != nullptr)
    {
	delete[] simMessageList;
	simMessageList = nullptr;
    }
    
    SHH::Log::Log("Simulation::OutputMessages::Deinit(): Ended successfully.");
}

bool SHH::Simulation::OutputMessages::PushSimMessage(message_sim const& msg)
{
    if (SHH::MessageHandler::GetMessagingMode() != MM_SERVER)
    {
	simMessageListSize = 0;
	return true;
    }
    if (simMessageListSize >= SIM_MESSAGE_LIST_SIZE)
    {
	SHH::Log::Warning("Simulation::OutputMessages::PushSimMessage(): No more room in simMessageList.");
	return false;
    }
    simMessageList[simMessageListSize++] = msg;
    return true;
}

void SHH::Simulation::OutputMessages::DistributeMessages()
{
    if (SHH::MessageHandler::GetMessagingMode() != MM_SERVER)
    {
	simMessageListSize = 0;
	return;
    }
    for (unsigned int i = 0; i < simMessageListSize; ++i)
    {
	SHH::Simulation::Ruleset::HandleSimMessage(simMessageList[i]);
	SHH::MessageHandler::PushSimulationMessage(simMessageList[i]);
    }
    simMessageListSize = 0;
}
