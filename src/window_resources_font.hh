#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace SHH
{
    namespace Window
    {
	namespace Resources
	{
	    class Font
	    {
	    public:
		Font();
		void SetRenderer(SDL_Renderer* r);
		bool Load(std::string name, int height);
		void Unload();
		SDL_Texture* GetCharMap();
		int GetCharWidth();
		int GetCharHeight();

	    private:
		SDL_Renderer* renderer;
		SDL_Texture* fontCharMap;
		TTF_Font* font;
		int charMapWidth;
		int charMapHeight;
	    };
	}
    }
}
