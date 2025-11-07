#ifndef ENTITIES_H
#define ENTITIES_H

#include "stdafx.h"
#include "Animation.h"
#include "Collider.h"


//parent class
class Entity
{
};


    //Enemies
class Enemy : Entity
{
public:
    virtual void render(sf::RenderTarget *target)=0;
    virtual void update(float deltaTime)=0;
    virtual void onCollison(sf::Vector2f direction)=0;
    virtual void die()=0;

    virtual Collider GetCollider()=0;
    virtual sf::FloatRect getBounds()=0;
    virtual sf::FloatRect getAttackBounds()=0;
    virtual bool isAttacking()=0;
    virtual bool isDead()=0;
};


    //1===================================================================================================================
class EvilWizard : public Enemy
{
public:
    EvilWizard(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);

    void render(sf::RenderTarget *target) override;
    void update(float deltaTime) override;
    void onCollison(sf::Vector2f direction) override;
    void die() override;

    //Accesors
    Collider GetCollider() override;
    sf::FloatRect getBounds() override;
    sf::FloatRect getAttackBounds() override;
    bool isAttacking() override;
    bool isDead() override;
private:
    std::map<std::string, sf::Texture*> &textures;
    Animation animation;
    bool faceRight;
    sf::Vector2f velocity;
    float speed;

    AnimMovingState AI_State;
    float maxMovingTime;    // czas po ktorym przeciwnik ma zaatakowac, a nastpenie isc w druga strone i powtorzyc cykl
    float totalMovingTime;  // moment tego cyklu
    float maxDeathTimer;
    float totalDeathTimer;
    bool isAlive;
    bool isDying;

    sf::RectangleShape attackHitbox;
    float attackTimer;
    bool attacking;

    sf::Sprite sprite;
    sf::RectangleShape hitbox;
    sf::Vector2f hitboxOffset; // offsety hitboxu od sprite'a
    sf::Vector2f hitboxSize;
};


    //2===================================================================================================================
class Skeleton : public Enemy
{
public:
    Skeleton(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);

    void render(sf::RenderTarget *target) override;
    void update(float deltaTime) override;
    void onCollison(sf::Vector2f direction) override;
    void die() override;

    //Accesors
    Collider GetCollider() override;
    sf::FloatRect getBounds() override;
    sf::FloatRect getAttackBounds() override;
    bool isAttacking() override;
    bool isDead() override;
private:
    std::map<std::string, sf::Texture*> &textures;
    Animation animation;
    bool faceRight;
    sf::Vector2f velocity;
    float speed;

    AnimMovingState AI_State;
    float maxMovingTime;    // czas po ktorym przeciwnik ma zaatakowac, a nastpenie isc w druga strone i powtorzyc cykl
    float totalMovingTime;  // moment tego cyklu
    float maxDeathTimer;
    float totalDeathTimer;
    bool isAlive;
    bool isDying;

    sf::RectangleShape attackHitbox;
    float attackTimer;
    bool attacking;

    sf::Sprite sprite;
    sf::RectangleShape hitbox;
    sf::Vector2f hitboxOffset; // offsety hitboxu od sprite'a
    sf::Vector2f hitboxSize;
};


    //3===================================================================================================================
class Mushroom : public Enemy
{
public:
    Mushroom(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);

    void render(sf::RenderTarget *target) override;
    void update(float deltaTime) override;
    void onCollison(sf::Vector2f direction) override;
    void die() override;

    //Accesors
    Collider GetCollider() override;
    sf::FloatRect getBounds() override;
    sf::FloatRect getAttackBounds() override;
    bool isAttacking() override;
    bool isDead() override;
private:
    std::map<std::string, sf::Texture*> &textures;
    Animation animation;
    bool faceRight;
    sf::Vector2f velocity;
    float speed;

    AnimMovingState AI_State;
    float maxMovingTime;    // czas po ktorym przeciwnik ma zaatakowac, a nastpenie isc w druga strone i powtorzyc cykl
    float totalMovingTime;  // moment tego cyklu
    float maxDeathTimer;
    float totalDeathTimer;
    bool isAlive;
    bool isDying;

    sf::RectangleShape attackHitbox;
    float attackTimer;
    bool attacking;

    sf::Sprite sprite;
    sf::RectangleShape hitbox;
    sf::Vector2f hitboxOffset; // offsety hitboxu od sprite'a
    sf::Vector2f hitboxSize;
};


    //4===================================================================================================================
class Goblin : public Enemy
{
public:
    Goblin(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);

    void render(sf::RenderTarget *target) override;
    void update(float deltaTime) override;
    void onCollison(sf::Vector2f direction) override;
    void die() override;

    //Accesors
    Collider GetCollider() override;
    sf::FloatRect getBounds() override;
    sf::FloatRect getAttackBounds() override;
    bool isAttacking() override;
    bool isDead() override;
private:
    std::map<std::string, sf::Texture*> &textures;
    Animation animation;
    bool faceRight;
    sf::Vector2f velocity;
    float speed;

    AnimMovingState AI_State;
    float maxMovingTime;    // czas po ktorym przeciwnik ma zaatakowac, a nastpenie isc w druga strone i powtorzyc cykl
    float totalMovingTime;  // moment tego cyklu
    float maxDeathTimer;
    float totalDeathTimer;
    bool isAlive;
    bool isDying;

    sf::RectangleShape attackHitbox;
    float attackTimer;
    bool attacking;

    sf::Sprite sprite;
    sf::RectangleShape hitbox;
    sf::Vector2f hitboxOffset; // offsety hitboxu od sprite'a
    sf::Vector2f hitboxSize;
};


    //5===================================================================================================================
class FlyingEye : public Enemy
{
public:
    FlyingEye(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos);

    void render(sf::RenderTarget *target) override;
    void update(float deltaTime) override;
    void onCollison(sf::Vector2f direction) override;
    void die() override;

    //Accesors
    Collider GetCollider() override;
    sf::FloatRect getBounds() override;
    sf::FloatRect getAttackBounds() override;
    bool isAttacking() override;
    bool isDead() override;
private:
    std::map<std::string, sf::Texture*> &textures;
    Animation animation;
    bool faceRight;
    sf::Vector2f velocity;
    float speed;

    AnimMovingState AI_State;
    float maxMovingTime;    // czas po ktorym przeciwnik ma zaatakowac, a nastpenie isc w druga strone i powtorzyc cykl
    float totalMovingTime;  // moment tego cyklu
    float maxDeathTimer;
    float totalDeathTimer;
    bool isAlive;
    bool isDying;

    sf::RectangleShape attackHitbox;
    float attackTimer;
    bool attacking;

    sf::Sprite sprite;
    sf::RectangleShape hitbox;
    sf::Vector2f hitboxOffset; // offsety hitboxu od sprite'a
    sf::Vector2f hitboxSize;
};


#endif // ENTITIES_H
