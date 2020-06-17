#pragma once
#include "component.h"
#include "gameObject.h"
#include "engine.h"

class LevelBehaviourComponent : public Component
{
public:
    LevelBehaviourComponent();
    ~LevelBehaviourComponent();
    void create(MyEngine* engine, GameObject* go);
    virtual void init();
    virtual void update(float dt);
    virtual void receive(Event event);
private:
};

class LavaBehaviourComponent : public Component 
{
public:
    LavaBehaviourComponent();
    ~LavaBehaviourComponent();
    void create(MyEngine* engine, GameObject* go);
    virtual void update(float dt);
    virtual void receive(Event event);
};
