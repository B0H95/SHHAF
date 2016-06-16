#include "messagehandler.hh"

#include "log.hh"

static const int INQUEUE_CTRL_MAXSIZE = 100;
static message_ctrl* inqueueCtrl = nullptr;
static int inqueueCtrlStart = 0;
static int inqueueCtrlEnd = 0;
static int inqueueCtrlSize = 0;

static const int INQUEUE_SIM_MAXSIZE = 100;
static message_sim* inqueueSim = nullptr;
static int inqueueSimStart = 0;
static int inqueueSimEnd = 0;
static int inqueueSimSize = 0;

bool SHH::MessageHandler::Init()
{
    SHH::Log::Log("SHH::MessageHandler::Init(): Started.");

    inqueueCtrl = new message_ctrl [INQUEUE_CTRL_MAXSIZE];
    if (inqueueCtrl == nullptr)
    {
	SHH::Log::Error("SHH::MessageHandler::Init(): Could not allocate memory for inqueueCtrl.");
	return false;
    }

    inqueueSim = new message_sim [INQUEUE_SIM_MAXSIZE];
    if (inqueueSim == nullptr)
    {
	SHH::Log::Error("SHH::MessageHandler::Init(): Could not allocate memory for inqueueSim.");
	delete[] inqueueCtrl;
	inqueueCtrl = nullptr;
	return false;
    }

    SHH::Log::Log("SHH::MessageHandler::Init(): Ended successfully.");
    return true;
}

void SHH::MessageHandler::Deinit()
{
    SHH::Log::Log("SHH::MessageHandler::Deinit(): Started.");

    if (inqueueCtrl != nullptr)
    {
	delete[] inqueueCtrl;
	inqueueCtrl = nullptr;
    }

    if (inqueueSim != nullptr)
    {
	delete[] inqueueSim;
	inqueueSim = nullptr;
    }

    SHH::Log::Log("SHH::MessageHandler::Deinit(): Ended successfully.");
}

bool SHH::MessageHandler::PushControlMessage(message_ctrl const& msg)
{
    if (inqueueCtrlSize == INQUEUE_CTRL_MAXSIZE)
    {
	SHH::Log::Warning("SHH::MessageHandler::PushControlMessage(): Queue is filled.");
	return false;
    }

    inqueueCtrl[inqueueCtrlEnd] = msg;
    inqueueCtrlEnd = (inqueueCtrlEnd + 1) % INQUEUE_CTRL_MAXSIZE;
    inqueueCtrlSize++;

    return true;
}

bool SHH::MessageHandler::PushSimulationMessage(message_sim const& msg)
{
    if (inqueueSimSize == INQUEUE_SIM_MAXSIZE)
    {
	SHH::Log::Warning("SHH::MessageHandler::PushSimulationMessage(): Queue is filled.");
	return false;
    }

    inqueueSim[inqueueSimEnd] = msg;
    inqueueSimEnd = (inqueueSimEnd + 1) % INQUEUE_SIM_MAXSIZE;
    inqueueSimSize++;

    return true;
}

message_ctrl SHH::MessageHandler::PopControlMessage()
{
    if (inqueueCtrlSize > 0)
    {
	inqueueCtrlSize--;
	int temp = inqueueCtrlStart;
	inqueueCtrlStart = (inqueueCtrlStart + 1) % INQUEUE_CTRL_MAXSIZE;
	return inqueueCtrl[temp];
    }
    return {MC_NOTHING, 0, 0, nullptr};
}

message_sim SHH::MessageHandler::PopSimulationMessage()
{
    if (inqueueSimSize > 0)
    {
	inqueueSimSize--;
	int temp = inqueueSimStart;
	inqueueSimStart = (inqueueSimStart + 1) % INQUEUE_SIM_MAXSIZE;
	return inqueueSim[temp];
    }
    return {MS_NOTHING, OT_NONE, OS_IDLE, OD_NOWHERE, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}
