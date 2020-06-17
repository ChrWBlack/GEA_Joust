#include "playerBehaviourComponent.h"
#include "mathHelper.h"
#include "sprite.h"

void PlayerBehaviourComponent::create(MyEngine * engine, GameObject * go, int lives)
{
    Component::create(engine, go);
    lives_ = lives;
}

void PlayerBehaviourComponent::init()
{
    rigid_body_ = go_->getComponent<RigidBodyComponent>();
    animator_ = go_->getComponent<SpriteAnimationComponent>();
    blinking_ = go_->getComponent<BlinkingComponent>();

    score_ = 0;
}

void PlayerBehaviourComponent::start()
{
    animator_->playAnimation("stand");
}

void PlayerBehaviourComponent::update(float dt)
{
    if (rigid_body_ == nullptr) {
        return;
    }

    Vector2 velocity = rigid_body_->getVelocity();
    MyEngine::KeyStatus keys = engine_->getKeyStates();

    if (animator_ != nullptr) {
        const char * current_animation = animator_->currentAnimation();
        if (abs(velocity.y) > FLT_EPSILON) {
            if ((left_facing_ && keys.right) || (!left_facing_ &&  keys.left)) {
                left_facing_ = keys.left;
            }
            if (keys.space && !last_keys_.space && current_animation != "flap") {
                animator_->playAnimation("flap", left_facing_);
            }
            else {
                animator_->playAnimation("airborne", left_facing_);
            }
        }
        else if (velocity.x < FLT_EPSILON && -(velocity.x) < FLT_EPSILON && current_animation != "stand") {
            animator_->playAnimation("stand", left_facing_);
        }
        else if ((velocity.x > 0 && keys.left || velocity.x < 0 && keys.right)) {
            if (current_animation != "break") {
                if ((left_facing_ && velocity.x > 0) || (!left_facing_ &&  velocity.x < 0)) {
                    left_facing_ = velocity.x < 0;
                }
                animator_->playAnimation("break", left_facing_);
            }
        }
        else if (abs(velocity.x) > 10.0f && current_animation != "walk") {
            if ((left_facing_ && velocity.x > 0) || (!left_facing_ &&  velocity.x < 0)) {
                left_facing_ = velocity.x < 0;
            }
            animator_->playAnimation("walk", left_facing_);
        }
    }
    last_velocity_ = velocity;

    if (abs(velocity.x) > abs(movement_level_) * 70) {
        velocity.x = movement_level_ * 70;
        rigid_body_->setVelocity(velocity);
    }

    if (keys.left && !last_keys_.left) {
        if (movement_level_ > movement_level_cap * -1) {
            --movement_level_;
        }
    }
    else if (keys.right && !last_keys_.right) {
        if (movement_level_ < movement_level_cap) {
            ++movement_level_;
        }
    }

    if (keys.space && !last_keys_.space) {
        //do the flap
        velocity = velocity + Vector2(0, -GRAVITY * 0.9f);
        if (velocity.y < -GRAVITY * 0.9f) {
            velocity.y = -GRAVITY * 0.9f;
        }
        rigid_body_->setVelocity(velocity);
    }

    rigid_body_->addForce(Vector2(movement_level_ * 70, 0));

    last_keys_ = keys;

    if (keys.right || keys.left) {
        key_hold_counter_ += dt;
        if (key_hold_counter_ >= key_hold_threshold_) {
            last_keys_.right = false;
            last_keys_.left = false;
            key_hold_counter_ -= key_hold_threshold_;
        }
    }
    else if (!keys.right && !keys.left) {
        key_hold_counter_ = 0;
    }

    if (invincible_) {
        if (invincibility_time_ <= 0) {
            invincibility_time_ = 0;
            invincible_ = false;
            if (blinking_ != nullptr) {
                blinking_->setActive(false);
            }
        }
        else {
            invincibility_time_ -= dt;
        }
    }
}

