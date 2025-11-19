#include "Enemies.h"


// 0 =========================================================================================================
Enemy::Enemy(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos)
    : textures(tex), animation(textures, switchTime), speed(speed)
{
    this->sprite.setTexture(*this->textures["Idle"]);
    this->sprite.setTextureRect(sf::IntRect(0, 0, textures["Idle"]->getSize().y, textures["Idle"]->getSize().y));
    this->sprite.setPosition(pos.x, pos.y);

    this->AI_State = AnimMovingState::Idle;
    this->maxMovingTime = 1.2f;
    this->totalMovingTime = 0.0f;
    this->deathElapsedTime = 0.0f;
    this->attackTimer = 0.0f;

    this->isAlive = true;
    this->isDying = false;
    this->faceRight = false;
    this->attacking = false;

    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineThickness(1.f);
    this->hitbox.setOutlineColor(sf::Color::Green);
    this->attackHitbox.setFillColor(sf::Color::Transparent);
    this->attackHitbox.setOutlineThickness(1.f);
    this->attackHitbox.setOutlineColor(sf::Color::Red);
}


void Enemy::update(float deltaTime) //✅
{
    this->attacking = false;
    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->velocity.x = 0.0f;

    if(!isFlying)
    {
        this->velocity.y += 9.81f * 100.0f * deltaTime;
    }

    //AI xD
    this->totalMovingTime += deltaTime;
    if(!this->isDying)
    {
        if(this->totalMovingTime > this->maxMovingTime)
        {
            this->totalMovingTime -= this->maxMovingTime;
            switch(this->AI_State)
            {
                case AnimMovingState::Idle:
                {
                    this->AI_State = AnimMovingState::MovingLeft;
                    this->sprite.setTexture(*this->textures["Run"]);
                    break;
                }
                case AnimMovingState::MovingLeft:
                {
                    this->AI_State = AnimMovingState::Attack;
                    this->sprite.setTexture(*this->textures["Attack"]);
                    break;
                }
                case AnimMovingState::Attack:
                {
                    this->AI_State = AnimMovingState::MovingRight;
                    this->sprite.setTexture(*this->textures["Run"]);
                    this->attackTimer = 0.0f;
                    this->faceRight = true;
                    break;
                }
                case AnimMovingState::MovingRight:
                {
                    this->AI_State = AnimMovingState::Idle;
                    this->sprite.setTexture(*this->textures["Idle"]);
                    this->faceRight = false;
                    break;
                }
                case AnimMovingState::Death:
                {
                    break;
                }
            }
            this->animation.updateImageCount(this->AI_State);
        }
    }
    else
    {
        if(isFlying)
        {
            this->velocity.y += 9.81f * 100.0f * deltaTime;
        }
        this->deathElapsedTime += deltaTime;
        if(this->deathElapsedTime > this->deathDuartion)
        {
            this->isAlive = false;
        }
    }

    switch(this->AI_State)
    {
        case AnimMovingState::Idle:
        {
            this->animation.update(deltaTime, this->faceRight);
            this->sprite.setTextureRect(this->animation.uvRect);
            break;
        }
        case AnimMovingState::MovingLeft:
        {
            this->velocity.x -= this->speed;
            this->animation.update(deltaTime, this->faceRight);
            this->sprite.setTextureRect(this->animation.uvRect);
            break;
        }
        case AnimMovingState::Attack:
        {
            this->attackTimer += deltaTime;
            if(this->attackTimer > this->maxMovingTime)
                this->attackTimer -= this->maxMovingTime;
            if(this->attackTimer > this->attackTimerStart 
            && this->attackTimer < this->attackTimerStop)
            {
                this->attacking = true;
            }
            this->animation.update(deltaTime, this->faceRight);
            this->sprite.setTextureRect(this->animation.uvRect);
            break;
        }
        case AnimMovingState::MovingRight:
        {
            this->velocity.x += this->speed;
            this->animation.update(deltaTime, this->faceRight);
            this->sprite.setTextureRect(this->animation.uvRect);
            break;
        }
        case AnimMovingState::Death:
        {
            if(this->deathElapsedTime < this->deathDuartion - 0.01f) // Animation ends 0.01 second faster so it doesnt loop and show first frame of death animation
            {
                this->animation.update(deltaTime, this->faceRight);
                this->sprite.setTextureRect(this->animation.uvRect);
            }
            break;
        }
    }

    if(this->faceRight)
    {
        this->attackHitbox.setPosition(
            this->hitbox.getPosition().x + this->hitboxSize.x - this->attackHitboxOffset.x, 
            this->hitbox.getPosition().y - this->attackHitboxOffset.y);
    }
    else
    {
        this->attackHitbox.setPosition(
            this->hitbox.getPosition().x - this->attackHitbox.getGlobalBounds().width + this->attackHitboxOffset.x, 
            this->hitbox.getPosition().y - this->attackHitboxOffset.y);
    }

    this->sprite.move(velocity * deltaTime);
}


void Enemy::onCollison(sf::Vector2f direction) //✅
{
    if(direction.x < 0.0f)
    {
        //Collison on the left
        this->velocity.x = 0.0f;
    }
    else if(direction.y  > 0.0f)
    {
        //Collison on the right
        this->velocity.x = 0.0f;
    }
    if(direction.y < 0.0f)
    {
        //Collison on the bottom
        this->velocity.y = 0.0f;
    }
    else if(direction.y > 0.0f)
    {
        //Collison on the top
        this->velocity.y = 0.0f;
    }
}


void Enemy::render(sf::RenderTarget *target) const//✅
{
    target->draw(this->sprite);
//    if(this->attacking)
//    if(false)
//    {
    //    target->draw(this->hitbox);
    //    target->draw(this->attackHitbox);
//    }
}


