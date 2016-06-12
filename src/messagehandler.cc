#include "messagehandler.hh"

#include "log.hh"

static const int INQUEUE_CTRL_MAXSIZE = 100;
static message_ctrl* inqueueCtrl = nullptr;
static int inqueueCtrlStart = 0;
static int inqueueCtrlEnd = 0;
static int inqueueCtrlSize = 0;

bool SHH::MessageHandler::Init()
{
    SHH::Log::Log("SHH::MessageHandler::Init(): Started.");

    inqueueCtrl = new message_ctrl [INQUEUE_CTRL_MAXSIZE];
    if (inqueueCtrl == nullptr)
    {
	SHH::Log::Error("SHH::MessageHandler::Init(): Could not allocate memory for inqueueCtrl.");
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

    SHH::Log::Log("SHH::MessageHandler::Deinit(): Ended successfully.");
}

bool SHH::MessageHandler::PushControlMessage(message_ctrl const& msg)
{
    if (inqueueCtrlSize == INQUEUE_CTRL_MAXSIZE)
    {
	SHH::Log::Warning("SHH::MessageHandler::PushControlMessage(...): Queue is filled.");
	return false;
    }

    inqueueCtrl[inqueueCtrlEnd] = msg;
    inqueueCtrlEnd = (inqueueCtrlEnd + 1) % INQUEUE_CTRL_MAXSIZE;
    inqueueCtrlSize++;

    return true;
}

bool SHH::MessageHandler::PushSimulationMessage(message_sim const& msg)
{
    //TODO: Networking
    return msg.index == 9898989;
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
    return {NOTHING, 0, 0, nullptr};
}

message_sim SHH::MessageHandler::PopSimulationMessage()
{
    //TODO: Networking
    return {OBJECTUPDATE, NONE, NOWHERE, IDLE, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}
