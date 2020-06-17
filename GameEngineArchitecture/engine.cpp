#include "mathHelper.h"
#include "sprite.h"
#include "objectPool.h"
#include "component.h"
#include "gameObject.h"
#include "engine.h"

MyEngine::~MyEngine()
{
}

bool MyEngine::initialize(int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed the initialization: %s\n", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow("ok ENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window_ == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_ == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL image failed initialization! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL mixer failed initialization! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // initialize the keys
    keys_.up = false;    keys_.down = false;    keys_.left = false,    keys_.right = false;    keys_.space = false;    keys_.escp = false;

    startTime_ = 0;
    mspf_ = 0;

    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer_);

    asset_manager_ = AssetManager();

    SDL_Log("Engine successfully initialized!");

    return true;
}

void MyEngine::gameLoop()
{
    for (int i = 0; i < numEntities_; ++i) {
        entities_[i]->start();
    }

    while (true) {
        int lastTime = startTime_;
        startTime_ = getElapsedTime();
        float deltaTime = (float)(startTime_ - lastTime) / 1000.f;

        //handle input
        pollInput();

        if (keys_.escp) {
            quit();
        }

        //update entities
        for (int i = 0; i < numEntities_; ++i) {
            entities_[i]->update(deltaTime);
        }

        //physics, rendering,...
        SDL_SetRenderDrawColor(renderer_, background_.r, background_.g, background_.b, background_.a);

        swapBuffers();
        clearWindow();

        int delayTime = mspf_ - (getElapsedTime() - startTime_);
        if (delayTime > 0) {
            SDL_Delay(delayTime);
        }
    }
}

void MyEngine::destroy()
{
    for (int i = 0; i < numEntities_; ++i) {
        entities_[i]->destroy();
    }

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    IMG_Quit();
    SDL_Quit();
}

void MyEngine::pollInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) 
        {
            quit();
            break;
        }
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                keys_.escp = true;
                break;
            case SDLK_UP:
                keys_.up = true;
                break;
            case SDLK_DOWN:
                keys_.down = true;
                break;
            case SDLK_LEFT:
                keys_.left = true;
                break;
            case SDLK_RIGHT:
                keys_.right = true;
                break;
            case SDLK_SPACE:
                keys_.space = true;
                break;
            }
        }

        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                keys_.escp = false;
                break;
            case SDLK_UP:
                keys_.up = false;
                break;
            case SDLK_DOWN:
                keys_.down = false;
                break;
            case SDLK_LEFT:
                keys_.left = false;
                break;
            case SDLK_RIGHT:
                keys_.right = false;
                break;
            case SDLK_SPACE:
                keys_.space = false;
                break;
            }
        }
    }
}

void MyEngine::setBackgroundColor(int r, int g, int b, int a)
{
    background_ = Color(r, g, b, a);
}

void MyEngine::setMSPerFrame(int ms) {
    mspf_ = ms;
}

void MyEngine::quit()
{
    destroy();
    exit(0);
}

void MyEngine::swapBuffers()
{
    SDL_RenderPresent(renderer_);
}

void MyEngine::clearWindow()
{
    SDL_RenderClear(renderer_);
}

int MyEngine::getElapsedTime()
{
    return SDL_GetTicks();
}

MyEngine::KeyStatus MyEngine::getKeyStates()
{
    return keys_;
}

GameObject* MyEngine::createObject()
{
    GameObject* entity = new GameObject(++numEntities_);
    entities_.push_back(entity);
    return entities_.back();
}

Sprite * MyEngine::createSprite(const char * path, bool clipped, int posX, int posY, int width, int height)
{
    SDL_Texture * texture = asset_manager_.LoadTexture(path, renderer_);

    if (texture == nullptr) {
        return NULL;
    }

    Sprite * sprite = new Sprite(renderer_, texture, clipped, posX, posY, width, height);

    return sprite;
}
