#ifndef GAME_H
#define GAME_H

#include "stdafx.h"
#include "Entities.h"
#include "Tile.h"
#include "Player.h"


enum GameState {
    StartingMenu,
    Playing,
    FinalScreenMenu
};

class Game
{
private:

    //Window and View
    float width;
    float height;
    sf::VideoMode videoMode;
    sf::RenderWindow* window;
    sf::View* view;
    sf::FloatRect viewBounds;


    //TileMap
    std::vector<std::vector<Tile>> tileMap;
    sf::RectangleShape tileSelector;
    sf::Vector2i mapSize;
    float gridSizeF;
    unsigned gridSizeU;

    //Map rendering optimasation variables
    int fromX = 0;
    int toX = 0;
    int fromY = 0;
    int toY = 0;

    //Textures container
    std::map<std::string, std::map<std::string, sf::Texture*>> textures;

    //Entities container and variables
    std::vector<Enemy*> enemies;
    sf::Vector2f enemyDirection;

    Player* player;
    sf::Vector2f playerPos;
    float playerSpeed;
    float jumpHeight;
    float animSpeed;
    sf::Vector2f playerDirection;

    //MainMenu / FinalScreen variables
    sf::Sprite background;
    sf::Font font;
    std::map<std::string, sf::Text*> texts;
    bool win;

    //Game variables
    GameState gameState;
    sf::Event event;
    sf::Clock dtCloak;
    float dt = 0.0f;

    //Mouse positions
    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;
    sf::Vector2u mousePosGrid;

    //Private functions, initializers
    void initWindowAndView();
    void initVariables();
    void initTextures();
    void initMenu();
    void initTileMap();
public:

    //Constructor /Destructor
    Game();
    ~Game();

    //Public functions
    void run();

    //Updating
    void updateMousePos();
    void updateEntities();
    void updateView();
    void updateCollisions();
    void updateTileMap();
    void updateCombat();

    void updateMenu();
    void updatePlaying();
    void updateFinalScreen();

    //Rendering
    void renderTileMap();

    void renderMenu();
    void renderPlaying();
    void renderFinalScreen();
};

#endif // GAME_H
