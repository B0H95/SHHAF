#pragma once

#include <cstdint>

enum ostate
{
    OS_IDLE
};

enum odirection
{
    OD_NOWHERE,
    OD_UP,
    OD_RIGHT,
    OD_DOWN,
    OD_LEFT
};

enum otype
{
    OT_NONE,
    OT_BASIC,
    OT_PLAYER,

    OT_MAXVALUE
};

enum etype
{
    ET_NONE,
    ET_SOLID
};

enum mstype
{
    MS_OBJECTUPDATE
};

enum mctype
{
    MC_NOTHING,
    MC_RUNLEFT,
    MC_RUNRIGHT,

    MC_MARKER_SIM,

    MC_MARKER_CTRL
};

struct object
{
    otype type;
    ostate state;
    odirection direction;
    float xaccel,yaccel,xspeed,yspeed,x,y,width,height;
};

struct environment
{
    etype type;
    float x,y,width,height;
};

struct message_sim
{
    mstype messagetype;
    otype objecttype;
    odirection direction;
    ostate state;
    uint32_t index;
    float x,y,xspeed,yspeed,xaccel,yaccel,width,height;
};

struct message_ctrl
{
    mctype messagetype;
    uint8_t sender;
    uint8_t strsize;
    char* str;
};

// The function ordering reflects calling order
struct behavior
{
    void (*OnUpdate)(object& obj);
    void (*OnMessage)(message_ctrl const& msg, object& obj);
};

namespace SHH
{
    namespace Units
    {
	void CreateNoneObject(object& obj);
	void CreateNoneEnvironment(environment& env);
	void CreateNoneControlMessage(message_ctrl& msg);
	environment CreateSolidEnvironment(float x, float y, float w, float h);
	object CreateBasicObject(float x, float y, float w, float h);
	object CreatePlayerObject(float x, float y, float w, float h);
    }
}
