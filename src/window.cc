#include "window.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "programcontroller.hh"
#include "log.hh"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Event event;

static int windowWidth;
static int windowHeight;

static SDL_Color currentColor = {0, 0, 0, 0};
static TTF_Font* currentFont = nullptr;
static int currentFontCharHeight = 0;
static int currentFontCharWidth = 0;

static const Uint8* keystate = nullptr;

bool SHH::Window::Init(int width, int height, std::string name)
{
    SHH::Log::Log("SHH::Window::Init(): Start.");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	SHH::Log::Error("SHH::Window::Init(): SDL_Init failed.");
	return false;
    }

    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
	SHH::Log::Error("SHH::Window::Init(): SDL_CreateWindow failed.");
	SDL_Quit();
	return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
	SHH::Log::Error("SHH::Window::Init(): SDL_CreateRenderer failed.");
	SDL_DestroyWindow(window);
	SDL_Quit();
	return false;
    }

    if (TTF_Init() == -1)
    {
	SHH::Log::Error("SHH::Window::Init(): TTF_Init() failed.");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return false;
    }
    
    windowWidth = width;
    windowHeight = height;

    keystate = SDL_GetKeyboardState(nullptr);

    SHH::Log::Log("SHH::Window::Init(): Ended successfully.");
    return true;
}

void SHH::Window::Deinit()
{
    SHH::Log::Log("SHH::Window::Deinit(): Start.");

    if (currentFont != nullptr)
    {
	TTF_CloseFont(currentFont);
    }

    TTF_Quit();

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
    currentColor = {r, g, b, a};
}

bool SHH::Window::LoadFont(std::string name, int size)
{
    if (currentFont != nullptr)
    {
	TTF_CloseFont(currentFont);
    }
    currentFont = TTF_OpenFont(name.c_str(), size);
    if (currentFont == nullptr)
    {
	SHH::Log::Error("SHH::Window::LoadFont(): Could not load font.");
	return false;
    }
    currentFontCharHeight = size;
    currentFontCharWidth = size;
    return true;
}

void SHH::Window::SetFontCharSize(int width, int height)
{
    currentFontCharWidth = width;
    currentFontCharHeight = height;
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

bool SHH::Window::DrawText(std::string text, int x, int y)
{
    if (x > windowWidth || y > windowHeight)
    {
	return true;
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(currentFont, text.c_str(), currentColor);
    if (textSurface == nullptr)
    {
	SHH::Log::Error("SHH::Window::DrawText(): TTF_RenderText_Solid failed.");
	return false;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr)
    {
	SHH::Log::Error("SHH::Window::DrawText(): SDL_CreateTextureFromSurface failed.");
	return false;
    }
    SDL_Rect rect = {x, y, (int)(currentFontCharWidth * text.length()), currentFontCharHeight};
    SDL_RenderCopy(renderer, textTexture, nullptr, &rect);
    return true;
}
