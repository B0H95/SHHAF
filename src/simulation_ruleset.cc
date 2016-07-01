#include "simulation_ruleset.hh"

#include "log.hh"

struct userdata
{
    int uid;
    bool alive;
};

static const unsigned int USERDATA_LIST_SIZE = 100;
static userdata* userdataList = nullptr;
static unsigned int userdataListSize = 0;

static int UserId(int id);
static void InitUserData(int uid, unsigned int listindex);

bool SHH::Simulation::Ruleset::Init()
{
    SHH::Log::Log("Simulation::Ruleset::Init(): Started.");

    userdataList = new userdata [USERDATA_LIST_SIZE];
    if (userdataList == nullptr)
    {
	SHH::Log::Error("Simulation::Ruleset::Init(): Could not allocate memory for userdataList.");
	return false;
    }

    userdataList[0].uid = 0;
    userdataList[0].alive = true; //TODO: Init local player some better way
    userdataListSize = 1;
    
    SHH::Log::Log("Simulation::Ruleset::Init(): Ended successfully.");
    return true;
}

void SHH::Simulation::Ruleset::Deinit()
{
    SHH::Log::Log("Simulation::Ruleset::Deinit(): Started.");

    if (userdataList != nullptr)
    {
	delete[] userdataList;
	userdataList = nullptr;
    }
    
    SHH::Log::Log("Simulation::Ruleset::Deinit(): Ended successfully.");
}

bool SHH::Simulation::Ruleset::HandleCtrlMessage(message_ctrl const& msg)
{
    int listId = UserId(msg.sender);

    if (msg.messagetype == MC_RESPAWN)
    {
	if (userdataList[listId].alive == false)
	{
	    userdataList[listId].alive = true;
	}
	else
	{
	    return false;
	}
    }
    
    return true;
}

static int UserId(int id)
{
    for (unsigned int i = 0; i < userdataListSize; ++i)
    {
	if (userdataList[i].uid == id)
	{
	    return i;
	}
    }

    InitUserData(id, userdataListSize);
    return userdataListSize++;
}

static void InitUserData(int uid, unsigned int listindex)
{
    userdataList[listindex].uid = uid;
    userdataList[listindex].alive = false;
}
