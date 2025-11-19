#ifndef ENEMIES_H
#define ENEMIES_H

#include "stdafx.h"
#include "Animation.h"
#include "Collider.h"


//Parent Class 
class Enemy
{
public:
    Enemy(std::map<std::string, sf::Texture*> &tex, float switchTim, float speed, sf::Vector2f pos);
    virtual ~Enemy() = default;


    void update(float deltaTime);
    void onCollison(sf::Vector2f direction);
    void render(sf::RenderTarget *target) const;
    Collider GetCollider();
    sf::FloatRect getBounds() const;
    sf::FloatRect getAttackBounds() const;
    bool isDead() const;
    bool isAttacking() const;
    void die();

protected:
    std::map<std::string, sf::Texture*> &textures;
    Animation animation;
    bool faceRight;
    sf::Vector2f velocity;
    float speed;

    AnimMovingState AI_State;
    float maxMovingTime;    // time of one animation cycle(not movment cycle) = 8 * 0.15s = 1.2s
    float totalMovingTime;  // current time in the cycle
    float deathDuartion;
    float deathElapsedTime;
    bool isAlive;
    bool isDying;
    bool isFlying; // new


    sf::RectangleShape attackHitbox;
    sf::Vector2f attackHitboxOffset; // new
    float attackTimer;
    float attackTimerStart; // new
    float attackTimerStop;  // new
    bool attacking;

    sf::Sprite sprite;
    sf::RectangleShape hitbox;
    sf::Vector2f hitboxOffset; // hitbox offset form the sprite
    sf::Vector2f hitboxSize;
};


// Child classes - Enemies
// 1===================================================================================================================
class EvilWizard : public Enemy
{
public:
    EvilWizard(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);
};


// 2===================================================================================================================
class Skeleton : public Enemy
{
public:
    Skeleton(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);
};


// 3===================================================================================================================
class Mushroom : public Enemy
{
public:
    Mushroom(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);
};


// 4===================================================================================================================
class Goblin : public Enemy
{
public:
    Goblin(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);
};


// 5===================================================================================================================
class FlyingEye : public Enemy
{
public:
    FlyingEye(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);
};


#endif // ENTITIES_H
