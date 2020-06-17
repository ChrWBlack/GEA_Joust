#pragma once
#include "mathHelper.h"

enum EventType { DEFAULT, HIT, COLLISION, ANIMATION_COMPLETE, UNUSED_2, UNUSED_3, UNUSED_4, UNUSED_5, UNUSED_6};

class GameObject;
class Message;

class Event
{
public:
    Event(int eventType, Message * message);
    ~Event();
    int getEventType();
    Message* getMessage();
private:
    int eventType_;
    Message * message_;
};

class Message
{
public:
    virtual ~Message();
    virtual void create() {};
    Event generateEvent();
protected:
    int eventType_ = DEFAULT;
};

class CollideMessage : public Message
{
public:
    virtual void create(GameObject * other, Vector2 normal, float depth);
    GameObject* getOther();
    Vector2 getCollisionNormal();
    float getPenetrationDepth();
protected:
    GameObject * other_;
    Vector2 normal_;
    float depth_;
};

class HitMessage : public Message 
{
public:
    virtual void create(GameObject * other);
    GameObject* getOther();
protected:
    GameObject * other_;
};

class AnimationCompleteMessage : public Message 
{
public:
    virtual void create(const char* name);
    const char* getName();
protected:
    const char* name_;
};
