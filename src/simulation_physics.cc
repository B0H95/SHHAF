#include "simulation_physics.hh"

#include "log.hh"
#include "programcontroller.hh"

static const float GRAVITY = 300.0f;

static odirection checkObjectEnvironmentCollision(object const& obj, environment const& env);
static void handleBehaviorRequest(object& obj, object& request);
static void updatePosition(object& obj, float ftime);
static void handleEnvironmentCollisions(object& obj, environment* elist, int elistsize);

bool SHH::Simulation::Physics::Init()
{
    SHH::Log::Log("Simulation::Physics::Init(): Started.");
    SHH::Log::Log("Simulation::Physics::Init(): Ended successfully.");
    return true;
}

void SHH::Simulation::Physics::Deinit()
{
    SHH::Log::Log("Simulation::Physics::Deinit(): Started.");
    SHH::Log::Log("Simulation::Physics::Deinit(): Ended successfully.");
}

void SHH::Simulation::Physics::ApplyPhysics(object* olist, object* obrlist, int olistsize, environment* elist, int elistsize)
{
    float frameTime = SHH::ProgramController::GetFrameTime();
    for (int i = 0; i < olistsize; ++i)
    {
	handleBehaviorRequest(olist[i], obrlist[i]);
	updatePosition(olist[i], frameTime);
	handleEnvironmentCollisions(olist[i], elist, elistsize);	
    }
}

static odirection checkObjectEnvironmentCollision(object const& obj, environment const& env)
{
    float right = env.x - (obj.x+obj.width);
    float left = obj.x - (env.x+env.width);
    float down = env.y - (obj.y+obj.height);
    float up = obj.y - (env.y+env.height);
    if (right < 0.0f && left < 0.0f && up < 0.0f && down < 0.0f)
    {
	if (right >= left && right >= up && right >= down) return OD_RIGHT;
	else if (left >= right && left >= up && left >= down) return OD_LEFT;
	else if (up >= left && up >= right && up >= down) return OD_UP;
	else return OD_DOWN;
    }
    return OD_NOWHERE;
}

static void handleBehaviorRequest(object& obj, object& request)
{
    obj.xspeed = request.xspeed;
    obj.xaccel = request.xaccel;
    if (request.yspeed < 0.0f && obj.state == OS_STANDING)
    {
	obj.yspeed = request.yspeed;
    }
}

static void updatePosition(object& obj, float ftime)
{
    if (obj.state == OS_IDLE)
    {
	obj.state = OS_FALLING;
    }
    obj.xspeed += obj.xaccel * ftime;
    if (obj.state == OS_FALLING)
    {
	obj.yaccel = GRAVITY;
	obj.yspeed += obj.yaccel * ftime;
    }
    else if (obj.state == OS_STANDING)
    {
	if (obj.yspeed < 0.0f)
	{
	    obj.state = OS_FALLING;
	}
	else
	{
	    obj.yaccel = 0.0f;
	    obj.yspeed = 0.0f;
	}
    }
    obj.x += obj.xspeed * ftime;
    obj.y += obj.yspeed * ftime;
}

static void handleEnvironmentCollisions(object& obj, environment* elist, int elistsize)
{
    bool stillStanding = false;
    for (int j = 0; j < elistsize; ++j)
    {
	environment& env = elist[j];
	odirection cdir = checkObjectEnvironmentCollision(obj, env);
	switch (cdir)
	{
	case OD_NOWHERE:
	    break;
	case OD_UP:
	    obj.y = env.y + env.height;
	    if (obj.yspeed < 0.0f)
	    {
		obj.yspeed = 0.0f;
	    }
	    break;
	case OD_RIGHT:
	    obj.x = env.x - obj.width;
	    obj.xspeed = 0.0f;
	    if (obj.xaccel > 0.0f)
	    {
		obj.xaccel = 0;
	    }
	    break;
	case OD_DOWN:
	    stillStanding = true;
	    if (obj.state == OS_FALLING && obj.yspeed > 0.0f)
	    {
		obj.y = env.y - obj.height;
		obj.yspeed = 0.0f;
		obj.yaccel = 0.0f;
		obj.state = OS_STANDING;
	    }
	    break;
	case OD_LEFT:
	    obj.x = env.x + env.width;
	    obj.xspeed = 0.0f;
	    if (obj.xaccel < 0.0f)
	    {
		obj.xaccel = 0;
	    }
	    break;
	default:
	    break;
	}
    }

    if (obj.state == OS_STANDING && !stillStanding)
    {
	obj.state = OS_FALLING;
    }
}
