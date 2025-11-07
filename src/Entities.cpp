#include "Entities.h"


//1 =========================================================================================================
EvilWizard::EvilWizard(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :textures(tex), animation(textures, switchTime), speed(speed)
{
    this->sprite.setTexture(*this->textures["Idle"]);
    this->sprite.setTextureRect(sf::IntRect(0, 0, textures["Idle"]->getSize().y, textures["Idle"]->getSize().y));
    this->sprite.setPosition(pos.x, pos.y);
    this->sprite.setScale(
                750.f / this->sprite.getGlobalBounds().width,
                750.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 340;
    this->hitboxOffset.y = 329;
    this->hitboxSize.x = 90;
    this->hitboxSize.y = 170;

    this->AI_State = AnimMovingState::Idle;
    this->maxMovingTime = 1.2f;
    this->totalMovingTime = 0.0f;
    this->maxDeathTimer = 1.05f;
    this->totalDeathTimer = 0.0f;
    this->attackTimer = 0.0f;
    this->isAlive = true;
    this->isDying = false;
    this->faceRight = false;
    this->attacking = false;

    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->hitbox.setSize(hitboxSize);
    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineThickness(1.f);
    this->hitbox.setOutlineColor(sf::Color::Green);

    this->attackHitbox.setSize(sf::Vector2f(320.f, 370.f));
    this->attackHitbox.setFillColor(sf::Color::Transparent);
    this->attackHitbox.setOutlineThickness(1.f);
    this->attackHitbox.setOutlineColor(sf::Color::Red);
}


void EvilWizard::update(float deltaTime) //✅
{
    this->attacking = false;
    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->velocity.x = 0.0f;
    this->velocity.y += 9.81f * 100.0f * deltaTime;

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
        this->totalDeathTimer += deltaTime;
        if(this->totalDeathTimer > maxDeathTimer)
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
            if(this->attackTimer > 1.2f)
                this->attackTimer -= 1.2f;
            if(this->attackTimer > 0.60f && this->attackTimer < 1.05f)
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
            if(this->totalDeathTimer < 1.0f)
            {
                this->animation.update(deltaTime, this->faceRight);
                this->sprite.setTextureRect(this->animation.uvRect);
            }
            break;
        }
    }

    if(this->faceRight)
    {
        this->attackHitbox.setPosition(this->hitbox.getPosition().x + this->hitboxSize.x, this->hitbox.getPosition().y - 200.0f);
    }
    else
    {
        this->attackHitbox.setPosition(this->hitbox.getPosition().x - this->attackHitbox.getGlobalBounds().width, this->hitbox.getPosition().y - 200.0f);
    }

    this->sprite.move(velocity * deltaTime);
}


void EvilWizard::render(sf::RenderTarget *target) //✅
{
    target->draw(this->sprite);
//    if(this->attacking)
//    if(false)
//    {
//        target->draw(this->hitbox);
//        target->draw(this->attackHitbox);
//    }
}


void EvilWizard::onCollison(sf::Vector2f direction) //✅
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


void EvilWizard::die() //✅
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


Collider EvilWizard::GetCollider() //✅
{
    return Collider(this->sprite, this->hitbox);
}


sf::FloatRect EvilWizard::getBounds() //✅
{
    return this->hitbox.getGlobalBounds();
}


sf::FloatRect EvilWizard::getAttackBounds() //✅
{
    return this->attackHitbox.getGlobalBounds();
}


bool EvilWizard::isAttacking() //✅
{
    return this->attacking;
}


bool EvilWizard::isDead() //✅
{
    return !this->isAlive;
}


