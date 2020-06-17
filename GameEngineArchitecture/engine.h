#pragma once
#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "assetManager.h"

#define GRAVITY 30.0f

class GameObject;
class Sprite;

class MyEngine
{
public:
    MyEngine() : numEntities_(0) {}
    ~MyEngine();

    struct KeyStatus
    {
        bool escp;
        bool up;
        bool down;
        bool left;
        bool right;
        bool space;
    };

    struct Color {
        int r;
        int g;
        int b;
        int a;

        Color(int _r = 255, int _g = 255, int _b = 255, int _a = 255)
            : r(_r), g(_g), b(_b), a(_a) {}
    };

    bool initialize(int width, int height);
    void gameLoop();
    void destroy();
    void pollInput();
    void setBackgroundColor(int r, int g, int b, int a);
    void setMSPerFrame(int ms);
    void quit();
    void swapBuffers();
    void clearWindow();
    int getElapsedTime();
    KeyStatus getKeyStates();
    GameObject* createObject();
    Sprite* createSprite(const char * path, bool clipped = false, int posX = 0, int posY = 0, int width = 0, int height = 0);

private:
    std::vector<GameObject*> entities_;
    int numEntities_;

    int startTime_; //in milliseconds
    int mspf_;

    SDL_Window * window_;
    SDL_Renderer * renderer_;

    KeyStatus keys_;
    Color background_;

    AssetManager asset_manager_;
};

