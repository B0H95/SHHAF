#include "messagehandler.hh"

#include "log.hh"
#include "circularcontainer.hh"

static messaging_mode messagingMode = MM_OFFLINE;

static const int INQUEUE_CTRL_MAXSIZE = 100;
static const int OUTQUEUE_CTRL_MAXSIZE = 100;
static const int INQUEUE_SIM_MAXSIZE = 100;

static CircularContainer<message_ctrl> inqueueCtrl (INQUEUE_CTRL_MAXSIZE);
static CircularContainer<message_ctrl> outqueueCtrl (OUTQUEUE_CTRL_MAXSIZE);
static CircularContainer<message_sim> inqueueSim (INQUEUE_SIM_MAXSIZE);

bool SHH::MessageHandler::Init()
{
    SHH::Log::Log("SHH::MessageHandler::Init(): Started.");

    if (!inqueueCtrl.IsValid())
    {
	SHH::Log::Error("SHH::MessageHandler::Init(): Could not allocate memory for inqueueCtrl.");
	return false;
    }

    if (!inqueueSim.IsValid())
    {
	SHH::Log::Error("SHH::MessageHandler::Init(): Could not allocate memory for inqueueSim.");
	inqueueCtrl.Deinit();
	return false;
    }

    if (!outqueueCtrl.IsValid())
    {
	SHH::Log::Error("SHH::MessageHandler::Init(): Could not allocate memory for outqueueCtrl.");
	inqueueSim.Deinit();
	inqueueCtrl.Deinit();
	return false;
    }

    SHH::Log::Log("SHH::MessageHandler::Init(): Ended successfully.");
    return true;
}

void SHH::MessageHandler::Deinit()
{
    SHH::Log::Log("SHH::MessageHandler::Deinit(): Started.");

    inqueueCtrl.Deinit();
    inqueueSim.Deinit();
    outqueueCtrl.Deinit();

    SHH::Log::Log("SHH::MessageHandler::Deinit(): Ended successfully.");
}

void SHH::MessageHandler::SetMessagingMode(messaging_mode mmode)
{
    messagingMode = mmode;
}

messaging_mode SHH::MessageHandler::GetMessagingMode()
{
    return messagingMode;
}

bool SHH::MessageHandler::PushControlMessage(message_ctrl const& msg)
{
    if (messagingMode == MM_CLIENT && !outqueueCtrl.Push(msg))
    {
	SHH::Log::Warning("SHH::MessageHandler::PushControlMessage(): Outqueue is filled.");
	return false;
    }

    if (!inqueueCtrl.Push(msg))
    {
	SHH::Log::Warning("SHH::MessageHandler::PushControlMessage(): Inqueue is filled.");
	return false;
    }

    return true;
}

bool SHH::MessageHandler::PushSimulationMessage(message_sim const& msg)
{
    if (!inqueueSim.Push(msg))
    {
	SHH::Log::Warning("SHH::MessageHandler::PushSimulationMessage(): Queue is filled.");
	return false;
    }

    return true;
}

message_ctrl SHH::MessageHandler::PopIncomingControlMessage()
{
    if (inqueueCtrl.Size() > 0)
    {
	return inqueueCtrl.Pop();
    }
    return {MC_NOTHING, 0, 0, nullptr};
}

message_ctrl SHH::MessageHandler::PopOutgoingControlMessage()
{
    if (outqueueCtrl.Size() > 0)
    {
	return outqueueCtrl.Pop();
    }
    return {MC_NOTHING, 0, 0, nullptr};
}

message_sim SHH::MessageHandler::PopSimulationMessage()
{
    if (inqueueSim.Size() > 0)
    {
	return inqueueSim.Pop();
    }
    return {MS_NOTHING, OT_NONE, OS_IDLE, OD_NOWHERE, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}
