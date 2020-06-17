#include "sprite.h"
#include "engine.h"

Sprite::Sprite(SDL_Renderer * renderer, SDL_Texture * texture, bool clipped, int posX, int posY, int width, int height)
{
    this->renderer = renderer;
    this->texture = texture;

    this->clipped_ = clipped;
    this->srcRect_.x = posX;
    this->srcRect_.y = posY;
    this->srcRect_.w = width;
    this->srcRect_.h = height;
    this->flip_ = SDL_FLIP_NONE;
}

void Sprite::draw(int x, int y)
{
    if (hidden_) {
        return;
    }
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    
    if (clipped_) {
        rect.w = srcRect_.w * 2;
        rect.h = srcRect_.h * 2;
    }
    else {
        SDL_QueryTexture(texture, NULL, NULL, &(rect.w), &(rect.h)); // retrieve width and height from texture
    }

    //Render texture to screen
    SDL_RenderCopyEx(renderer, texture, clipped_ ? &srcRect_ : NULL, &rect, 0, NULL, flip_);
}

void Sprite::setClipRect(int posX, int posY, int width, int height)
{
    if (!clipped_) {
        return;
    }

    this->srcRect_.x = posX;
    this->srcRect_.y = posY;
    this->srcRect_.w = width;
    this->srcRect_.h = height;
}

bool Sprite::getSpriteSize(int * w, int * h)
{
    if (clipped_) {
        *w = srcRect_.w * 2;
        *h = srcRect_.h * 2;
        return true;
    }
    else {
        Uint32 format = 0;
        int result, access = 0;
        result = SDL_QueryTexture(texture, &format, &access, w, h);
        return result == 0;
    }
}

void Sprite::flipSprite(SDL_RendererFlip flip) {
    this-> flip_ = flip;
}

void Sprite::hide(bool hide)
{
    this->hidden_ = hide;
}

bool Sprite::isHidden()
{
    return hidden_;
}


void Sprite::destroy()
{
    SDL_DestroyTexture(texture);
}