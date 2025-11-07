#include "Player.h"


    //Constructor /Destructor
Player::Player(std::map<std::string, sf::Texture*> &tex, float switchTime, float speed, sf::Vector2u imageCount, float jumpHeight)
    :textures(tex), animation(this->textures["Player_sheet"], imageCount, switchTime)
{
    this->attacking = false;
    this->jumpHeight = jumpHeight;
    this->speed = speed;
    this->row = 20;
    this->faceRight = true;
    this->velocity = sf::Vector2f(0, 0);

    this->sprite.setTexture(*this->textures["Player_sheet"]);
    this->sprite.setTextureRect(sf::IntRect(0, 0, 50, 36));

    this->sprite.setScale(
                150.f / this->sprite.getGlobalBounds().width,
                150.f / this->sprite.getGlobalBounds().width
                );

    //hitbox part

    this->hitboxOffset.x = 45.f;
    this->hitboxOffset.y = 20.f;
    this->hitboxSize.x = 60.f;
    this->hitboxSize.y = 86.f;

    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);
    this->hitbox.setSize(hitboxSize);
    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineThickness(1.f);
    this->hitbox.setOutlineColor(sf::Color::Green);

    this->attackHitbox.setSize(sf::Vector2f(40.f, 80.f));
    this->attackHitbox.setFillColor(sf::Color::Transparent);
    this->attackHitbox.setOutlineThickness(1.f);
    this->attackHitbox.setOutlineColor(sf::Color::Red);

}


Player::~Player()
{

}


    //Public functions
void Player::update(float deltaTime)
{
    this->attacking = false;

    this->hitbox.setPosition(this->sprite.getPosition().x + this->hitboxOffset.x, this->sprite.getPosition().y + this->hitboxOffset.y);

    this->velocity.x = 0.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) // ruszanie sie w lewo
        this->velocity.x -= this->speed;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) // ruszanie sie w prawo
        this->velocity.x += this->speed;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))  // zeby dalo sie zatrzymac w miejscu od razu
        this->velocity.x = 0.0f;

    if(this->velocity.y > 0)
        this->canJump = false;

    //Jumping
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->canJump)
    {
        this->canJump = false;
        this->velocity.y = -sqrtf(2.0f * 9.81f * 75.0f * this->jumpHeight);
    }

    //Gravity
    this->velocity.y += 9.81f * 100.0f * deltaTime;

    if(!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if(this->velocity.x == 0.0f)
        {
            //idle: row 0, offset 0, images 4
            //crouch: row 0, offset 4, images 4
            if(this->row != 0)
            {
                this->row = 0;
                this->offset = 0;
                this->images = 4;
                this->animation.setRowAndOffset(this->row, this->offset);
            }
            this->animation.update(deltaTime, this->faceRight, this->images, this->offset);
        }
        else
        {
            //run: row 1, offset 1, images 6
            if(this->row != 1)
            {
                this->row = 1;
                this->offset = 1;
                this->images = 6;
                this->animation.setRowAndOffset(this->row, this->offset);
            }
            if(velocity.x > 0.0f)
                this->faceRight = true;
            else
                this->faceRight = false;

            this->animation.update(deltaTime, this->faceRight, this->images, this->offset);
        }
    }

    //attcking and its' animation
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        this->attackTimer += deltaTime;
        if(this->attackTimer > 0.9f)
            this->attackTimer -= 0.9f;
        if(this->attackTimer > 0.30f && this->attackTimer < 0.75f)
            this->attacking = true;

        //attack2: row 6/7, offset 5, images 6
        if(this->row != 6)
        {
            this->row = 6;
            this->offset = 5;
            this->images = 6;
            this->animation.setRowAndOffset(this->row, this->offset);
        }
        if(velocity.x == 0.0f)
        {

        }
        else if(velocity.x > 0.0f)
        {
            this->faceRight = true;
        }
        else
        {
            this->faceRight = false;
        }

        this->animation.update(deltaTime, this->faceRight, this->images, this->offset);
    }
    else
    {
        this->attackTimer = 0.f;
    }

    if(this->faceRight)
    {
        this->attackHitbox.setPosition(this->hitbox.getPosition().x + 63.0f, this->hitbox.getPosition().y);
    }
    else
    {
        this->attackHitbox.setPosition(this->hitbox.getPosition().x - 42.0f, this->hitbox.getPosition().y);
    }

    this->sprite.setTextureRect(this->animation.uvRect);
    this->sprite.move(velocity * deltaTime);

}


void Player::render(sf::RenderTarget *target)
{
    target->draw(this->sprite);
//    if(this->attacking)
//    {
//        target->draw(this->hitbox);
//        target->draw(this->attackHitbox);
//    }
}


void Player::setPosition(const float x, const float y)
{
    this->sprite.setPosition(x - this->hitboxOffset.x + 1, y - this->hitboxOffset.y + 1);
}


void Player::onCollison(sf::Vector2f direction)
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
        this->canJump = true;
    }
    else if(direction.y > 0.0f)
    {
        //Collison on the top
        this->velocity.y = 0.0f;
    }
}


sf::FloatRect Player::getBounds()
{
    return this->hitbox.getGlobalBounds();
}


sf::FloatRect Player::getAttackBounds()
{
    return this->attackHitbox.getGlobalBounds();
}


Collider Player::GetCollider()
{
    return Collider(this->sprite, this->hitbox);
}


bool Player::isAttacking()
{
    return this->attacking;
}
