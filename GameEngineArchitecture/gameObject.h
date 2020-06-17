#pragma once
#include <vector>
#include "mathHelper.h"

class Component;
class Event;

class GameObject
{
public:
    GameObject() { };
    GameObject(uint32_t id);
    ~GameObject();

    void init();
    void start();
    void update(float dt);
    bool isEnabled();
    void enable(bool enable);
    void destroy();

    Vector2 getPosition();
    void setPosition(Vector2 position);

    void send(Event event);
    void addComponent(Component* component);

    template<typename T>
    T* getComponent() {
        for (Component * c : components_) {
            T* t = dynamic_cast<T*>(c);  //ugly but it works...
            if (t != nullptr) {
                return t;
            }
        }

        return nullptr;
    }

    uint32_t getID();
    void setTag(std::string tag);
    std::string getTag();

private:
    bool enabled_;
    uint32_t id_;
    std::string tag_;
    Vector2 position_;
    std::vector<Component *> components_;
};

