#pragma once
#include <unordered_map>
#include "objectPool.h"
#include "mathHelper.h"
#include "event.h"

class GameObject;
class MyEngine;
class Sprite;

struct AABB {
    Vector2 min;
    Vector2 max;

    AABB(Vector2 _min = Vector2(), Vector2 _max = Vector2())
        : min(_min), max(_max) {}
};

class Component
{
public:
    virtual ~Component();
    virtual void create(MyEngine* engine, GameObject * go);
    virtual void receive(Event event) {};
    virtual void init() {};
    virtual void start() {};
    virtual void update(float dt) = 0;
    virtual void destroy() {}
protected:
    MyEngine* engine_;
    GameObject * go_;
};

class RenderComponent : public Component
{
public:
    virtual void create(MyEngine* engine, GameObject * go, const char * sprite_name, bool clipped = false, int posX = 0, int posY = 0, int width = 0, int height = 0);
    virtual void update(float dt);
    virtual void destroy();

    Sprite * getSprite() { return sprite_; }
private:
    Sprite* sprite_;
};

class SpriteAnimationComponent : public Component 
{
public:
    struct Animation {
        int frame_count;
        bool looping;
        SDL_Rect* frames;

        Animation(int frame_count_, SDL_Rect* frames_, bool looping_ = false) : frame_count(frame_count_), frames(frames_), looping(looping_) {}
    };
    virtual void create(MyEngine* engine, GameObject* go, Sprite* sprite, int mspf);
    virtual void update(float dt);
    virtual void addAnimation(const char* name, int frame_count_, SDL_Rect* frames_, bool looping_ = false);
    virtual void addAnimation(const char* name, int frame_count_, int pos_x, int pos_y, int width, int height, int offset, bool looping_ = false);
    virtual bool playAnimation(const char* name, bool flip = false);
    virtual bool isRunning();
    virtual const char* currentAnimation();
    virtual int getMSPF();
protected:
    Sprite* sprite;
    std::unordered_map<const char*, Animation> animations;
    Animation* current;
    const char* current_name;
    int iterator;
    bool running;
    int mspf_;
    int ms_counter_;
};

class CollideComponent : public Component
{
public:
    virtual void create(MyEngine* engine, GameObject * go, ObjectPool<GameObject> * coll_objects, Vector2 min, Vector2 max);
    virtual void update(float dt);
    virtual AABB getBoundingBox();
    virtual bool AABBvsAABB(AABB a, AABB b, Vector2 * normal, float * depth);
private:
    ObjectPool<GameObject> * coll_objects_; // collision will be tested with these objects
    AABB relative_bounding_box_;
};

class RigidBodyComponent : public Component
{
public:
    virtual void create(MyEngine* engine, GameObject * go);
    virtual void receive(Event event);
    virtual void update(float dt);
    Vector2 getAcceleration();
    void setAcceleration(Vector2 acceleration);
    Vector2 getVelocity();
    void setVelocity(Vector2 velocity);
    void addForce(Vector2);

    void resolveCollision(RigidBodyComponent* other);
private:
    Vector2 velocity_;
    Vector2 acceleration_;
    Vector2 force_;
    Vector2 last_position_;
};

