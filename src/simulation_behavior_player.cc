#include "simulation_behavior_player.hh"

#include "simulation.hh"

void OT_PLAYER_OnUpdate(object& obj)
{
    obj.xspeed = 0.0f;
}

void OT_PLAYER_OnMessage(message_ctrl* msg, object& obj)
{
    unsigned int id = (unsigned int)msg->sender;
    
    switch (msg->messagetype)
    {
    case MC_NOTHING:
	break;
    case MC_RUNLEFT:
	if (obj.owner == id)
	{
	    obj.xspeed -= 250.0f;
	}
	break;
    case MC_RUNRIGHT:
	if (obj.owner == id)
	{
	    obj.xspeed += 250.0f;
	}
	break;
    case MC_JUMP:
	if (obj.owner == id)
	{
	    obj.yspeed = -250.0f;
	}
	break;
    default:
	break;
    }
}
