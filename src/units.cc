#include "units.hh"

void SHH::Units::CreateNoneObject(object& obj)
{
    obj.type = OT_NONE;
    obj.state = OS_IDLE;
    obj.direction = OD_NOWHERE;
    obj.syncindex = 0;
    obj.x = 0.0f;
    obj.y = 0.0f;
    obj.xspeed = 0.0f;
    obj.yspeed = 0.0f;
    obj.xaccel = 0.0f;
    obj.yaccel = 0.0f;
    obj.width = 0.0f;
    obj.height = 0.0f;
}

void SHH::Units::CreateNoneEnvironment(environment& env)
{
    env.type = ET_NONE;
    env.x = 0.0f;
    env.y = 0.0f;
    env.width = 0.0f;
    env.height = 0.0f;
}

void SHH::Units::CreateNoneControlMessage(message_ctrl& msg)
{
    msg.messagetype = MC_NOTHING;
    msg.sender = 0;
    msg.strsize = 0;
    msg.str = nullptr;
}

environment SHH::Units::CreateSolidEnvironment(float x, float y, float w, float h)
{
    environment e = {ET_SOLID,x,y,w,h};
    return e;
}

object SHH::Units::CreateBasicObject(float x, float y, float w, float h)
{
    object o = {OT_BASIC,OS_IDLE,OD_NOWHERE,0,0.0f,0.0f,0.0f,0.0f,x,y,w,h};
    return o;
}

object SHH::Units::CreatePlayerObject(float x, float y, float w, float h)
{
    object o = {OT_PLAYER,OS_IDLE,OD_NOWHERE,0,0.0f,0.0f,0.0f,0.0f,x,y,w,h};
    return o;
}
