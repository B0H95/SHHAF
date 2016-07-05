#include "window_resources.hh"

#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "log.hh"

static SDL_Renderer* renderer;

static std::unordered_map<std::string, SHH::Window::Resources::Font> fontList;
static std::unordered_map<std::string, SHH::Window::Resources::Texture> textureList;

static void UnloadResources();

bool SHH::Window::Resources::Init(SDL_Renderer* currentrenderer)
{
    SHH::Log::Log("Window::Resources::Init(): Started.");

    renderer = currentrenderer;

    if (TTF_Init() == -1)
    {
	SHH::Log::Error("Window::Resources::Init(): TTF_Init() failed.");
	return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
	SHH::Log::Error("Window::Resources::Init(): IMG_Init failed.");
	TTF_Quit();
	return false;
    }

    SHH::Log::Log("Window::Resources::Init(): Ended successfully.");
    return true;
}

void SHH::Window::Resources::Deinit()
{
    SHH::Log::Log("Window::Resources::Deinit(): Started.");
    
    UnloadResources();
    IMG_Quit();
    TTF_Quit();
    
    SHH::Log::Log("Window::Resources::Deinit(): Ended successfully.");
}

bool SHH::Window::Resources::LoadFont(std::string name, int height)
{
    if (fontList.find(name) != fontList.end())
    {
	SHH::Log::Log("Window::Resources::LoadFont(): Font \"" + name + "\" already loaded.");
	return true;
    }
    fontList[name].SetRenderer(renderer);
    if (!(fontList[name].Load(name, height)))
    {
	SHH::Log::Error("Window::Resources::LoadFont(): Could not load font \"" + name + "\".");
	fontList.erase(name);
	return false;
    }
    return true;
}

bool SHH::Window::Resources::LoadTexture(std::string name)
{
    if (textureList.find(name) != textureList.end())
    {
	SHH::Log::Log("Window::Resources::LoadTexture(): Texture \"" + name + "\" already loaded.");
	return true;
    }
    textureList[name].SetRenderer(renderer);
    if (!(textureList[name].Load(name)))
    {
	SHH::Log::Error("Window::Resources::LoadTexture(): Could not load texture \"" + name + "\".");
	textureList.erase(name);
	return false;
    }
    return true;
}

SHH::Window::Resources::Font* SHH::Window::Resources::GetFont(std::string name)
{
    std::unordered_map<std::string, SHH::Window::Resources::Font>::iterator iter = fontList.find(name);
    if (iter == fontList.end())
    {
	SHH::Log::Warning("Window::Resources::GetFont(): Font \"" + name + "\" not loaded.");
	return nullptr;
    }
    return &(iter->second);
}

SHH::Window::Resources::Texture* SHH::Window::Resources::GetTexture(std::string name)
{
    std::unordered_map<std::string, SHH::Window::Resources::Texture>::iterator iter = textureList.find(name);
    if (iter == textureList.end())
    {
	SHH::Log::Warning("Window::Resources::GetTexture(): Texture \"" + name + "\" not loaded.");
	return nullptr;
    }
    return &(iter->second);
}

static void UnloadResources()
{
    for (auto it = fontList.begin(); it != fontList.end(); ++it)
    {
	it->second.Unload();
    }

    for (auto it = textureList.begin(); it != textureList.end(); ++it)
    {
	it->second.Unload();
    }
}
