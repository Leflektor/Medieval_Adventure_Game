#ifndef PLAYERANIMATION_H
#define PLAYERANIMATION_H
#include <stdafx.h>

class PlayerAnimation
{
    //methods
private:

public:
    PlayerAnimation(sf::Texture* texture, sf::Vector2u texImageCount, float switchTime);
    void update(float deltaTime, bool faceRight, unsigned int animImageCount, unsigned int columnOffset);
    void setRowAndOffset(int row, unsigned int columnOffset);

    //variables
private:
    sf::Vector2u texImageCount;
    sf::Vector2u currentImage;

    float totalTime;
    float switchTime;

public:
    sf::IntRect uvRect;

};

#endif // PLAYERANIMATION_H