void PlayerBehaviourComponent::receive(Event event)
{
    GameObject * other;
    switch (event.getEventType()) {
    case HIT:
        other = ((HitMessage*)(event.getMessage()))->getOther();
        if (other->getTag() == "Egg") {
            score_ += 250;
        }
        else if (other->getTag() == "Enemy") {
            if (other->getPosition().y < go_->getPosition().y) {
                reduceLives();
            }
        }
        else if (other->getTag() == "Lava") {
            reduceLives();
        }
        break;
    case DEFAULT:
        break;
    }
}

int PlayerBehaviourComponent::getLives()
{
    return lives_;
}

int PlayerBehaviourComponent::getScore()
{
    return score_;
}

void PlayerBehaviourComponent::reduceLives()
{
    if (!invincible_) {
        lives_--;
        if (lives_ <= 0) {
            go_->enable(false);
        }
        else {
            setInvincible(2.0f);
        }
    }
}

void PlayerBehaviourComponent::increaseScore(int value)
{
    score_ += value;
}

void PlayerBehaviourComponent::setInvincible(float seconds)
{
    invincibility_time_ += seconds;
    invincible_ = true;
    if (blinking_ != nullptr) {
        blinking_->setActive(true);
    }
}

void EnemyBehaviourComponent::create(MyEngine * engine, GameObject * go, ObjectPool<GameObject> * eggs)
{
    Component::create(engine, go);
    this->eggs_ = eggs;
}

void EnemyBehaviourComponent::init()
{
    rigid_body_ = go_->getComponent<RigidBodyComponent>();
    animator_ = go_->getComponent<SpriteAnimationComponent>();
    direction = rand() % 1 + 0;
    direction = (direction == 0) ? -1 : 0;
    movement_level_ = 1;
}

void EnemyBehaviourComponent::start()
{
    if (animator_ != nullptr) {
        animator_->playAnimation("stand");
    }
}

void EnemyBehaviourComponent::update(float dt)
{
    if (rigid_body_ == nullptr) {
        return;
    }

    Vector2 velocity = rigid_body_->getVelocity();
    Vector2 acceleration = rigid_body_->getAcceleration();
    int random = rand() % 50 + 1;
    int velocity_direction = abs(velocity.x == 0) ? 0 : velocity.x / abs(velocity.x);
    int last_velocity_direction = abs(last_velocity_.x == 0) ? 0 : last_velocity_.x / abs(last_velocity_.x);

    if (abs(velocity.x) > abs(movement_level_) * 70) {
        velocity.x = movement_level_ * 70 * direction;
    }

    int change = 0;
    if (random <= 3) {
        change = 1;
    }
    else if (random <= 4) {
        change = -1;
    }

    if (animator_ != nullptr) {
        const char * current_animation = animator_->currentAnimation();
        if (abs(velocity.y) > FLT_EPSILON) {
            if ((left_facing_ && change == 1) || (!left_facing_ &&  change == -1)) {
                left_facing_ = change == -1;
            }
            else if (change == 0 && velocity.x != 0) {
                left_facing_ = velocity.x < 0;
            }
            if (random == 1 && current_animation != "flap") {
                animator_->playAnimation("flap", left_facing_);
            }
            else {
                animator_->playAnimation("airborne", left_facing_);
            }
        }
        else if (velocity.x < FLT_EPSILON && -(velocity.x) < FLT_EPSILON && current_animation != "stand") {
            animator_->playAnimation("stand", left_facing_);
        }
        else if (abs(velocity.x) > 10.0f && (current_animation != "walk" || velocity_direction != last_velocity_direction)) {
            if ((left_facing_ && velocity.x > 0) || (!left_facing_ &&  velocity.x < 0)) {
                left_facing_ = velocity.x < 0;
            }
            animator_->playAnimation("walk", left_facing_);
        }
    }
    last_velocity_ = velocity;

    movement_level_ += change;
    if (movement_level_ > movement_level_cap) {
        --movement_level_;
    }
    else if (movement_level_ < 0) {
        ++movement_level_;
    }

    if (random == 1) {
        //do the flap
        velocity = velocity + Vector2(0, -GRAVITY * 0.9f);
        if (velocity.y < -GRAVITY * 0.9f) {
            velocity.y = -GRAVITY * 0.9f;
        }
    }
    rigid_body_->setVelocity(velocity);
    rigid_body_->addForce(Vector2(movement_level_ * 70 * direction, 0));
}

