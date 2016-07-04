#pragma once

#include <string>
#include <SDL2/SDL.h>

namespace SHH
{
    namespace Window
    {
	namespace Resources
	{
	    class Texture
	    {
	    public:
		Texture();
		void SetRenderer(SDL_Renderer* r);
		bool Load(std::string name);
		void Unload();
		SDL_Texture* GetTexture();

	    private:
		SDL_Renderer* renderer;
		SDL_Texture* texture;
		SDL_Surface* surface;
	    };
	}
    }
}
