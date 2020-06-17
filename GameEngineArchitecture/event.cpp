#include "event.h"

Event::Event(int eventType, Message * message)
{
    this->eventType_ = eventType;
    this->message_ = message;
}

Event::~Event()
{
}

int Event::getEventType()
{
    return eventType_;
}

Message * Event::getMessage()
{
    return message_;
}

Message::~Message()
{
}

Event Message::generateEvent()
{
    Event e = Event(eventType_, this);
    return e;
}

void CollideMessage::create(GameObject * other, Vector2 normal, float depth)
{
    this->other_ = other;
    this->normal_ = normal;
    this->depth_ = depth;
    eventType_ = COLLISION;
}

GameObject * CollideMessage::getOther()
{
    return other_;
}

Vector2 CollideMessage::getCollisionNormal()
{
    return normal_;
}

float CollideMessage::getPenetrationDepth()
{
    return depth_;
}

void HitMessage::create(GameObject * other)
{
    this->other_ = other;
    eventType_ = HIT;
}

GameObject * HitMessage::getOther()
{
    return other_;
}

void AnimationCompleteMessage::create(const char * name)
{
    this->name_ = name;
    eventType_ = ANIMATION_COMPLETE;
}

const char * AnimationCompleteMessage::getName()
{
    return name_;
}
