#include "Collider.h"


Collider::Collider(sf::Sprite& sprite, sf::RectangleShape& hitbox)
    : sprite(sprite), hitbox(hitbox)
{

}


Collider::~Collider()
{

}


void Collider::Move(float dx, float dy)
{
    sprite.move(dx, dy);
}


bool Collider::CheckCollision(Collider other, sf::Vector2f& direction, float push)
{
    sf::Vector2f otherHalfSize = other.GetHalfSize();
    sf::Vector2f otherPosition = other.GetPosition() + otherHalfSize;
    sf::Vector2f thisHalfSize = GetHalfSize();
    sf::Vector2f thisPosition = GetPosition() + thisHalfSize;

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;

    float intersectX = abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    if(intersectX < 0.0f && intersectY < 0.0f)
    {
        push = std::min(std::max(push, 0.0f), 1.0f);

        if(abs(intersectX) < abs(intersectY))
        {
            if(deltaX > 0.0f)
            {
                Move(intersectX * (1.0f - push),0.0f);
                other.Move(-intersectX * push, 0.0f);

                direction.x = 1.f;
                direction.y = 0.f;
            }
            else
            {
                Move(-intersectX * (1.0f - push),0.0f);
                other.Move(intersectX * push, 0.0f);

                direction.x = -1.f;
                direction.y = 0.f;
            }
        }
        else
        {
            if(deltaY > 0.0f)
            {
                Move(0.0f, intersectY * (1.0f - push));
                other.Move(0.0f, -intersectY * push);

                direction.x = 0.f;
                direction.y = 1.f;
            }
            else
            {
                Move(0.0f, -intersectY * (1.0f - push));
                other.Move(0.0f, intersectY * push);

                direction.x = 0.f;
                direction.y = -1.f;
            }
        }
        return true;
    }
    return false;
}


sf::Vector2f Collider::GetPosition()
{
    return hitbox.getPosition();
}


sf::Vector2f Collider::GetHalfSize()
{
    sf::Vector2f HalfSize;
    HalfSize.x = hitbox.getGlobalBounds().width / 2.0f;
    HalfSize.y = hitbox.getGlobalBounds().height / 2.0f;
    return HalfSize;
}
