#include "window_resources_texture.hh"

#include <SDL2/SDL_image.h>
#include "log.hh"

SHH::Window::Resources::Texture::Texture() :
    renderer(nullptr),
    texture(nullptr),
    surface(nullptr)
{
}

void SHH::Window::Resources::Texture::SetRenderer(SDL_Renderer* r)
{
    renderer = r;
}

bool SHH::Window::Resources::Texture::Load(std::string name)
{
    if (renderer == nullptr)
    {
	SHH::Log::Error("Window::Resources::Texture::Load(): No renderer.");
	return false;
    }

    surface = IMG_Load(name.c_str());
    if (surface == nullptr)
    {
	SHH::Log::Error("Window::Resources::Texture::Load(): IMG_Load error.");
	return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
	SHH::Log::Error("Window::Resources::Texture::Load(): SDL_CreateTextureFromSurface error.");
	SDL_FreeSurface(surface);
	surface = nullptr;
	return false;
    }

    SDL_FreeSurface(surface);
    surface = nullptr;
    
    return true;
}

void SHH::Window::Resources::Texture::Unload()
{
    if (texture != nullptr)
    {
	SDL_DestroyTexture(texture);
	texture = nullptr;
    }
}

SDL_Texture* SHH::Window::Resources::Texture::GetTexture()
{
    return texture;
}
