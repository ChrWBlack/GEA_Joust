#include "assetManager.h"

AssetManager::AssetManager()
{
}


AssetManager::~AssetManager()
{
}

SDL_Texture * AssetManager::LoadTexture(const char * path, SDL_Renderer * renderer)
{
    if (textures_.find(path) == textures_.end()) {
        SDL_Surface* surf = IMG_Load(path);
        if (surf == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load image %s! SDL_image Error: %s\n", path, SDL_GetError());
            return nullptr;
        }

        //Create texture from surface pixels
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surf);
        if (texture == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
            return nullptr;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(surf);

        textures_.insert({ path, texture });
    }
    return textures_.at(path);
}

Mix_Chunk * AssetManager::LoadSoundEffect(const char * path)
{
    if (sounds_.find(path) == sounds_.end()) {
        Mix_Chunk* sound = Mix_LoadWAV(path);
        if (sound == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load sound from %s! SDL Error: %s\n", path, SDL_GetError());
            return nullptr;
        }

        sounds_.insert({ path, sound });
    }
    return sounds_.at(path);
}
