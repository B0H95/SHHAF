#pragma once

#include <cstdint>

enum ostate
{
    IDLE
};

enum odirection
{
    NOWHERE,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum otype
{
    NONE,
    BASIC
};

enum mstype
{
    OBJECTUPDATE
};

enum mctype
{
    NOTHING,
    RUNLEFT,
    RUNRIGHT
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
