#include "window_resources.hh"

#include <SDL2/SDL_ttf.h>
#include "log.hh"

static SDL_Renderer* renderer;

static TTF_Font* font = nullptr;
static SDL_Texture* fontCharMap = nullptr;
static int charMapWidth = 0;
static int charHeight = 0;

bool SHH::Window::Resources::Init(SDL_Renderer* currentrenderer)
{
    SHH::Log::Log("Window::Resources::Init(): Started.");

    renderer = currentrenderer;

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

    TTF_Quit();
    
    SHH::Log::Log("Window::Resources::Deinit(): Ended successfully.");
}

bool SHH::Window::Resources::LoadFont(std::string name, int height)
{
    if (font != nullptr)
    {
	TTF_CloseFont(font);
    }
    font = TTF_OpenFont(name.c_str(), height);
    if (font == nullptr)
    {
	SHH::Log::Error("Window::Resources::LoadFont(): Could not load font \"" + name + "\".");
	return false;
    }

    std::string charmapstr = "";
    for (char i = 0; i < 127; ++i)
    {
	if (i >= 33 && i <= 126)
	{
	    charmapstr += i;
	}
	else
	{
	    charmapstr += " ";
	}
    }
    charmapstr += " ";

    SDL_Color fontcolor = {0xFF, 0xFF, 0xFF, 0xFF};    
    if (TTF_SizeText(font, charmapstr.c_str(), &charMapWidth, &charHeight) == -1)
    {
	SHH::Log::Error("Window::Resources::LoadFont(): Could not acquire font size.");
	TTF_CloseFont(font);
	return false;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, charmapstr.c_str(), fontcolor);
    if (textSurface == nullptr)
    {
	SHH::Log::Error("Window::Resources::LoadFont(): TTF_RenderText_Solid error.");
	TTF_CloseFont(font);
	return false;
    }
    fontCharMap = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (fontCharMap == nullptr)
    {
	SHH::Log::Error("Window::Resources::LoadFont(): SDL_CreateTextureFromSurface error.");
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
	return false;
    }
    return true;
}

SDL_Texture* SHH::Window::Resources::GetFontCharMap()
{
    return fontCharMap;
}

int SHH::Window::Resources::GetFontWidth()
{
    return charMapWidth / 128;
}

int SHH::Window::Resources::GetFontHeight()
{
    return charHeight;
}