Collider Enemy::GetCollider() //✅
{
    return Collider(this->sprite, this->hitbox);
}


sf::FloatRect Enemy::getBounds() const //✅
{
    return this->hitbox.getGlobalBounds();
}


sf::FloatRect Enemy::getAttackBounds() const
{
    return this->attackHitbox.getGlobalBounds();
}


bool Enemy::isDead() const
{
    return !this->isAlive;
}


bool Enemy::isAttacking() const
{
    return this->attacking;
}


void Enemy::die() //✅
{
    if(!this->isDying)
    {
        this->AI_State = AnimMovingState::Death;
        this->sprite.setTexture(*this->textures["Death"]);
        this->animation.updateImageCount(this->AI_State);
        this->animation.resetForDeath();
        this->isDying = true;
    }
}

// 1 =========================================================================================================
EvilWizard::EvilWizard(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :Enemy(tex, switchTime, speed, pos)
{
    this->sprite.setScale(
                750.f / this->sprite.getGlobalBounds().width,
                750.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 340;
    this->hitboxOffset.y = 329;
    this->hitboxSize.x = 90;
    this->hitboxSize.y = 170;
    
    this->deathDuartion = 1.05f;
    this->isFlying = false;

    this->hitbox.setPosition(
        this->sprite.getPosition().x + this->hitboxOffset.x, 
        this->sprite.getPosition().y + this->hitboxOffset.y);

    this->hitbox.setSize(hitboxSize);
    this->attackHitbox.setSize(sf::Vector2f(320.f, 370.f));

    this->attackTimerStart = 0.60f;
    this->attackTimerStop = 1.05f;
    this->attackHitboxOffset.x = 0.f;
    this->attackHitboxOffset.y = 200.0f;
}


// 2 =========================================================================================================
Skeleton::Skeleton(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :Enemy(tex, switchTime, speed, pos)
{
    this->sprite.setScale(
                400.f / this->sprite.getGlobalBounds().width,
                400.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 150.0f;
    this->hitboxOffset.y = 132.0f;
    this->hitboxSize.x = 100.0f;
    this->hitboxSize.y = 135.0f;
    
    this->deathDuartion = 0.6f;
    this->isFlying = false;

    this->hitbox.setPosition(
        this->sprite.getPosition().x + this->hitboxOffset.x, 
        this->sprite.getPosition().y + this->hitboxOffset.y);

    this->hitbox.setSize(hitboxSize);
    this->attackHitbox.setSize(sf::Vector2f(150.f, 135.f));

    this->attackTimerStart = 0.9f;
    this->attackTimerStop = 1.2f;
    this->attackHitboxOffset.x = 0.f;
    this->attackHitboxOffset.y = 0.f;
}


// 3 =========================================================================================================
Mushroom::Mushroom(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :Enemy(tex, switchTime, speed, pos)
{
    this->sprite.setScale(
                400.f / this->sprite.getGlobalBounds().width,
                400.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 164.0f;
    this->hitboxOffset.y = 167.0f;
    this->hitboxSize.x = 72.0f;
    this->hitboxSize.y = 100.0f;

    this->deathDuartion = 0.6f;
    this->isFlying = false;

    this->hitbox.setPosition(
        this->sprite.getPosition().x + this->hitboxOffset.x, 
        this->sprite.getPosition().y + this->hitboxOffset.y);

    this->hitbox.setSize(hitboxSize);
    this->attackHitbox.setSize(sf::Vector2f(65.f, 100.f));

    this->attackTimerStart = 0.9f;
    this->attackTimerStop = 1.2f;
    this->attackHitboxOffset.x = 0.f;
    this->attackHitboxOffset.y = 0.f;
}


// 4 =========================================================================================================
Goblin::Goblin(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :Enemy(tex, switchTime, speed, pos)
{
    this->sprite.setScale(
                400.f / this->sprite.getGlobalBounds().width,
                400.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 150.0f;
    this->hitboxOffset.y = 167.0f;
    this->hitboxSize.x = 100.0f;
    this->hitboxSize.y = 100.0f;

    this->deathDuartion = 0.6f;
    this->isFlying = false;

    this->hitbox.setPosition(
        this->sprite.getPosition().x + this->hitboxOffset.x, 
        this->sprite.getPosition().y + this->hitboxOffset.y);

    this->hitbox.setSize(hitboxSize);
    this->attackHitbox.setSize(sf::Vector2f(240.f, 115.f));

    this->attackTimerStart = 0.9f;
    this->attackTimerStop = 1.2f;
    this->attackHitboxOffset.x = 170.f;
    this->attackHitboxOffset.y = 15.f;
}


// 5 =========================================================================================================
FlyingEye::FlyingEye(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :Enemy(tex, switchTime, speed, pos)
{
    this->sprite.setScale(
                400.f / this->sprite.getGlobalBounds().width,
                400.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 150.0f;
    this->hitboxOffset.y = 180.0f;
    this->hitboxSize.x = 100.0f;
    this->hitboxSize.y = 50.0f;

    this->deathDuartion = 0.6f;
    this->isFlying = true;

    this->hitbox.setPosition(
        this->sprite.getPosition().x + this->hitboxOffset.x, 
        this->sprite.getPosition().y + this->hitboxOffset.y);
    this->hitbox.setSize(hitboxSize);
    this->attackHitbox.setSize(sf::Vector2f(75.f, 100.f));

    this->attackTimerStart = 0.9f;
    this->attackTimerStop = 1.2f;
    this->attackHitboxOffset.x = 50.f;
    this->attackHitboxOffset.y = 25.f;
}