//2 =========================================================================================================
Skeleton::Skeleton(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :textures(tex), animation(textures, switchTime), speed(speed)
{
    this->sprite.setTexture(*this->textures["Idle"]);
    this->sprite.setTextureRect(sf::IntRect(0, 0, textures["Idle"]->getSize().y, textures["Idle"]->getSize().y));
    this->sprite.setPosition(pos.x, pos.y);
    this->sprite.setScale(
                400.f / this->sprite.getGlobalBounds().width,
                400.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 150.0f;
    this->hitboxOffset.y = 132.0f;
    this->hitboxSize.x = 100.0f;
    this->hitboxSize.y = 135.0f;

    this->AI_State = AnimMovingState::Idle;
    this->maxMovingTime = 1.2f;
    this->totalMovingTime = 0.0f;
    this->maxDeathTimer = 0.6f;
    this->totalDeathTimer = 0.0f;
    this->attackTimer = 0.0f;
    this->isAlive = true;
    this->isDying = false;
    this->faceRight = false;
    this->attacking = false;

    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->hitbox.setSize(hitboxSize);
    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineThickness(1.f);
    this->hitbox.setOutlineColor(sf::Color::Green);

    this->attackHitbox.setSize(sf::Vector2f(150.f, 135.f));
    this->attackHitbox.setFillColor(sf::Color::Transparent);
    this->attackHitbox.setOutlineThickness(1.f);
    this->attackHitbox.setOutlineColor(sf::Color::Red);
}


void Skeleton::update(float deltaTime) //✅
{
    this->attacking = false;
    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->velocity.x = 0.0f;
    this->velocity.y += 9.81f * 100.0f * deltaTime;

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
        this->totalDeathTimer += deltaTime;
        if(this->totalDeathTimer > maxDeathTimer)
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
            if(this->attackTimer > 1.2f)
                this->attackTimer -= 1.2f;
            if(this->attackTimer > 0.9f && this->attackTimer < 1.2f)
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
            if(this->totalDeathTimer < 0.55f)
            {
                this->animation.update(deltaTime, this->faceRight);
                this->sprite.setTextureRect(this->animation.uvRect);
            }
            break;
        }
    }

    if(this->faceRight)
    {
        this->attackHitbox.setPosition(this->hitbox.getPosition().x + this->hitboxSize.x, this->hitbox.getPosition().y);
    }
    else
    {
        this->attackHitbox.setPosition(this->hitbox.getPosition().x - this->attackHitbox.getGlobalBounds().width, this->hitbox.getPosition().y);
    }

    this->sprite.move(velocity * deltaTime);
}


void Skeleton::render(sf::RenderTarget *target) //✅
{
    target->draw(this->sprite);
//    if(this->attacking)
//    if(true)
//    {
//        target->draw(this->hitbox);
//        target->draw(this->attackHitbox);
//    }
}


void Skeleton::onCollison(sf::Vector2f direction) //✅
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


void Skeleton::die() //✅
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


Collider Skeleton::GetCollider() //✅
{
    return Collider(this->sprite, this->hitbox);
}


sf::FloatRect Skeleton::getBounds() //✅
{
    return this->hitbox.getGlobalBounds();
}


sf::FloatRect Skeleton::getAttackBounds() //✅
{
    return this->attackHitbox.getGlobalBounds();
}


bool Skeleton::isAttacking() //✅
{
    return this->attacking;
}


bool Skeleton::isDead() //✅
{
    return !this->isAlive;
}


//3 =========================================================================================================
Mushroom::Mushroom(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :textures(tex), animation(textures, switchTime), speed(speed)
{
    this->sprite.setTexture(*this->textures["Idle"]);
    this->sprite.setTextureRect(sf::IntRect(0, 0, textures["Idle"]->getSize().y, textures["Idle"]->getSize().y));
    this->sprite.setPosition(pos.x, pos.y);
    this->sprite.setScale(
                400.f / this->sprite.getGlobalBounds().width,
                400.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 164.0f;
    this->hitboxOffset.y = 167.0f;
    this->hitboxSize.x = 72.0f;
    this->hitboxSize.y = 100.0f;

    this->AI_State = AnimMovingState::Idle;
    this->maxMovingTime = 1.2f;
    this->totalMovingTime = 0.0f;
    this->maxDeathTimer = 0.6f;
    this->totalDeathTimer = 0.0f;
    this->attackTimer = 0.0f;
    this->isAlive = true;
    this->isDying = false;
    this->faceRight = false;
    this->attacking = false;

    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->hitbox.setSize(hitboxSize);
    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineThickness(1.f);
    this->hitbox.setOutlineColor(sf::Color::Green);

    this->attackHitbox.setSize(sf::Vector2f(65.f, 100.f));
    this->attackHitbox.setFillColor(sf::Color::Transparent);
    this->attackHitbox.setOutlineThickness(1.f);
    this->attackHitbox.setOutlineColor(sf::Color::Red);
}


void Mushroom::update(float deltaTime) //✅
{
    this->attacking = false;
    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->velocity.x = 0.0f;
    this->velocity.y += 9.81f * 100.0f * deltaTime;

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
        this->totalDeathTimer += deltaTime;
        if(this->totalDeathTimer > maxDeathTimer)
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
            if(this->attackTimer > 1.2f)
                this->attackTimer -= 1.2f;
            if(this->attackTimer > 0.9f && this->attackTimer < 1.2f)
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
            if(this->totalDeathTimer < 0.55f)
            {
                this->animation.update(deltaTime, this->faceRight);
                this->sprite.setTextureRect(this->animation.uvRect);
            }
            break;
        }
    }

    if(this->faceRight)
    {
        this->attackHitbox.setPosition(this->hitbox.getPosition().x + this->hitboxSize.x, this->hitbox.getPosition().y);
    }
    else
    {
        this->attackHitbox.setPosition(this->hitbox.getPosition().x - this->attackHitbox.getGlobalBounds().width, this->hitbox.getPosition().y);
    }

    this->sprite.move(velocity * deltaTime);
}


