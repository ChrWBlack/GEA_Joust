#pragma once
#include "component.h"
#include "gameObject.h"
#include "engine.h"

class Sprite;

class BlinkingComponent : public Component
{
public:
    void create(MyEngine* engine, GameObject* go, Sprite* sprite);
    virtual void update(float dt);
    void setActive(bool active);
private:
    Sprite * sprite_ = nullptr;
    bool active_ = false;
    const float interval_ = 0.07f;
    float counter_;
};

class PlayerBehaviourComponent : public Component
{
public:
    void create(MyEngine* engine, GameObject * go, int lives);
    virtual void init();
    virtual void start();
    virtual void update(float dt);
    virtual void receive(Event event);
    //void destroy();
    virtual int getLives();
    virtual int getScore();
    virtual void reduceLives();
    virtual void increaseScore(int value);
    virtual void setInvincible(float seconds);

private:
    const int movement_level_cap = 3;
    int movement_level_;
    MyEngine::KeyStatus last_keys_;
    const float key_hold_threshold_ = 0.2f;
    float key_hold_counter_;
    RigidBodyComponent * rigid_body_ = nullptr;
    SpriteAnimationComponent * animator_ = nullptr;
    BlinkingComponent * blinking_ = nullptr;
    Vector2 last_velocity_ = Vector2();
    bool left_facing_ = false;

    int lives_ = 0;
    int score_ = 0;

    bool invincible_ = false;
    float invincibility_time_ = 0;
};

class EnemyBehaviourComponent : public Component
{
public:
    void create(MyEngine* engine, GameObject* go, ObjectPool<GameObject>* eggs);
    virtual void init();
    virtual void start();
    virtual void update(float dt);
    virtual void receive(Event event);
    //void destroy();
    virtual void die();

private:
    const int movement_level_cap = 3;
    int movement_level_;
    RigidBodyComponent * rigid_body_ = nullptr;
    SpriteAnimationComponent * animator_ = nullptr;
    ObjectPool<GameObject> * eggs_ = nullptr;
    Vector2 last_velocity_ = Vector2();

    int direction = 0;
    bool left_facing_ = false;
};

class ScreenBoundsComponent : public Component {
public:
    void create(MyEngine* engine, GameObject* go, int screen_width);
    virtual void init();
    virtual void update(float dt);
private:
    int sprite_width_;
    int screen_width_;
    RigidBodyComponent * rigid_body_ = nullptr;
};

class EggBehaviourComponent : public Component
{
public:
    void create(MyEngine* engine, GameObject* go, ObjectPool<GameObject>* knights);
    virtual void init();
    virtual void update(float dt);
    virtual void receive(Event event);
private:
    RigidBodyComponent * rigid_body_ = nullptr;
    ObjectPool<GameObject> * knights_ = nullptr;
    const float respawn_time_ = 10;
    float time_counter_ = 0;
};

class PterodactylBehaviourComponent : public Component 
{
public:
    void create(MyEngine* engine, GameObject* go, GameObject* player);
    virtual void init();
    virtual void start();
    virtual void update(float dt);
    virtual void receive(Event event);
private:
    GameObject * player_;
    RigidBodyComponent * rigid_body_ = nullptr;
    SpriteAnimationComponent * animator_ = nullptr;

    int movement_speed_;
    Vector2 last_velocity_ = Vector2();
    int mspf_animator_;
    int ms_counter_;

    bool stunned_ = false;
    const float stunned_time_ = 1.25f;
    float stun_timer_;
};
