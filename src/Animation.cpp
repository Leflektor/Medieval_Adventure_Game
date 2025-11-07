#include "Animation.h"

Animation::Animation(std::map<std::string, sf::Texture*> &tex, float switchTime)
    :textures(tex), switchTime(switchTime)
{
    this->currentImage = 0;
    this->totalTime = 0.f;

    //Warunki poczatkowe animacji
    this->texImageCount = this->textures["Idle"]->getSize().x / this->textures["Idle"]->getSize().y;
    this->uvRect.width  = this->textures["Idle"]->getSize().x / float(this->texImageCount);
    this->uvRect.height = this->textures["Idle"]->getSize().y;
}


void Animation::update(float deltaTime, bool faceRight)
{
    this->totalTime += deltaTime;

    if(this->totalTime >= this->switchTime)
    {
        this->totalTime -= this->switchTime;
        this->currentImage++;
        if(this->currentImage >= this->texImageCount)
            this->currentImage = 0;
    }

    if(faceRight)
    {
        this->uvRect.left = this->currentImage * this->uvRect.width;
        this->uvRect.width = abs(this->uvRect.width);
    }
    else
    {
        this->uvRect.left = (this->currentImage + 1) * abs(this->uvRect.width);
        this->uvRect.width = -abs(this->uvRect.width);
    }
}


void Animation::updateImageCount(AnimMovingState state)
{
    switch(state)
    {
        case AnimMovingState::Idle:
        {
            this->currentImage = 0;
            this->totalTime = 0.f;
            this->texImageCount = this->textures["Idle"]->getSize().x / this->textures["Idle"]->getSize().y;
            this->uvRect.width  = this->textures["Idle"]->getSize().x / float(this->texImageCount);
            this->uvRect.height = this->textures["Idle"]->getSize().y;
            break;
        }
        case AnimMovingState::MovingLeft:
        {
            this->currentImage = 0;
            this->totalTime = 0.f;
            this->texImageCount = this->textures["Run"]->getSize().x / this->textures["Run"]->getSize().y;
            this->uvRect.width  = this->textures["Run"]->getSize().x / float(this->texImageCount);
            this->uvRect.height = this->textures["Run"]->getSize().y;
            break;
        }
        case AnimMovingState::Attack:
        {
            this->currentImage = 0;
            this->totalTime = 0.f;
            this->texImageCount = this->textures["Attack"]->getSize().x / this->textures["Attack"]->getSize().y;
            this->uvRect.width  = this->textures["Attack"]->getSize().x / float(this->texImageCount);
            this->uvRect.height = this->textures["Attack"]->getSize().y;
            break;
        }
        case AnimMovingState::MovingRight:
        {
            this->currentImage = 0;
            this->totalTime = 0.f;
            this->texImageCount = this->textures["Run"]->getSize().x / this->textures["Run"]->getSize().y;
            this->uvRect.width  = this->textures["Run"]->getSize().x / float(this->texImageCount);
            this->uvRect.height = this->textures["Run"]->getSize().y;
            break;
        }
        case AnimMovingState::Death:
        {
            this->currentImage = 0;
            this->totalTime = 0.f;
            this->texImageCount = this->textures["Death"]->getSize().x / this->textures["Death"]->getSize().y;
            this->uvRect.width  = this->textures["Death"]->getSize().x / float(this->texImageCount);
            this->uvRect.height = this->textures["Death"]->getSize().y;
            break;
        }
    }
}


void Animation::resetForDeath()
{
    this->currentImage = 0;
    this->totalTime = 0.f;
}
