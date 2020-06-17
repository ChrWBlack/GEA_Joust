#pragma once
#include <unordered_map>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    SDL_Texture* LoadTexture(const char* path, SDL_Renderer * renderer);
    Mix_Chunk* LoadSoundEffect(const char* path);

private:
    std::unordered_map<const char*, SDL_Texture*> textures_;
    std::unordered_map<const char*, Mix_Chunk*> sounds_;
};
