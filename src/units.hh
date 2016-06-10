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

enum mtype
{
    OBJECTUPDATE
};

struct object
{
    otype type;
    ostate state;
    odirection direction;
    float xaccel,yaccel,xspeed,yspeed,x,y,width,height;
};

struct message
{
    uint8_t messagetype;
    uint8_t objecttype;
    uint8_t direction;
    uint8_t state;
    uint32_t index;
    float x,y,xspeed,yspeed,xaccel,yaccel,width,height;
    uint8_t strsize;
    char* str;
    uint32_t checksum;
};

struct queuemessage
{
    message msg;
    bool fresh;
};
