#include "window.hh"

#include "window_resources.hh"
#include "window_resources_font.hh"
#include "window_resources_texture.hh"

#include <SDL2/SDL.h>
#include "programcontroller.hh"
#include "log.hh"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Event event;

static int windowWidth;
static int windowHeight;

static SDL_Color currentColor = {0, 0, 0, 0};
static int fontCharDrawHeight = 0;
static int fontCharDrawWidth = 0;

static const Uint8* keystate = nullptr;
static Uint8 previousKeystates [SDL_NUM_SCANCODES];
static SDL_Keycode currentKey = 0;

bool SHH::Window::Init(int width, int height, std::string name)
{
    SHH::Log::Log("Window::Init(): Start.");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	SHH::Log::Error("Window::Init(): SDL_Init failed.");
	return false;
    }

    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
	SHH::Log::Error("Window::Init(): SDL_CreateWindow failed.");
	SDL_Quit();
	return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
	SHH::Log::Error("Window::Init(): SDL_CreateRenderer failed.");
	SDL_DestroyWindow(window);
	SDL_Quit();
	return false;
    }

    if (!SHH::Window::Resources::Init(renderer))
    {
	SHH::Log::Error("Window::Init(): Could not init resources.");
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    windowWidth = width;
    windowHeight = height;

    keystate = SDL_GetKeyboardState(nullptr);
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
	previousKeystates[i] = 0;
    }

    SHH::Log::Log("Window::Init(): Ended successfully.");
    return true;
}

void SHH::Window::Deinit()
{
    SHH::Log::Log("Window::Deinit(): Start.");
    
    SHH::Window::Resources::Deinit();

    if (renderer != nullptr)
    {
	SDL_DestroyRenderer(renderer);
    }
    
    if (window != nullptr)
    {
	SDL_DestroyWindow(window);
    }

    SDL_Quit();

    SHH::Log::Log("Window::Deinit(): Ended successfully.");
}

void SHH::Window::ProcessEvents()
{
    currentKey = 0;
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
	previousKeystates[i] = keystate[i];
    }
    while (SDL_PollEvent(&event) != 0)
    {
	if (event.type == SDL_QUIT)
	{
	    SHH::ProgramController::Quit();
	}
	else if (event.type == SDL_KEYDOWN)
	{
	    currentKey = event.key.keysym.sym;
	}
    }
}

bool SHH::Window::IsKeyDown(const char* keyname)
{
    return keystate[SDL_GetScancodeFromKey(SDL_GetKeyFromName(keyname))] == 1;
}

bool SHH::Window::IsKeyPressed(const char* keyname)
{
    int scancode = SDL_GetScancodeFromKey(SDL_GetKeyFromName(keyname));
    return keystate[scancode] == 1 && previousKeystates[scancode] == 0;
}

bool SHH::Window::IsKeyReleased(const char* keyname)
{
    int scancode = SDL_GetScancodeFromKey(SDL_GetKeyFromName(keyname));
    return keystate[scancode] == 0 && previousKeystates[scancode] == 1;
}

std::string SHH::Window::GetCurrentKey()
{
    if (currentKey == 0)
    {
	return "";
    }
    return SDL_GetKeyName(currentKey);
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
    if (!SHH::Window::Resources::LoadFont(name, size))
    {
	SHH::Log::Error("Window::LoadFont(): Could not load font.");
	return false;
    }
    return true;
}

bool SHH::Window::LoadTexture(std::string name)
{
    if (!SHH::Window::Resources::LoadTexture(name))
    {
	SHH::Log::Error("Window::LoadTexture(): Could not load texture.");
	return false;
    }
    return true;
}

void SHH::Window::SetFontCharSize(int width, int height)
{
    fontCharDrawWidth = width;
    fontCharDrawHeight = height;
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

void SHH::Window::DrawText(std::string fontname, std::string text, int x, int y)
{
    if (x > windowWidth || y > windowHeight || text.length() <= 0)
    {
	return;
    }

    SHH::Window::Resources::Font* font = SHH::Window::Resources::GetFont(fontname);
    if (font == nullptr)
    {
	return;
    }
    
    int charWidth = font->GetCharWidth();
    SDL_Texture* charmap = font->GetCharMap();
    SDL_Rect rect = {0, y, fontCharDrawWidth, fontCharDrawHeight};
    SDL_Rect charmappos = {0, 0, charWidth, font->GetCharHeight()};

    for (int i = 0; i < (int)text.length(); ++i)
    {
	rect.x = x + (charWidth * i);

	if (rect.x >= windowWidth)
	{
	    return;
	}

	charmappos.x = (int)text[i] * charWidth;
	SDL_RenderCopy(renderer, charmap, &charmappos, &rect);
    }
    
    return;
}

void SHH::Window::DrawTexture(std::string name, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SHH::Window::Resources::Texture* tx = SHH::Window::Resources::GetTexture(name);
    if (tx == nullptr)
    {
	return;
    }
    
    SDL_Texture* sdltx = tx->GetTexture();
    SDL_RenderCopy(renderer, sdltx, nullptr, &rect);
}
