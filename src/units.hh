#pragma once

#include <string>

enum ostate
{
    OS_IDLE,
    OS_STANDING,
    OS_FALLING
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
    MS_NOTHING,
    MS_OBJECTUPDATE,
    MS_PLAYERIDENTIFICATION
};

enum mctype
{
    MC_NOTHING,
    MC_RUNLEFT,
    MC_RUNRIGHT,
    MC_JUMP,

    MC_MARKER_SIM,

    MC_REQUESTID,

    MC_MARKER_CTRL
};

enum messaging_mode
{
    MM_OFFLINE,
    MM_CLIENT,
    MM_SERVER
};

struct object
{
    otype type;
    ostate state;
    odirection direction;
    unsigned int syncindex;
    unsigned int owner;
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
    object obj;
};

struct message_ctrl
{
    mctype messagetype;
    int sender;
    int strsize;
    std::string str;
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

	std::string SerializeSimMessage(message_sim const& msg);
	std::string SerializeCtrlMessage(message_ctrl const& msg);
	std::string SerializeObject(object const& obj);
	std::string SerializeInt(int value);
	std::string SerializeFloat(float value);

	message_sim DeserializeSimMessage(std::string str);
	message_ctrl DeserializeCtrlMessage(std::string str);
	object DeserializeObject(std::string str);
	int DeserializeInt(std::string str);
	float DeserializeFloat(std::string str);
    }
}
