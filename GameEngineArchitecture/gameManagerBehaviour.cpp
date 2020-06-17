#include "gameManagerBehaviour.h"
#include "sprite.h"

GameManagerBehaviour::GameManagerBehaviour()
{
}

GameManagerBehaviour::~GameManagerBehaviour()
{
}

void GameManagerBehaviour::create(MyEngine* engine, GameObject* go, GameObject * player, ObjectPool<GameObject>* enemies, ObjectPool<GameObject>* eggs, GameObject* pterodactyl, Sprite* life_sprite, std::vector<Sprite*>* numbers, Sprite* game_over, int spawn_enemies)
{
    Component::create(engine, go);
    this->player_ = player;
    this->player_behaviour_ = player->getComponent<PlayerBehaviourComponent>();
    this->enemies_ = enemies;
    this->eggs_ = eggs;
    this->pterodactyl_ = pterodactyl;
    this->life_sprite_ = life_sprite;
    this->numbers_ = numbers;
    this->game_over_ = game_over;
    this->spawn_enemies_ = spawn_enemies;
}

void GameManagerBehaviour::start()
{
    for (auto i = enemies_->pool.begin(); i != enemies_->pool.end(); i++) {
        if ((*i)->isEnabled()) {
            (*i)->enable(false);
        }
    }
    for (int i = 0; i < spawn_enemies_; i++) {
        GameObject* enemy = enemies_->FirstAvailable();
        enemy->setPosition(enemy_spawn_positions_.at(rand() % (enemy_spawn_positions_.size() - 1) + 0));
        enemy->enable(true);
    }

    player_->setPosition(Vector2(196, 278));
    pterodactyl_->setPosition(enemy_spawn_positions_.at(rand() % (enemy_spawn_positions_.size() - 1) + 0));
}

void GameManagerBehaviour::update(float dt)
{
    int lives = player_behaviour_->getLives();
    if (lives <= 0) {
        game_over_->draw(133, 125);
    }
    else {
        for (int i = 0; i < lives; i++) {
            life_sprite_->draw(lives_display_position_.x + i * 5, lives_display_position_.y);
        }
    }

    int score = player_behaviour_->getScore();
    std::vector<int> digits_reverse;
    if (score > 0) {
        for (score; score > 0; score /= 10) {
            digits_reverse.push_back(score % 10);
        }
        for (int i = 0; i < digits_reverse.size(); i++) {
            numbers_->at(digits_reverse.at(i))->draw(lives_display_position_.x - 14 - i * 11, lives_display_position_.y);
        }
    }
    else {
        numbers_->at(0)->draw(lives_display_position_.x - 14, lives_display_position_.y);
    }

    bool all_done = true;
    for (auto i = enemies_->pool.begin(); i != enemies_->pool.end(); ++i) {
        if ((*i)->isEnabled()) {
            all_done = false;
            break;
        }
    }
    if (all_done) {
        for (auto i = eggs_->pool.begin(); i != eggs_->pool.end(); ++i) {
            if ((*i)->isEnabled()) {
                all_done = false;
                break;
            }
        }
        all_done = all_done && !(pterodactyl_->isEnabled());
    } 
    if (all_done) {
        spawn_enemies_++;
        for (int i = 0; i < spawn_enemies_ && i < enemies_->pool.size(); ++i) {
            GameObject* enemy = enemies_->FirstAvailable();
            enemy->setPosition(enemy_spawn_positions_.at(rand() % (enemy_spawn_positions_.size() - 1) + 0));
            enemy->enable(true);
        }
        if (spawn_enemies_ % 3 == 1) {
            pterodactyl_->setPosition(enemy_spawn_positions_.at(rand() % (enemy_spawn_positions_.size() - 1) + 0));
            pterodactyl_->getComponent<SpriteAnimationComponent>()->playAnimation("move");
            pterodactyl_->enable(true);
        }
    }

}