void EnemyBehaviourComponent::receive(Event event)
{
    GameObject* other = nullptr;
    switch (event.getEventType()) {
    case HIT:
        other = ((HitMessage*)(event.getMessage()))->getOther();
        if (other->getTag() == "Player") {
            direction *= -1;
            if (other->getPosition().y < go_->getPosition().y) {
                die();
            }
        }
        else if (other->getTag() == "Lava") {
            die();
        }
        break;
    case DEFAULT:
        break;
    }
}

void EnemyBehaviourComponent::die()
{
    GameObject* egg = nullptr;
    egg = eggs_->FirstAvailable();
    egg->init();
    egg->setPosition(go_->getPosition());
    egg->getComponent<RigidBodyComponent>()->setVelocity(rigid_body_->getVelocity());
    go_->enable(false);
}

void ScreenBoundsComponent::create(MyEngine * engine, GameObject * go, int screen_width)
{
    Component::create(engine, go);
    this->screen_width_ = screen_width;
}

void ScreenBoundsComponent::init()
{
    int width, height = 0;
    if (go_->getComponent<RenderComponent>()->getSprite()->getSpriteSize(&width, &height)) {
        sprite_width_ = width;
    }
    rigid_body_ = go_->getComponent<RigidBodyComponent>();
}

void ScreenBoundsComponent::update(float dt)
{
    if (go_->getPosition().x < -sprite_width_) {
        go_->setPosition(go_->getPosition() + Vector2(screen_width_ + sprite_width_, 0));
    }
    else if (go_->getPosition().x > screen_width_) {
        go_->setPosition(go_->getPosition() + Vector2(-(screen_width_ + sprite_width_), 0));
    }
    if (go_->getPosition().y < 0) {
        go_->setPosition(Vector2(go_->getPosition().x, 0));
        if (rigid_body_ != nullptr) {
            rigid_body_->setVelocity(Vector2(rigid_body_->getVelocity().x, -(rigid_body_->getVelocity().y)));
        }
    }
}

void EggBehaviourComponent::create(MyEngine * engine, GameObject * go, ObjectPool<GameObject> * knights)
{
    Component::create(engine, go);
    this->knights_ = knights;
}

void EggBehaviourComponent::init()
{
    rigid_body_ = go_->getComponent<RigidBodyComponent>();
}

void EggBehaviourComponent::update(float dt)
{
    if (rigid_body_ == nullptr) {
        return;
    }
    Vector2 velocity = rigid_body_->getVelocity();
    velocity.x *= pow(0.71, dt);
    if (velocity.x < 0.8) {
        velocity.x = 0;
    }
    rigid_body_->setVelocity(velocity);
    if (time_counter_ <= 0) {
        time_counter_ = respawn_time_;
        go_->enable(false);
        GameObject* knight = knights_->FirstAvailable();
        knight->setPosition(go_->getPosition());
        knight->enable(true);
    }
    else
    {
        time_counter_ -= dt;
    }
}

void EggBehaviourComponent::receive(Event event)
{
    switch (event.getEventType()) {
    case HIT:
        if (((HitMessage*)(event.getMessage()))->getOther()->getTag() == "Player" || ((HitMessage*)(event.getMessage()))->getOther()->getTag() == "Lava") {
            time_counter_ = respawn_time_;
            go_->enable(false);
        }
        break;
    case DEFAULT:
        break;
    }
}

void PterodactylBehaviourComponent::create(MyEngine * engine, GameObject * go, GameObject * player)
{
    Component::create(engine, go);
    this->player_ = player;
    movement_speed_ = 60;
}

