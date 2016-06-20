#include "window_resources.hh"

#include <SDL2/SDL_ttf.h>
#include "log.hh"

static SDL_Renderer* renderer;

static SHH::Window::Resources::Font currentFont;

bool SHH::Window::Resources::Init(SDL_Renderer* currentrenderer)
{
    SHH::Log::Log("Window::Resources::Init(): Started.");

    renderer = currentrenderer;
    currentFont.SetRenderer(currentrenderer);

    if (TTF_Init() == -1)
    {
	SHH::Log::Error("Window::Resources::Init(): TTF_Init() failed.");
	return false;
    }

    SHH::Log::Log("Window::Resources::Init(): Ended successfully.");
    return true;
}

void SHH::Window::Resources::Deinit()
{
    SHH::Log::Log("Window::Resources::Deinit(): Started.");
    
    currentFont.Unload();
    TTF_Quit();
    
    SHH::Log::Log("Window::Resources::Deinit(): Ended successfully.");
}

bool SHH::Window::Resources::LoadFont(std::string name, int height)
{
    if (!currentFont.Load(name, height))
    {
	SHH::Log::Error("Window::Resources::LoadFont(): Could not load font \"" + name + "\".");
	return false;
    }
    return true;
}

SHH::Window::Resources::Font* SHH::Window::Resources::GetCurrentFont()
{
    return &currentFont;
}
