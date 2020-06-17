#include "engine.h"
#include "gameObject.h"
#include "objectPool.h"
#include "sprite.h"
#include "component.h"
#include <algorithm>

Component::~Component()
{
}

void Component::create(MyEngine * engine, GameObject * go)
{
    this->go_ = go;
    this->engine_ = engine;
}

void RenderComponent::create(MyEngine * engine, GameObject * go, const char * sprite_name, bool clipped, int posX, int posY, int width, int height)
{
    Component::create(engine, go);

    this->sprite_ = engine->createSprite(sprite_name, clipped, posX, posY, width, height);
}

void RenderComponent::update(float dt)
{
    if (!go_->isEnabled())
        return;

    if (sprite_)
        sprite_->draw(int(go_->getPosition().x), int(go_->getPosition().y));
}

void RenderComponent::destroy()
{
    if (sprite_ != NULL)
        sprite_->destroy();
    sprite_ = NULL;
}


void CollideComponent::create(MyEngine* engine, GameObject * go, ObjectPool<GameObject> * coll_objects, Vector2 min, Vector2 max)
{
    Component::create(engine, go);
    this->coll_objects_ = coll_objects;
    this->relative_bounding_box_ = AABB(min, max);
}


void CollideComponent::update(float dt)
{
    for (auto i = 0; i < coll_objects_->pool.size(); i++)
    {
        GameObject * go0 = coll_objects_->pool[i];
        CollideComponent * collider0 = go0->getComponent<CollideComponent>();
        if (go0->isEnabled() && collider0 != nullptr)
        {
            Vector2 normal = Vector2();
            float depth = 0.0f;
            if (AABBvsAABB(getBoundingBox(), collider0->getBoundingBox(), &normal, &depth))
            {
                CollideMessage* col_message = new CollideMessage();
                col_message->create(go0, normal, depth);
                HitMessage* message = new HitMessage();
                message->create(go0);
                HitMessage* message0 = new HitMessage();
                message0->create(go_);
                go_->send(col_message->generateEvent());
                go_->send(message->generateEvent());
                go0->send(message0->generateEvent());
            }
        }
    }
}

AABB CollideComponent::getBoundingBox()
{
    Vector2 position = go_->getPosition();
    return AABB(position + relative_bounding_box_.min, position + relative_bounding_box_.max);
}

bool CollideComponent::AABBvsAABB(AABB a, AABB b, Vector2 * normal, float * depth)
{
    if (a.max.x < b.min.x or a.min.x > b.max.x) {
        return false;
    }
    if (a.max.y < b.min.y or a.min.y > b.max.y) {
        return false;
    }

    static const Vector2 faces[4] = {
        Vector2(-1,  0), // 'left' face normal (-x direction)
        Vector2(1,  0), // 'right' face normal (+x direction)
        Vector2(0,  1), // 'bottom' face normal (+y direction)
        Vector2(0, -1) // 'top' face normal (-y direction)
    };

    float distances[4] =
    {
        (b.max.x - a.min.x), // distance of box 'b' to face on 'left' side of 'a'.
        (a.max.x - b.min.x), // distance of box 'b' to face on 'right' side of 'a'.
        (a.max.y - b.min.y), // distance of box 'b' to face on 'bottom' side of 'a'.
        (b.max.y - a.min.y) // distance of box 'b' to face on 'top' side of 'a'.
    };

    for (int i = 0; i < 4; i++)
    {
        // face of least intersection depth
        if ((i == 0) || (distances[i] < *depth))
        {
            *normal = faces[i];
            *depth = distances[i];
        }
    }
    return true;
}

void RigidBodyComponent::create(MyEngine* engine, GameObject * go)
{
    Component::create(engine, go);
}

void RigidBodyComponent::receive(Event event)
{
    switch (event.getEventType()) {
    case COLLISION:
        RigidBodyComponent * other = ((CollideMessage*)(event.getMessage()))->getOther()->getComponent<RigidBodyComponent>();
        if (other != nullptr) {
            this->resolveCollision(other);
        }
        break;
    }
}

