#include "Tile.h"


    //Constructor /Destructor
Tile::Tile()
{
}


Tile::~Tile()
{
}


    //Public functions
bool Tile::getCollison()
{
    return this->collison;
}


Collider Tile::GetCollider()
{
    return Collider(this->sprite, this->hitbox);
}


sf::FloatRect Tile::getBounds()
{
    return this->sprite.getGlobalBounds();
}


void Tile::setTile(const sf::Texture &texture, const float gridSize, int x, int y, bool collision)
{
    this->collison = collision;

    this->sprite.setTexture(texture);

    this->sprite.setScale
                (
                gridSize / this->sprite.getGlobalBounds().width,
                gridSize / this->sprite.getGlobalBounds().height
                );
    this->sprite.setPosition(x * gridSize, y * gridSize);
    this->hitbox.setPosition(this->sprite.getPosition());
    this->hitbox.setSize(sf::Vector2f(this->sprite.getGlobalBounds().width, this->sprite.getGlobalBounds().height));
}


void Tile::renderTile(sf::RenderTarget *target)
{
    target->draw(this->sprite);
}

