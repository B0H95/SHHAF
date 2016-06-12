#include "window.hh"

#include <SDL2/SDL.h>
#include "programcontroller.hh"
#include "log.hh"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Event event;

static int windowWidth;
static int windowHeight;

static const Uint8* keystate = nullptr;

bool SHH::Window::Init(int width, int height, std::string name)
{
    SHH::Log::Log("SHH::Window::Init(" + std::to_string(width) + ", " + std::to_string(height) + ", \"" + name + "\"): Start.");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	SHH::Log::Error("SHH::Window::Init(" + std::to_string(width) + ", " + std::to_string(height) + ", \"" + name + "\"): SDL_Init failed.");
	return false;
    }

    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
	SHH::Log::Error("SHH::Window::Init(" + std::to_string(width) + ", " + std::to_string(height) + ", \"" + name + "\"): SDL_CreateWindow failed.");
	SDL_Quit();
	return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
	SHH::Log::Error("SHH::Window::Init(" + std::to_string(width) + ", " + std::to_string(height) + ", \"" + name + "\"): SDL_CreateRenderer failed.");
	SDL_DestroyWindow(window);
	SDL_Quit();
	return false;
    }
    
    windowWidth = width;
    windowHeight = height;

    keystate = SDL_GetKeyboardState(nullptr);

    SHH::Log::Log("SHH::Window::Init(" + std::to_string(width) + ", " + std::to_string(height) + ", \"" + name + "\"): Ended successfully.");
    return true;
}

void SHH::Window::Deinit()
{
    SHH::Log::Log("SHH::Window::Deinit(): Start.");

    if (renderer != nullptr)
    {
	SDL_DestroyRenderer(renderer);
    }
    
    if (window != nullptr)
    {
	SDL_DestroyWindow(window);
    }
    SDL_Quit();

    SHH::Log::Log("SHH::Window::Deinit(): Ended successfully.");
}

void SHH::Window::ProcessEvents()
{
    while (SDL_PollEvent(&event) != 0)
    {
	if (event.type == SDL_QUIT)
	{
	    SHH::ProgramController::Quit();
	}
    }
}

bool SHH::Window::IsKeyDown(const char* keyname)
{
    return keystate[SDL_GetScancodeFromKey(SDL_GetKeyFromName(keyname))] == 1;
}

int SHH::Window::GetWindowWidth()
{
    return windowWidth;
}

int SHH::Window::GetWindowHeight()
{
    return windowHeight;
}

void SHH::Window::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void SHH::Window::ClearScreen()
{
    SDL_RenderClear(renderer);
}

void SHH::Window::UpdateScreen()
{
    SDL_RenderPresent(renderer);
}

void SHH::Window::DrawRectangle(int x1, int y1, int x2, int y2)
{
    SDL_Rect rect = {x1, y1, x2-x1, y2-y1};
    SDL_RenderDrawRect(renderer, &rect);
}

void SHH::Window::DrawFilledRectangle(int x1, int y1, int x2, int y2)
{
    SDL_Rect rect = {x1, y1, x2-x1, y2-y1};
    SDL_RenderFillRect(renderer, &rect);
}

void SHH::Window::DrawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void SHH::Window::DrawPoint(int x, int y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}
