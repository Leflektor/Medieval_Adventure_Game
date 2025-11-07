#include "PlayerAnimation.h"

PlayerAnimation::PlayerAnimation(sf::Texture* texture, sf::Vector2u texImageCount, float switchTime)
    :switchTime(switchTime), texImageCount(texImageCount)
{
    this->totalTime = 0.f;
    this->currentImage.x = 0;

    this->uvRect.width  = texture->getSize().x / float(texImageCount.x) - 5;
    this->uvRect.height = texture->getSize().y / float(texImageCount.y);
}

void PlayerAnimation::update(float deltaTime, bool faceRight, unsigned int animImageCount, unsigned int columnOffset)
{
    this->totalTime += deltaTime;

    if(this->totalTime >= this->switchTime)
    {
        this->totalTime -= this->switchTime;
        this->currentImage.x++;

        if(this->currentImage.x >= this->texImageCount.x)
        {
            this->currentImage.x = 0.f;
            if(animImageCount + columnOffset > this->texImageCount.x)
            {
                ++this->currentImage.y;
            }
        }
        if(this->currentImage.x >= (animImageCount + columnOffset) % this->texImageCount.x)
        {
            if(this->currentImage.x < columnOffset)
            {
                this->currentImage.x = columnOffset;
                if(animImageCount + columnOffset > this->texImageCount.x)
                {
                    --this->currentImage.y;
                }
            }
            else if(animImageCount + columnOffset < this->texImageCount.x)
            {
                this->currentImage.x = columnOffset;
            }
        }
    }

    this->uvRect.top = this->currentImage.y * this->uvRect.height;
    if(faceRight)
    {
        this->uvRect.left = this->currentImage.x * this->uvRect.width;
        this->uvRect.width = abs(this->uvRect.width);
    }
    else
    {
        this->uvRect.left = (this->currentImage.x + 1) * abs(this->uvRect.width);
        this->uvRect.width = -abs(this->uvRect.width);
    }
}


void PlayerAnimation::setRowAndOffset(int row,unsigned int columnOffset)
{
    this->currentImage.x = columnOffset;
    this->currentImage.y = row;
}
