#ifndef ANIMATION_H
#define ANIMATION_H

#include "stdafx.h"

enum AnimMovingState
{
    MovingLeft,
    Attack,
    MovingRight,
    Idle,
    Death
};


class Animation
{
public:
    Animation(std::map<std::string, sf::Texture*> &tex, float switchTime);
    void update(float deltaTime, bool faceRight);
    void updateImageCount(AnimMovingState state);
    void resetForDeath();

    unsigned int currentImage;
    float texImageCount;
    float totalTime;
    float switchTime;
    sf::IntRect uvRect;

private:
    std::map<std::string, sf::Texture*> &textures;
};
#endif // ANIMATION_H
