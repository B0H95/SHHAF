#include "simulation_physics.hh"

#include "log.hh"
#include "programcontroller.hh"

static const float GRAVITY = 300.0f;

odirection checkObjectEnvironmentCollision(object const& obj, environment const& env);

bool SHH::Simulation::Physics::Init()
{
    SHH::Log::Log("SHH::Simulation::Physics::Init(): Started.");
    SHH::Log::Log("SHH::Simulation::Physics::Init(): Ended successfully.");
    return true;
}

void SHH::Simulation::Physics::Deinit()
{
    SHH::Log::Log("SHH::Simulation::Physics::Deinit(): Started.");
    SHH::Log::Log("SHH::Simulation::Physics::Deinit(): Ended successfully.");
}

void SHH::Simulation::Physics::ApplyPhysics(object* olist, int olistsize, environment* elist, int elistsize)
{
    //TODO: Fix standing on walls
    float frameTime = SHH::ProgramController::GetFrameTime();
    for (int i = 0; i < olistsize; ++i)
    {
	olist[i].xspeed += olist[i].xaccel * frameTime;
	if (olist[i].state == OS_FALLING)
	{
	    olist[i].yspeed += (olist[i].yaccel + GRAVITY) * frameTime;
	}
	else if (olist[i].state == OS_STANDING && olist[i].yaccel < 0.0f)
	{
	    olist[i].yspeed += (olist[i].yaccel + GRAVITY) * frameTime;
	    olist[i].state = OS_FALLING;
	}
	olist[i].x += olist[i].xspeed * frameTime;
	olist[i].y += olist[i].yspeed * frameTime;

	bool stillStanding = false;
	for (int j = 0; j < elistsize; ++j)
	{
	    odirection cdir = checkObjectEnvironmentCollision(olist[i], elist[j]);
	    switch (cdir)
	    {
	    case OD_NOWHERE:
		break;
	    case OD_UP:
		olist[i].y = elist[j].y + elist[j].height;
		if (olist[i].yspeed < 0.0f)
		{
		    olist[i].yspeed = 0.0f;
		}
		if (olist[i].yaccel < 0.0f)
		{
		    olist[i].yaccel = 0;
		}
		break;
	    case OD_RIGHT:
		olist[i].x = elist[j].x - olist[i].width;
		olist[i].xspeed = 0.0f;
		if (olist[i].xaccel > 0.0f)
		{
		    olist[i].xaccel = 0;
		}
		break;
	    case OD_DOWN:
		if (olist[i].state == OS_FALLING)
		{
		    olist[i].y = elist[j].y - olist[i].height;
		    olist[i].yspeed = 0.0f;
		    if (olist[i].yaccel >= 0.0f)
		    {
			olist[i].yaccel = 0.0f;
			olist[i].state = OS_STANDING;
			stillStanding = true;
		    }
		}
		break;
	    case OD_LEFT:
		olist[i].x = elist[j].x + elist[j].width;
		olist[i].xspeed = 0.0f;
		if (olist[i].xaccel < 0.0f)
		{
		    olist[i].xaccel = 0;
		}
		break;
	    default:
		break;
	    }
	}

        
	switch (olist[i].state)
	{
	case OS_IDLE:
	    olist[i].state = OS_FALLING;
	case OS_STANDING:
	    if (!stillStanding)
	    {
		olist[i].state = OS_FALLING;
	    }
	    break;
	case OS_FALLING:
	    break;
	default:
	    break;
        }
    }
}

odirection checkObjectEnvironmentCollision(object const& obj, environment const& env)
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
