#ifndef TILE_H
#define TILE_H

#include "stdafx.h"
#include "Collider.h"


class Tile
{
private:
    sf::Sprite sprite;
    sf::RectangleShape hitbox;
    bool collison;

    //Private functions

public:
    //Constructor / Destructors
    Tile();
    ~Tile();

    //Public functions
    //Accesors
    bool getCollison();
    Collider GetCollider();
    sf::FloatRect getBounds();
    void setTile(const sf::Texture &texture, const float gridSize, int x, int y, bool collision);
    void renderTile(sf::RenderTarget *target);
};

#endif // TILE_H