void RigidBodyComponent::update(float dt)
{
    acceleration_ = acceleration_ + force_ * dt;

    if (abs(acceleration_.y) > abs(force_.y)) {
        acceleration_.y = force_.y;
    }
    if (abs(acceleration_.x) > abs(force_.x)) {
        acceleration_.x = force_.x;
    }
    last_position_ = go_->getPosition();
    go_->setPosition(last_position_ + (velocity_ * dt));
    velocity_ = velocity_ + acceleration_ * dt;

    force_ = Vector2(0, GRAVITY);
}

Vector2 RigidBodyComponent::getAcceleration()
{
    return acceleration_;
}

void RigidBodyComponent::setAcceleration(Vector2 acceleration)
{
    acceleration_ = acceleration;
}

Vector2 RigidBodyComponent::getVelocity()
{
    return velocity_;
}

void RigidBodyComponent::setVelocity(Vector2 velocity)
{
    velocity_ = velocity;
}

void RigidBodyComponent::addForce(Vector2 force)
{
    force_ = force_ + force;
}

void RigidBodyComponent::resolveCollision(RigidBodyComponent* other)
{
    Vector2 goTogo0 = other->go_->getPosition() - go_->getPosition(); 
    double distanceBetweenObjects = goTogo0.magnitude();

    Vector2 goTogo0Normalized = goTogo0 / distanceBetweenObjects;

    double dotProduct = velocity_.dotProduct(goTogo0Normalized);
    double dotProduct0 = other->getVelocity().dotProduct(goTogo0Normalized);

    velocity_ = goTogo0Normalized * dotProduct0;
    other->setVelocity(other->getVelocity() - goTogo0Normalized * dotProduct);

    go_->setPosition(last_position_);
    other->go_->setPosition(other->last_position_);
}

void SpriteAnimationComponent::create(MyEngine * engine, GameObject * go, Sprite * sprite, int mspf)
{
    Component::create(engine, go);
    this->sprite = sprite;
    this->animations = std::unordered_map<const char*, Animation>();
    this->current = nullptr;
    this->iterator = 0;
    this->running = false;
    this->mspf_ = mspf;
}

void SpriteAnimationComponent::update(float dt)
{
    if (!running) {
        return;
    }
    sprite->setClipRect(current->frames[iterator].x, current->frames[iterator].y, current->frames[iterator].w, current->frames[iterator].h);
    if (ms_counter_ >= mspf_) {
        iterator++;
        ms_counter_ -= mspf_;
        if (iterator >= current->frame_count) {
            if (current->looping) {
                iterator = 0;
            }
            else {
                AnimationCompleteMessage message = AnimationCompleteMessage();
                message.create(current_name);
                go_->send(message.generateEvent());
                running = false;
                ms_counter_ = 0;
            }
        }
    }
    ms_counter_ += (dt * 1000);
}

void SpriteAnimationComponent::addAnimation(const char * name, int frame_count_, SDL_Rect * frames_, bool looping_)
{
    animations.insert({ name, Animation(frame_count_, frames_, looping_) });
}

void SpriteAnimationComponent::addAnimation(const char * name, int frame_count_, int pos_x, int pos_y, int width, int height, int offset, bool looping_)
{
    SDL_Rect* frames = new SDL_Rect[frame_count_];
    for (int i = 0; i < frame_count_; ++i) {
        frames[i] = SDL_Rect();
        frames[i].x = pos_x + i * (width + offset);
        frames[i].y = pos_y;
        frames[i].w = width;
        frames[i].h = height;
    }
    animations.insert({ name, Animation(frame_count_, frames, looping_) });
}

bool SpriteAnimationComponent::playAnimation(const char * name, bool flip)
{
    try {
        current = &(animations.at(name));
        iterator = 0;
        current_name = name;
        running = true;
        if (flip) {
            sprite->flipSprite(SDL_FLIP_HORIZONTAL);
        }
        else {
            sprite->flipSprite(SDL_FLIP_NONE);
        }
        return true;
    }
    catch (const std::exception& e) {
        SDL_Log(e.what());
        current = nullptr;
        return false;
    }
}

bool SpriteAnimationComponent::isRunning()
{
    return running;
}

const char * SpriteAnimationComponent::currentAnimation()
{
    if (running) {
        return current_name;

    }
    else {
        return "";
    }
}

int SpriteAnimationComponent::getMSPF()
{
    return mspf_;
}
