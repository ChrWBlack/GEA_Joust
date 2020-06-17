#include "engine.h"
#include "gameObject.h"
#include "component.h"
#include "mathHelper.h"
#include "event.h"

GameObject::GameObject(uint32_t id)
{
    id_ = id;
}


GameObject::~GameObject()
{
}

void GameObject::init()
{
    for (auto component : components_) {
        component->init();
    }

    enabled_ = true;
}

void GameObject::start() {
    for (auto component : components_) {
        component->start();
    }
}

void GameObject::update(float dt)
{
    if (!enabled_) {
        return;
    }

    for (auto component : components_) {
        component->update(dt);
    }
}

bool GameObject::isEnabled()
{
    return enabled_;
}

void GameObject::enable(bool enable)
{
    enabled_ = enable;
}

void GameObject::destroy()
{
    for (auto component : components_) {
        component->destroy();
    }
}

Vector2 GameObject::getPosition() {
    return position_;
}

void GameObject::setPosition(Vector2 position)
{
    position_ = position;
}

void GameObject::send(Event event)
{
    for (auto component : components_) {
        component->receive(event);
    }
}

void GameObject::addComponent(Component* component)
{
    components_.push_back(component);
}

uint32_t GameObject::getID() {
    return id_;
}

void GameObject::setTag(std::string tag) {
    tag_ = tag;
}

std::string GameObject::getTag() {
    return tag_;
}