void Mushroom::render(sf::RenderTarget *target) //✅
{
    target->draw(this->sprite);
//    if(this->attacking)
//    if(true)
//    {
//        target->draw(this->hitbox);
//        target->draw(this->attackHitbox);
//    }
}


void Mushroom::onCollison(sf::Vector2f direction) //✅
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


void Mushroom::die() //✅
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


Collider Mushroom::GetCollider() //✅
{
    return Collider(this->sprite, this->hitbox);
}


sf::FloatRect Mushroom::getBounds() //✅
{
    return this->hitbox.getGlobalBounds();
}


sf::FloatRect Mushroom::getAttackBounds() //✅
{
    return this->attackHitbox.getGlobalBounds();
}


bool Mushroom::isAttacking() //✅
{
    return this->attacking;
}


bool Mushroom::isDead() //✅
{
    return !this->isAlive;
}


//4 =========================================================================================================
Goblin::Goblin(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :textures(tex), animation(textures, switchTime), speed(speed)
{
    this->sprite.setTexture(*this->textures["Idle"]);
    this->sprite.setTextureRect(sf::IntRect(0, 0, textures["Idle"]->getSize().y, textures["Idle"]->getSize().y));
    this->sprite.setPosition(pos.x, pos.y);
    this->sprite.setScale(
                400.f / this->sprite.getGlobalBounds().width,
                400.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 150.0f;
    this->hitboxOffset.y = 167.0f;
    this->hitboxSize.x = 100.0f;
    this->hitboxSize.y = 100.0f;

    this->AI_State = AnimMovingState::Idle;
    this->maxMovingTime = 1.2f;
    this->totalMovingTime = 0.0f;
    this->maxDeathTimer = 0.6f;
    this->totalDeathTimer = 0.0f;
    this->attackTimer = 0.0f;
    this->isAlive = true;
    this->isDying = false;
    this->faceRight = false;
    this->attacking = false;

    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->hitbox.setSize(hitboxSize);
    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineThickness(1.f);
    this->hitbox.setOutlineColor(sf::Color::Green);

    this->attackHitbox.setSize(sf::Vector2f(230.f, 115.f));
    this->attackHitbox.setFillColor(sf::Color::Transparent);
    this->attackHitbox.setOutlineThickness(1.f);
    this->attackHitbox.setOutlineColor(sf::Color::Red);
}


void Goblin::update(float deltaTime) //✅
{
    this->attacking = false;
    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->velocity.x = 0.0f;
    this->velocity.y += 9.81f * 100.0f * deltaTime;

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
        this->totalDeathTimer += deltaTime;
        if(this->totalDeathTimer > maxDeathTimer)
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
            if(this->attackTimer > 1.2f)
                this->attackTimer -= 1.2f;
            if(this->attackTimer > 0.9f && this->attackTimer < 1.2f)
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
            if(this->totalDeathTimer < 0.55f)
            {
                this->animation.update(deltaTime, this->faceRight);
                this->sprite.setTextureRect(this->animation.uvRect);
            }
            break;
        }
    }

    this->attackHitbox.setPosition(this->hitbox.getPosition().x - 65.f, this->hitbox.getPosition().y - 15.f);

    this->sprite.move(velocity * deltaTime);
}


