#include "simulation_behavior_player.hh"

void OT_PLAYER_OnUpdate(object& obj)
{
    obj.xspeed = 0.0f;
}

void OT_PLAYER_OnMessage(message_ctrl const& msg, object& obj)
{
    switch (msg.messagetype)
    {
    case MC_NOTHING:
	break;
    case MC_RUNLEFT:
	if (obj.owner == 0)
	{
	    obj.xspeed -= 250.0f;
	}
	break;
    case MC_RUNRIGHT:
	if (obj.owner == 0)
	{
	    obj.xspeed += 250.0f;
	}
	break;
    case MC_JUMP:
	if (obj.owner == 0)
	{
	    obj.yspeed = -250.0f;
	}
	break;
    default:
	break;
    }
}
