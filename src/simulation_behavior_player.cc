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
	obj.xspeed -= 250.0f;
	break;
    case MC_RUNRIGHT:
	obj.xspeed += 250.0f;
	break;
    case MC_JUMP:
	if (obj.state == OS_STANDING)
	{
	    obj.yspeed = -230.0f;
	    obj.state = OS_FALLING;
	}
	break;
    default:
	break;
    }
}
