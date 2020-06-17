#define _CRT_SECURE_NO_WARNINGS
#include "engine.h"
#include "playerBehaviourComponent.h"
#include "levelBehaviourComponent.h"
#include "gameManagerBehaviour.h"
#include <time.h>  
#undef main

int main(int argc, char** argv)
{
    static int MS_PER_FRAME = 16;
    const static int SCREEN_WIDTH = 472;
    const static int SCREEN_HEIGHT = 388;
    const static int PLAYER_LIVES = 3;
    const static int START_ENEMY_KNIGHTS = 4;
    const static int MAX_ENEMY_KNIGHTS = 15;

    MyEngine engine;

    GameObject* gameManager;

    GameObject* arena;
    GameObject* lava;

    GameObject* player;

    GameObject * pterodactyl;

    ObjectPool<GameObject> enemies;
    ObjectPool<GameObject> eggs;

    ObjectPool<GameObject> * world_collisions = new ObjectPool<GameObject>();
    ObjectPool<GameObject> * player_collisions = new ObjectPool<GameObject>();


    srand(time(NULL));

    engine.initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

    engine.setMSPerFrame(MS_PER_FRAME);
    engine.setBackgroundColor(0, 0, 0, 255);

    arena = engine.createObject();
    RenderComponent * arena_render = new RenderComponent();
    arena_render->create(&engine, arena, "Assets/Joust_Spritesheet_Alpha.png", true, 2, 44, 236, 194);
    CollideComponent * arena_collider_01 = new CollideComponent();
    arena_collider_01->create(&engine, arena, world_collisions, Vector2(80, 310), Vector2(376, 342));
    CollideComponent * arena_collider_02 = new CollideComponent();
    arena_collider_02->create(&engine, arena, world_collisions, Vector2(-30, 54), Vector2(46, 69));
    CollideComponent * arena_collider_03 = new CollideComponent();
    arena_collider_03->create(&engine, arena, world_collisions, Vector2(396, 54), Vector2(550, 69));
    CollideComponent * arena_collider_04 = new CollideComponent();
    arena_collider_04->create(&engine, arena, world_collisions, Vector2(130, 76), Vector2(268, 96));
    CollideComponent * arena_collider_05 = new CollideComponent();
    arena_collider_05->create(&engine, arena, world_collisions, Vector2(316, 162), Vector2(406, 186));
    CollideComponent * arena_collider_06 = new CollideComponent();
    arena_collider_06->create(&engine, arena, world_collisions, Vector2(-30, 178), Vector2(96, 196));
    CollideComponent * arena_collider_07 = new CollideComponent();
    arena_collider_07->create(&engine, arena, world_collisions, Vector2(400, 178), Vector2(550, 196));
    CollideComponent * arena_collider_08 = new CollideComponent();
    arena_collider_08->create(&engine, arena, world_collisions, Vector2(162, 224), Vector2(262, 242));
    LevelBehaviourComponent * level_behaviour = new LevelBehaviourComponent();
    level_behaviour->create(&engine, arena);
    arena->addComponent(arena_render);
    arena->addComponent(arena_collider_01);
    arena->addComponent(arena_collider_02);
    arena->addComponent(arena_collider_03);
    arena->addComponent(arena_collider_04);
    arena->addComponent(arena_collider_05);
    arena->addComponent(arena_collider_06);
    arena->addComponent(arena_collider_07);
    arena->addComponent(arena_collider_08);
    arena->addComponent(level_behaviour);
    arena->setTag("Arena");
    arena->init();

    lava = engine.createObject();
    LavaBehaviourComponent * lava_behaviour = new LavaBehaviourComponent();
    lava_behaviour->create(&engine, lava);
    RenderComponent * lava_render = new RenderComponent();
    lava_render->create(&engine, lava, "Assets/Joust_Spritesheet_Alpha.png", true, 2, 243, 236, 26);
    CollideComponent * lava_collider = new CollideComponent();
    lava_collider->create(&engine, lava, world_collisions, Vector2(0, 6), Vector2(475, 52));
    lava->addComponent(lava_behaviour);
    lava->addComponent(lava_render);
    lava->addComponent(lava_collider);
    lava->setTag("Lava");
    lava->init();
    lava->setPosition(Vector2(0, SCREEN_HEIGHT - 52));

    player = engine.createObject();
    PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
    player_behaviour->create(&engine, player, PLAYER_LIVES);
    RenderComponent * player_render = new RenderComponent();
    player_render->create(&engine, player, "Assets/Joust_Spritesheet_Alpha.png", true, 248, 44, 13, 18);
    SpriteAnimationComponent * player_animator = new SpriteAnimationComponent();
    player_animator->create(&engine, player, player_render->getSprite(), 80);
    player_animator->addAnimation("stand", 1, 248, 44, 13, 18, 0, true);
    player_animator->addAnimation("walk", 4, 248, 64, 13, 18, 2, true);
    player_animator->addAnimation("break", 1, 248, 82, 13, 18, 0, true);
    player_animator->addAnimation("airborne", 1, 263, 102, 13, 12, 0, true);
    player_animator->addAnimation("flap", 1, 248, 102, 13, 12, 0, true);
    RigidBodyComponent * player_rigid = new RigidBodyComponent();
    player_rigid->create(&engine, player);
    CollideComponent * player_collider = new CollideComponent();
    player_collider->create(&engine, player, player_collisions, Vector2(), Vector2(26, 32));
    ScreenBoundsComponent * player_bounds = new ScreenBoundsComponent();
    player_bounds->create(&engine, player, SCREEN_WIDTH);
    BlinkingComponent * blinking = new BlinkingComponent();
    blinking->create(&engine, player, player_render->getSprite());
    player->addComponent(player_behaviour);
    player->addComponent(player_render);
    player->addComponent(player_animator);
    player->addComponent(player_rigid);
    player->addComponent(player_collider);
    player->addComponent(player_bounds);
    player->addComponent(blinking);
    player->setTag("Player");
    player->init();
    world_collisions->pool.push_back(player);

    enemies.Create(MAX_ENEMY_KNIGHTS);
    for (int i = 0; i < enemies.pool.size(); ++i) {
        enemies.pool.at(i) = engine.createObject();
        EnemyBehaviourComponent * enemy_behaviour = new EnemyBehaviourComponent();
        enemy_behaviour->create(&engine, enemies.pool.at(i), &eggs);
        RenderComponent * enemy_render = new RenderComponent();
        enemy_render->create(&engine, enemies.pool.at(i), "Assets/Joust_Spritesheet_Alpha.png", true, 248, 194, 13, 18);
        SpriteAnimationComponent * enemy_animator = new SpriteAnimationComponent();
        enemy_animator->create(&engine, enemies.pool.at(i), enemy_render->getSprite(), 80);
        enemy_animator->addAnimation("stand", 1, 248, 194, 13, 18, 0, true);
        enemy_animator->addAnimation("walk", 4, 248, 194, 13, 18, 2, true);
        enemy_animator->addAnimation("airborne", 1, 263, 214, 13, 12, 0, true);
        enemy_animator->addAnimation("flap", 1, 248, 214, 13, 12, 0, true);
        RigidBodyComponent * enemy_rigid = new RigidBodyComponent();
        enemy_rigid->create(&engine, enemies.pool.at(i));
        CollideComponent * enemy_collider = new CollideComponent();
        enemy_collider->create(&engine, enemies.pool.at(i), new ObjectPool<GameObject>(), Vector2(), Vector2(26, 32));
        ScreenBoundsComponent * enemy_bounds = new ScreenBoundsComponent();
        enemy_bounds->create(&engine, enemies.pool.at(i), SCREEN_WIDTH);
        enemies.pool.at(i)->addComponent(enemy_behaviour);
        enemies.pool.at(i)->addComponent(enemy_render);
        enemies.pool.at(i)->addComponent(enemy_animator);
        enemies.pool.at(i)->addComponent(enemy_rigid);
        enemies.pool.at(i)->addComponent(enemy_collider);
        enemies.pool.at(i)->addComponent(enemy_bounds);
        enemies.pool.at(i)->setTag("Enemy");
        enemies.pool.at(i)->init();
        world_collisions->pool.push_back(enemies.pool.at(i));
        player_collisions->pool.push_back(enemies.pool.at(i));
    }

    eggs.Create(MAX_ENEMY_KNIGHTS);
    for (int i = 0; i < eggs.pool.size(); ++i) {
        eggs.pool.at(i) = engine.createObject();
        EggBehaviourComponent * egg_behaviour = new EggBehaviourComponent();
        egg_behaviour->create(&engine, eggs.pool.at(i), &enemies);
        RenderComponent * egg_render = new RenderComponent();
        egg_render->create(&engine, eggs.pool.at(i), "Assets/Joust_Spritesheet_Alpha.png", true, 248, 278, 5, 6);
        RigidBodyComponent * egg_rigid = new RigidBodyComponent();
        egg_rigid->create(&engine, eggs.pool.at(i));
        CollideComponent * egg_collider = new CollideComponent();
        egg_collider->create(&engine, eggs.pool.at(i), new ObjectPool<GameObject>(), Vector2(), Vector2(10, 12));
        ScreenBoundsComponent * egg_bounds = new ScreenBoundsComponent();
        egg_bounds->create(&engine, eggs.pool.at(i), SCREEN_WIDTH);
        eggs.pool.at(i)->addComponent(egg_behaviour);
        eggs.pool.at(i)->addComponent(egg_render);
        eggs.pool.at(i)->addComponent(egg_rigid);
        eggs.pool.at(i)->addComponent(egg_collider);
        eggs.pool.at(i)->addComponent(egg_bounds);
        eggs.pool.at(i)->setTag("Egg");
        world_collisions->pool.push_back(eggs.pool.at(i));
        player_collisions->pool.push_back(eggs.pool.at(i));
    }

    pterodactyl = engine.createObject();
    PterodactylBehaviourComponent * pterodactyl_behaviour = new PterodactylBehaviourComponent();
    pterodactyl_behaviour->create(&engine, pterodactyl, player);
    RenderComponent * pterodactyl_render = new RenderComponent();
    pterodactyl_render->create(&engine, pterodactyl, "Assets/Joust_Spritesheet_Alpha.png", true, 248, 330, 23, 12);
    SpriteAnimationComponent * pterodactyl_animator = new SpriteAnimationComponent();
    pterodactyl_animator->create(&engine, pterodactyl, pterodactyl_render->getSprite(), 160);
    SDL_Rect* frames = new SDL_Rect[4];
    frames[0] = { 248, 330, 20, 12 };
    frames[1] = { 270, 330, 19, 12 };
    frames[2] = { 291, 330, 20, 12 };
    frames[3] = { 313, 330, 23, 12 };
    pterodactyl_animator->addAnimation("move", 4, frames, true);
    pterodactyl_animator->addAnimation("die", 3, 248, 345, 21, 12, 2, false);
    RigidBodyComponent * pterodactyl_rigid = new RigidBodyComponent();
    pterodactyl_rigid->create(&engine, pterodactyl);
    CollideComponent * pterodactyl_collider = new CollideComponent();
    pterodactyl_collider->create(&engine, pterodactyl, new ObjectPool<GameObject>(), Vector2(), Vector2(26, 24));
    ScreenBoundsComponent * pterodactyl_bounds = new ScreenBoundsComponent();
    pterodactyl_bounds->create(&engine, pterodactyl, SCREEN_WIDTH);
    pterodactyl->addComponent(pterodactyl_behaviour);
    pterodactyl->addComponent(pterodactyl_render);
    pterodactyl->addComponent(pterodactyl_animator);
    pterodactyl->addComponent(pterodactyl_rigid);
    pterodactyl->addComponent(pterodactyl_collider);
    pterodactyl->addComponent(pterodactyl_bounds);
    pterodactyl->setTag("Pterodactyl");
    pterodactyl->init();
    world_collisions->pool.push_back(pterodactyl);
    player_collisions->pool.push_back(pterodactyl);

    std::vector<Sprite*> number_sprites;
    for (int i = 0; i < 10; i++) {
        number_sprites.push_back(engine.createSprite("Assets/Joust_Spritesheet_Alpha.png", true, 2 + i * 6, 299, 5, 7));
    }
    gameManager = engine.createObject();
    GameManagerBehaviour * manager_behaviour = new GameManagerBehaviour();
    manager_behaviour->create(&engine, gameManager, player, &enemies, &eggs, pterodactyl, engine.createSprite("Assets/Joust_Spritesheet_Alpha.png", true, 62, 299, 2, 7), &number_sprites, engine.createSprite("Assets/Joust_Spritesheet_Alpha.png", true, 2, 378, 95, 7), START_ENEMY_KNIGHTS);
    gameManager->addComponent(manager_behaviour);
    gameManager->init();

    engine.gameLoop();

    return 0;
}