void PterodactylBehaviourComponent::init()
{
    rigid_body_ = go_->getComponent<RigidBodyComponent>();
    animator_ = go_->getComponent<SpriteAnimationComponent>();
    mspf_animator_ = animator_->getMSPF();
}

void PterodactylBehaviourComponent::start()
{
    if (animator_ != nullptr) {
        animator_->playAnimation("move");
        ms_counter_ = 0;
    }
}

void PterodactylBehaviourComponent::update(float dt)
{
    if (rigid_body_ == nullptr) {
        return;
    }
    if (animator_->currentAnimation() == "die") {
        rigid_body_->setVelocity(Vector2());
        return;
    }

    ms_counter_ += (dt * 1000);
    if (ms_counter_ >= mspf_animator_ * 4) {
        ms_counter_ -= mspf_animator_ * 4;
    }

    if (stunned_) {
        if (stun_timer_ <= 0) {
            stunned_ = false;
            stun_timer_ = 0;
        }
        else {
            stun_timer_ -= dt;
        }
    }
    else {
        if (player_ != nullptr && player_->isEnabled()) {
            Vector2 direction = player_->getPosition() - go_->getPosition();
            rigid_body_->setVelocity(direction.normalized() * movement_speed_);
        }
        else {
            rigid_body_->setVelocity(Vector2(last_velocity_.x / abs(last_velocity_.x), 0) * movement_speed_);
        }
        if (rigid_body_->getVelocity().magnitude() > movement_speed_) {
            rigid_body_->setVelocity(rigid_body_->getVelocity().normalized() * movement_speed_);
        }
    }

    if (rigid_body_->getVelocity().x < 0 && last_velocity_.x >= 0) {
        animator_->playAnimation("move", true);
        ms_counter_ = 0;
    }
    else if (rigid_body_->getVelocity().x >= 0 && last_velocity_.x < 0) {
        animator_->playAnimation("move");
        ms_counter_ = 0;
    }

    last_velocity_ = rigid_body_->getVelocity();
}

void PterodactylBehaviourComponent::receive(Event event)
{
    GameObject * other = nullptr;
    Vector2 position;
    PlayerBehaviourComponent * player = nullptr;
    switch (event.getEventType()) {
    case HIT:
        other = ((HitMessage*)(event.getMessage()))->getOther();
        position = go_->getPosition();
        if (other->getTag() == "Player" && animator_->currentAnimation() != "die") {
            player = other->getComponent<PlayerBehaviourComponent>();

            if (other->getPosition().y <= position.y && other->getPosition().y >= position.y - 4 && ms_counter_ >= mspf_animator_ * 3) {
                animator_->playAnimation("die", rigid_body_->getVelocity().x < 0);
                if (player != nullptr) {
                    player->increaseScore(1000);
                }
            }
            else {
                if (player != nullptr) {
                    player->reduceLives();

                    stunned_ = true;
                    stun_timer_ = stunned_time_;
                    rigid_body_->addForce(position - other->getPosition());
                }
            }
        }
        break;
    case ANIMATION_COMPLETE:
        if (((AnimationCompleteMessage*)(event.getMessage()))->getName() == "die") {
            go_->enable(false);
        }
    case DEFAULT:
        break;
    }
}

void BlinkingComponent::create(MyEngine * engine, GameObject * go, Sprite * sprite)
{
    Component::create(engine, go);
    this->sprite_ = sprite;
}

void BlinkingComponent::update(float dt)
{
    if (active_ && sprite_ != nullptr) {
        if (counter_ >= interval_) {
            sprite_->hide(!(sprite_->isHidden()));
            counter_ -= interval_;
        }
        counter_ += dt;
    }
}

void BlinkingComponent::setActive(bool active)
{
    this->active_ = active;
    if (active) {
        counter_ = 0;
    }
    sprite_->hide(active);
}
