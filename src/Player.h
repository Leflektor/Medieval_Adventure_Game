#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "Entities.h"
#include "PlayerAnimation.h"
#include "Collider.h"

//player sheet dimensions 50x37

class Player:Entity
{
    //methods
private:

public:
    Player(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2u imageCount, float jumpHeight);
    ~Player();
    void update(float deltaTime);
    void setPosition(const float x, const float y);
    void onCollison(sf::Vector2f direction);
    void render(sf::RenderTarget *target);

    //Collision
    Collider GetCollider();

    //Accessors
    sf::FloatRect getBounds();
    sf::FloatRect getAttackBounds();
    bool isAttacking();

    //variables
private:
    sf::Sprite sprite;
    sf::RectangleShape hitbox;
    sf::RectangleShape attackHitbox;
    sf::Vector2f hitboxOffset; // offsety hitboxu od sprite'a
    sf::Vector2f hitboxSize;

    float attackTimer;
    bool attacking;

    std::map<std::string, sf::Texture*> &textures;
    PlayerAnimation animation;
    unsigned int row;
    unsigned int offset; //offset klatek na texturesheecie
    unsigned int images;
    float speed;
    bool faceRight;
    sf::IntRect currentFrame;
    sf::Vector2f velocity;
    bool canJump;
    float jumpHeight;
public:

};

#endif // PLAYER_H
