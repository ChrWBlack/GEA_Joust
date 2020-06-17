#pragma once
#include "SDL.h"

class Sprite
{
public:
    Sprite(SDL_Renderer * renderer, SDL_Texture * texture, bool clipped = false, int posX = 0, int posY = 0, int width = 0, int height = 0);

    void destroy();

    virtual void draw(int x, int y);

    void setClipRect(int posX, int posY, int width, int height);

    bool getSpriteSize(int * w, int * h);

    void flipSprite(SDL_RendererFlip flip);

    void hide(bool hide);
    bool isHidden();

protected:
    SDL_Renderer * renderer;
    SDL_Texture * texture;
    bool clipped_;
    SDL_Rect srcRect_;
    SDL_RendererFlip flip_;
    bool hidden_;
};
