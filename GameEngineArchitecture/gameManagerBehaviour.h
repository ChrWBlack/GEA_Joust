#pragma once
#include "component.h"
#include "gameObject.h"
#include "engine.h"
#include "playerBehaviourComponent.h"

class GameManagerBehaviour : public Component
{
public:
    GameManagerBehaviour();
    ~GameManagerBehaviour();

    void create(MyEngine* engine, GameObject* go, GameObject* player, ObjectPool<GameObject>* enemies, ObjectPool<GameObject>* eggs, GameObject* pterodactyl, Sprite* life_sprite, std::vector<Sprite*>* numbers, Sprite* game_over, int spawn_enemies);
    virtual void start();
    virtual void update(float dt);
private:
    GameObject* player_;
    PlayerBehaviourComponent* player_behaviour_;
    ObjectPool<GameObject>* enemies_;
    ObjectPool<GameObject>* eggs_;
    GameObject* pterodactyl_;
    Sprite* life_sprite_;
    Sprite* game_over_;
    std::vector<Sprite*>* numbers_;
    std::vector<Vector2> enemy_spawn_positions_ {Vector2(-26, 22), Vector2(498, 22), Vector2(-26, 146), Vector2(498, 146)};

    int spawn_enemies_ = 0;
    int killed_enemies_ = 0;

    const Vector2 lives_display_position_ = Vector2(172, 327);
};

