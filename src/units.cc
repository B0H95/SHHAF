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
    msg.str = "";
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

std::string SHH::Units::SerializeSimMessage(message_sim const& msg)
{
    return SerializeInt(msg.messagetype) + "," +
	SerializeObject(msg.obj);
}

std::string SHH::Units::SerializeCtrlMessage(message_ctrl const& msg)
{
    return SerializeInt(msg.messagetype) + "," +
	SerializeInt(msg.sender) + "," +
	SerializeInt(msg.strsize) + "," +
	msg.str;
}

std::string SHH::Units::SerializeObject(object const& obj)
{
    return SerializeInt(obj.type) + "," +
	SerializeInt(obj.state) + "," +
	SerializeInt(obj.direction) + "," +
	SerializeInt(obj.syncindex) + "," +
	SerializeFloat(obj.x) + "," +
	SerializeFloat(obj.y) + "," +
	SerializeFloat(obj.xspeed) + "," +
	SerializeFloat(obj.yspeed) + "," +
	SerializeFloat(obj.xaccel) + "," +
	SerializeFloat(obj.yaccel) + "," +
	SerializeFloat(obj.width) + "," +
	SerializeFloat(obj.height);
}

std::string SHH::Units::SerializeInt(int value)
{
    return std::to_string(value);
}

std::string SHH::Units::SerializeFloat(float value)
{
    std::string str = std::to_string(value);
    while (str.back() == '0' && str.length() > 1)
    {
	str.pop_back();
    }
    return str;
}

message_sim SHH::Units::DeserializeSimMessage(std::string str)
{
    message_sim returnmsg;
    
    std::size_t separatorpos = str.find(",");
    returnmsg.messagetype = (mstype)DeserializeInt(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);

    returnmsg.obj = DeserializeObject(str);

    return returnmsg;
}

message_ctrl SHH::Units::DeserializeCtrlMessage(std::string str)
{
    message_ctrl returnmsg;
    
    std::size_t separatorpos = str.find(",");
    returnmsg.messagetype = (mctype)DeserializeInt(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);

    separatorpos = str.find(",");
    returnmsg.sender = DeserializeInt(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);

    separatorpos = str.find(",");
    returnmsg.strsize = DeserializeInt(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);

    returnmsg.str = str;

    return returnmsg;
}

object SHH::Units::DeserializeObject(std::string str)
{
    object returnobj;
    
    std::size_t separatorpos = str.find(",");
    returnobj.type = (otype)DeserializeInt(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.state = (ostate)DeserializeInt(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.direction = (odirection)DeserializeInt(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.syncindex = DeserializeInt(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.x = DeserializeFloat(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.y = DeserializeFloat(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.xspeed = DeserializeFloat(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.yspeed = DeserializeFloat(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.xaccel = DeserializeFloat(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.yaccel = DeserializeFloat(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.width = DeserializeFloat(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    separatorpos = str.find(",");
    returnobj.height = DeserializeFloat(str.substr(0, separatorpos));
    str.erase(0, separatorpos + 1);
    
    return returnobj;
}

int SHH::Units::DeserializeInt(std::string str)
{
    if (str.length() > 0)
    {
	return std::stoi(str);
    }
    return 0;
}

float SHH::Units::DeserializeFloat(std::string str)
{
    if (str.length() > 0)
    {
	return std::stof(str);
    }
    return 0.0f;
}
