#include "programcontroller.hh"

#include <cstring>
#include <chrono>
#include <thread>
#include "log.hh"
#include "window.hh"
#include "ui.hh"
#include "simulation.hh"

static bool running = true;
static int argumentCount;
static char** arguments;
static float frameLoad = 0.0f;
static float frameTime = 0.0f;
static float frameRate = 60.0;
static float frameRateTime = 1.0f / 60.0f;
static std::chrono::high_resolution_clock::time_point startTime;
static std::chrono::high_resolution_clock::time_point endTime;

bool SHH::ProgramController::Init(int argc, char* argv[])
{
    SHH::Log::Log("SHH::ProgramController::Init(): Start.");
    
    argumentCount = argc;
    arguments = argv;

    if (!SHH::Window::Init(800, 600, "Super Heroin Hobo Arena Fighter!"))
    {
	SHH::Log::Error("SHH::ProgramController::Init(): Could not init window.");
	return false;
    }

    if (!SHH::UI::Init())
    {
	SHH::Log::Error("SHH::ProgramController::Init(): Could not init UI.");
	SHH::Window::Deinit();
	return false;
    }

    if (!SHH::Simulation::Init())
    {
	SHH::Log::Error("SHH::ProgramController::Init(): Could not init simulation.");
	SHH::UI::Deinit();
	SHH::Window::Deinit();
	return false;
    }

    SHH::Log::Log("SHH::ProgramController::Init(): Ended successfully.");
    return true;
}

void SHH::ProgramController::Deinit()
{
    SHH::Log::Log("SHH::ProgramController::Deinit(): Start.");

    SHH::Simulation::Deinit();
    SHH::UI::Deinit();
    SHH::Window::Deinit();

    SHH::Log::Log("SHH::ProgramController::Deinit(): Ended successfully.");
}

void SHH::ProgramController::Run()
{
    SHH::Log::Log("SHH::ProgramController::Run(): Start.");

    while (running)
    {
	startTime = std::chrono::high_resolution_clock::now();

	SHH::Window::ProcessEvents();	
	SHH::UI::ProcessInputs();
	SHH::Simulation::Update();
	SHH::UI::Draw();

	endTime = std::chrono::high_resolution_clock::now();
	frameTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();
	frameLoad = frameTime * frameRate;
	if (frameTime < frameRateTime)
	{
	    std::this_thread::sleep_for(std::chrono::microseconds(int((frameRateTime - frameTime) * 1000000.0f)));
	    frameTime = frameRateTime;
	}
    }

    SHH::Log::Log("SHH::ProgramController::Run(): Ended successfully.");
}

void SHH::ProgramController::Quit()
{
    SHH::Log::Log("SHH::ProgramController::Quit(): Quit called.");
    running = false;
}

float SHH::ProgramController::GetFrameTime()
{
    return frameTime;
}

float SHH::ProgramController::GetFrameLoad()
{
    return frameLoad;
}

bool SHH::ProgramController::GetParameter(const char* param)
{
    for (int i = 1; i < argumentCount; ++i)
    {
	if (strcmp(param, arguments[i]) == 0)
	{
	    return true;
	}
    }
    return false;
}

void SHH::ProgramController::SetFramerate(int rate)
{
    frameRate = (float)rate;
    frameRateTime = 1.0f / (float)rate;
}
