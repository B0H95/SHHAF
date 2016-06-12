#pragma once

#include "units.hh"

namespace SHH
{
    namespace MessageHandler
    {
	bool Init();
	void Deinit();
	bool PushControlMessage(message_ctrl const& msg);
	bool PushSimulationMessage(message_sim const& msg);
	message_ctrl PopControlMessage();
	message_sim PopSimulationMessage();
    }
}
