#include "messagehandler.hh"

#include <mutex>
#include "log.hh"
#include "circularcontainer.hh"

static messaging_mode messagingMode = MM_OFFLINE;

static const int INQUEUE_CTRL_MAXSIZE = 100;
static const int OUTQUEUE_CTRL_MAXSIZE = 100;
static const int INQUEUE_SIM_MAXSIZE = 100;

static std::mutex inqueueLock;
static std::mutex outqueueLock;
static std::mutex simqueueLock;

static CircularContainer<message_ctrl> inqueueCtrl (INQUEUE_CTRL_MAXSIZE);
static CircularContainer<message_ctrl> outqueueCtrl (OUTQUEUE_CTRL_MAXSIZE);
static CircularContainer<message_sim> inqueueSim (INQUEUE_SIM_MAXSIZE);

bool SHH::MessageHandler::Init()
{
    SHH::Log::Log("MessageHandler::Init(): Started.");

    if (!inqueueCtrl.IsValid())
    {
	SHH::Log::Error("MessageHandler::Init(): Could not allocate memory for inqueueCtrl.");
	return false;
    }

    if (!inqueueSim.IsValid())
    {
	SHH::Log::Error("MessageHandler::Init(): Could not allocate memory for inqueueSim.");
	inqueueCtrl.Deinit();
	return false;
    }

    if (!outqueueCtrl.IsValid())
    {
	SHH::Log::Error("MessageHandler::Init(): Could not allocate memory for outqueueCtrl.");
	inqueueSim.Deinit();
	inqueueCtrl.Deinit();
	return false;
    }

    SHH::Log::Log("MessageHandler::Init(): Ended successfully.");
    return true;
}

void SHH::MessageHandler::Deinit()
{
    SHH::Log::Log("MessageHandler::Deinit(): Started.");

    inqueueCtrl.Deinit();
    inqueueSim.Deinit();
    outqueueCtrl.Deinit();

    SHH::Log::Log("MessageHandler::Deinit(): Ended successfully.");
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
    if (messagingMode == MM_CLIENT)
    {
	outqueueLock.lock();
	
	if (!outqueueCtrl.Push(msg))
	{
	    outqueueLock.unlock();
	    SHH::Log::Warning("MessageHandler::PushControlMessage(): Outqueue is filled.");
	    return false;
	}
	
	outqueueLock.unlock();
    }

    inqueueLock.lock();
    if (!inqueueCtrl.Push(msg))
    {
	inqueueLock.unlock();
	SHH::Log::Warning("MessageHandler::PushControlMessage(): Inqueue is filled.");
	return false;
    }
    inqueueLock.unlock();

    return true;
}

bool SHH::MessageHandler::PushOutgoingControlMessage(message_ctrl const& msg)
{
    outqueueLock.lock();
    if (!outqueueCtrl.Push(msg))
    {
	outqueueLock.unlock();
	SHH::Log::Warning("MessageHandler::PushOutgoingControlMessage(): Outqueue is filled.");
	return false;
    }
    outqueueLock.unlock();
    return true;
}

bool SHH::MessageHandler::PushIncomingControlMessage(message_ctrl const& msg)
{
    inqueueLock.lock();
    if (!inqueueCtrl.Push(msg))
    {
	inqueueLock.unlock();
	SHH::Log::Warning("MessageHandler::PushIncomingControlMessage(): Inqueue is filled.");
	return false;
    }
    inqueueLock.unlock();

    return true;
}

bool SHH::MessageHandler::PushSimulationMessage(message_sim const& msg)
{
    simqueueLock.lock();
    if (!inqueueSim.Push(msg))
    {
	simqueueLock.unlock();
	SHH::Log::Warning("MessageHandler::PushSimulationMessage(): Queue is filled.");
	return false;
    }
    simqueueLock.unlock();

    return true;
}

message_ctrl SHH::MessageHandler::PopIncomingControlMessage()
{
    inqueueLock.lock();
    if (inqueueCtrl.Size() > 0)
    {
	message_ctrl msg = inqueueCtrl.Pop();
	inqueueLock.unlock();
	return msg;
    }
    inqueueLock.unlock();
    return {MC_NOTHING, 0, 0, ""};
}

message_ctrl SHH::MessageHandler::PopOutgoingControlMessage()
{
    outqueueLock.lock();
    if (outqueueCtrl.Size() > 0)
    {
	message_ctrl msg = outqueueCtrl.Pop();
	outqueueLock.unlock();
	return msg;
    }
    outqueueLock.unlock();
    return {MC_NOTHING, 0, 0, ""};
}

message_sim SHH::MessageHandler::PopSimulationMessage()
{
    simqueueLock.lock();
    if (inqueueSim.Size() > 0)
    {
	message_sim msg = inqueueSim.Pop();
	simqueueLock.unlock();
	return msg;
    }
    simqueueLock.unlock();
    return {MS_NOTHING, OT_NONE, OS_IDLE, OD_NOWHERE, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}
