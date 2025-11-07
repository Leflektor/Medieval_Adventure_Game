#ifndef COLLIDER_H
#define COLLIDER_H

#include "stdafx.h"

class Collider
{
public:
    Collider(sf::Sprite& sprite, sf::RectangleShape& hitbox);
    ~Collider();

    void Move(float dx, float dy);

    bool CheckCollision(Collider&& other,sf::Vector2f& direction, float push);
    sf::Vector2f GetPosition();
    sf::Vector2f GetHalfSize();

private:
    sf::Sprite& sprite;
    sf::RectangleShape& hitbox;
};

#endif // COLLIDER_H