void Goblin::render(sf::RenderTarget *target) //✅
{
    target->draw(this->sprite);
//    if(this->attacking)
//    if(true)
//    {
//        target->draw(this->hitbox);
//        target->draw(this->attackHitbox);
//    }
}


void Goblin::onCollison(sf::Vector2f direction) //✅
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


void Goblin::die() //✅
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


Collider Goblin::GetCollider() //✅
{
    return Collider(this->sprite, this->hitbox);
}


sf::FloatRect Goblin::getBounds() //✅
{
    return this->hitbox.getGlobalBounds();
}


sf::FloatRect Goblin::getAttackBounds() //✅
{
    return this->attackHitbox.getGlobalBounds();
}


bool Goblin::isAttacking() //✅
{
    return this->attacking;
}


bool Goblin::isDead() //✅
{
    return !this->isAlive;
}


//5 =========================================================================================================
FlyingEye::FlyingEye(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2f pos) //✅
    :textures(tex), animation(textures, switchTime), speed(speed)
{
    this->sprite.setTexture(*this->textures["Idle"]);
    this->sprite.setTextureRect(sf::IntRect(0, 0, textures["Idle"]->getSize().y, textures["Idle"]->getSize().y));
    this->sprite.setPosition(pos.x, pos.y);
    this->sprite.setScale(
                400.f / this->sprite.getGlobalBounds().width,
                400.f / this->sprite.getGlobalBounds().width
                );

    this->hitboxOffset.x = 150.0f;
    this->hitboxOffset.y = 180.0f;
    this->hitboxSize.x = 100.0f;
    this->hitboxSize.y = 50.0f;

    this->AI_State = AnimMovingState::Idle;
    this->maxMovingTime = 1.2f;
    this->totalMovingTime = 0.0f;
    this->maxDeathTimer = 0.6f;
    this->totalDeathTimer = 0.0f;
    this->attackTimer = 0.0f;
    this->isAlive = true;
    this->isDying = false;
    this->faceRight = false;
    this->attacking = false;

    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->hitbox.setSize(hitboxSize);
    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineThickness(1.f);
    this->hitbox.setOutlineColor(sf::Color::Green);

    this->attackHitbox.setSize(sf::Vector2f(75.f, 100.f));
    this->attackHitbox.setFillColor(sf::Color::Transparent);
    this->attackHitbox.setOutlineThickness(1.f);
    this->attackHitbox.setOutlineColor(sf::Color::Red);
}


void FlyingEye::update(float deltaTime) //✅
{
    this->attacking = false;
    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->velocity.x = 0.0f;
//    this->velocity.y += 9.81f * 100.0f * deltaTime;  //No jednak FLYINGEYE

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
        this->velocity.y += 9.81f * 100.0f * deltaTime;
        this->totalDeathTimer += deltaTime;
        if(this->totalDeathTimer > maxDeathTimer)
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
            if(this->attackTimer > 1.2f)
                this->attackTimer -= 1.2f;
            if(this->attackTimer > 0.9f && this->attackTimer < 1.2f)
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
            if(this->totalDeathTimer < 0.55f)
            {
                this->animation.update(deltaTime, this->faceRight);
                this->sprite.setTextureRect(this->animation.uvRect);
            }
            break;
        }
    }

    this->attackHitbox.setPosition(this->hitbox.getPosition().x - 25.f, this->hitbox.getPosition().y - 25.f);

    this->sprite.move(velocity * deltaTime);
}


void FlyingEye::render(sf::RenderTarget *target) //✅
{
    target->draw(this->sprite);
//    if(this->attacking)
//    if(true)
//    {
//        target->draw(this->hitbox);
//        target->draw(this->attackHitbox);
//    }
}


void FlyingEye::onCollison(sf::Vector2f direction) //✅
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


void FlyingEye::die() //✅
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


Collider FlyingEye::GetCollider() //✅
{
    return Collider(this->sprite, this->hitbox);
}


sf::FloatRect FlyingEye::getBounds() //✅
{
    return this->hitbox.getGlobalBounds();
}


sf::FloatRect FlyingEye::getAttackBounds() //✅
{
    return this->attackHitbox.getGlobalBounds();
}


bool FlyingEye::isAttacking() //✅
{
    return this->attacking;
}


bool FlyingEye::isDead() //✅
{
    return !this->isAlive;
}

