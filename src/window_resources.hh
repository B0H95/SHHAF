#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace SHH
{
    namespace Window
    {
	namespace Resources
	{
	    bool Init(SDL_Renderer* currentrenderer);
	    void Deinit();
	    bool LoadFont(std::string name, int height);
	    SDL_Texture* GetFontCharMap();
	    int GetFontWidth();
	    int GetFontHeight();
	}
    }
}
