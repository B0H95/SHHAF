#pragma once

#include "units.hh"

namespace SHH
{
    namespace MessageHandler
    {
	bool Init();
	void Deinit();
	void SetMessagingMode(messaging_mode mmode);
	messaging_mode GetMessagingMode();
	bool PushControlMessage(message_ctrl const& msg);
	bool PushOutgoingControlMessage(message_ctrl const& msg);
	bool PushIncomingControlMessage(message_ctrl const& msg);
	bool PushSimulationMessage(message_sim const& msg);
	message_ctrl PopIncomingControlMessage();
	message_ctrl PopOutgoingControlMessage();
	message_sim PopSimulationMessage();
    }
}
