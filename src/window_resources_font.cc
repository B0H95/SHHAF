#include "window_resources_font.hh"

#include "log.hh"

SHH::Window::Resources::Font::Font() :
    renderer(nullptr),
    fontCharMap(nullptr),
    font(nullptr),
    charMapWidth(0),
    charMapHeight(0)
{
}

void SHH::Window::Resources::Font::SetRenderer(SDL_Renderer* r)
{
    renderer = r;
}

bool SHH::Window::Resources::Font::Load(std::string name, int height)
{
    if (renderer == nullptr)
    {
	SHH::Log::Error("Window::Resources::Font::Load(): No renderer.");	
	return false;
    }
    Unload();
    font = TTF_OpenFont(name.c_str(), height);
    if (font == nullptr)
    {
	SHH::Log::Error("Window::Resources::Font::Load(): Could not load font \"" + name + "\".");
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
    if (TTF_SizeText(font, charmapstr.c_str(), &charMapWidth, &charMapHeight) == -1)
    {
	SHH::Log::Error("Window::Resources::Font::Load(): Could not acquire font size.");
	TTF_CloseFont(font);
	return false;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, charmapstr.c_str(), fontcolor);
    if (textSurface == nullptr)
    {
	SHH::Log::Error("Window::Resources::Font::Load(): TTF_RenderText_Solid error.");
	TTF_CloseFont(font);
	return false;
    }
    fontCharMap = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (fontCharMap == nullptr)
    {
	SHH::Log::Error("Window::Resources::Font::Load(): SDL_CreateTextureFromSurface error.");
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
	return false;
    }
    SDL_FreeSurface(textSurface);
    return true;
}

void SHH::Window::Resources::Font::Unload()
{
    if (font != nullptr)
    {
	TTF_CloseFont(font);
    }
    if (fontCharMap != nullptr)
    {
	SDL_DestroyTexture(fontCharMap);
    }
}

SDL_Texture* SHH::Window::Resources::Font::GetCharMap()
{
    return fontCharMap;
}

int SHH::Window::Resources::Font::GetCharWidth()
{
    return charMapWidth / 128;
}

int SHH::Window::Resources::Font::GetCharHeight()
{
    return charMapHeight;
}
