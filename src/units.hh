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
    OT_BASIC
};

enum mstype
{
    MS_OBJECTUPDATE
};

enum mctype
{
    MC_NOTHING,
    MC_RUNLEFT,
    MC_RUNRIGHT
};

struct object
{
    otype type;
    ostate state;
    odirection direction;
    float xaccel,yaccel,xspeed,yspeed,x,y,width,height;
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
