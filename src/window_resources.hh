#pragma once

#include "window_resources_font.hh"

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
	    SHH::Window::Resources::Font* GetCurrentFont();
	}
    }
}
