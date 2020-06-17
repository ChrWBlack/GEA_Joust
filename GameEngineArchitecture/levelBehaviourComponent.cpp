#include "levelBehaviourComponent.h"



LevelBehaviourComponent::LevelBehaviourComponent()
{
}


LevelBehaviourComponent::~LevelBehaviourComponent()
{
}

void LevelBehaviourComponent::create(MyEngine * engine, GameObject * go)
{
    Component::create(engine, go);
}

void LevelBehaviourComponent::init()
{
}

void LevelBehaviourComponent::update(float dt)
{
}

void LevelBehaviourComponent::receive(Event event)
{
    GameObject * other;
    Vector2 normal;
    float depth;
    Vector2 velocity;
    RigidBodyComponent* rigid_body = nullptr;
    switch (event.getEventType()) {
    case COLLISION:
        other = ((CollideMessage*)(event.getMessage()))->getOther();
        normal = ((CollideMessage*)(event.getMessage()))->getCollisionNormal();
        depth = ((CollideMessage*)(event.getMessage()))->getPenetrationDepth();
        rigid_body = other->getComponent<RigidBodyComponent>();
        other->setPosition(other->getPosition() + normal * depth);
        if (rigid_body != nullptr) {
            velocity = rigid_body->getVelocity();
            if (normal.y == -1) {
                rigid_body->setVelocity(Vector2(velocity.x, 0));
            }
            else {
                rigid_body->setVelocity(velocity - (normal * velocity.dotProduct(normal) * 2));
            }
        }
        break;
    case DEFAULT:
        SDL_Log("%i : NOPE!", go_->getID());
        break;
    }
}

LavaBehaviourComponent::LavaBehaviourComponent()
{
}

LavaBehaviourComponent::~LavaBehaviourComponent()
{
}

void LavaBehaviourComponent::create(MyEngine* engine, GameObject* go)
{
    Component::create(engine, go);
}

void LavaBehaviourComponent::update(float dt)
{
}

void LavaBehaviourComponent::receive(Event event)
{
    GameObject * other;
    Vector2 normal;
    float depth;
    Vector2 velocity;
    RigidBodyComponent* rigid_body = nullptr;
    switch (event.getEventType())
    {
    case COLLISION:
        other = ((CollideMessage*)(event.getMessage()))->getOther();
        normal = ((CollideMessage*)(event.getMessage()))->getCollisionNormal();
        depth = ((CollideMessage*)(event.getMessage()))->getPenetrationDepth();
        other->setPosition(other->getPosition() + normal * depth);
        rigid_body = other->getComponent<RigidBodyComponent>();
        if (rigid_body != nullptr) {
            velocity.y -= GRAVITY;
            rigid_body->setVelocity(velocity);
        }
        break;
    }
}
