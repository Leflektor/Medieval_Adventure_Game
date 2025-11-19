#include "Game.h"

    //Private functions /Initalizers
void Game::initVariables()
{
    //Game logic variables
    this->gameState = StartingMenu;
//    this->gameState = FinalScreenMenu;

    this->win = false;

    //Window variables
    this->width = 1920;
    this->height = 1001;

    //Player and enemies variables

    this->playerSpeed = 425.f;
    this->jumpHeight = 500.f;
    this->animSpeed = 0.15f;
    this->playerDirection.x = 0.f;
    this->playerDirection.y = 0.f;

    this->enemyDirection.x = 0.f;
    this->enemyDirection.y = 0.f;

    //Map and grid variables
    this->gridSizeF = 75.f;
    this->gridSizeU = static_cast<unsigned>(gridSizeF);
    this->mapSize.x = 150.f;
    this->mapSize.y = 15.f;
}


void Game::initWindowAndView()
{
    this->videoMode.width = width;
    this->videoMode.height = height;
    this->window = new sf::RenderWindow(videoMode, "Medieval Adventure! - Play in Fullscreen");
    this->view = new sf::View();
    this->view->setSize(width, height);
    this->view->setCenter(this->window->getSize().x / 2, this->window->getSize().y / 2);
}


void Game::initTextures()
{
    std::string string;
    std::string temp;
    std::fstream file;
    std::map<std::string, sf::Texture*> tempMap;

    file.open("Loaders/TexturesLoadingFile.txt", std::ios::in);
    if(file.is_open())
    {
        while(getline(file, string))
        {
            temp = string;
            while(getline(file, string) && string[0] != '=')
            {
                std::stringstream path;
                path << "Textures/" << temp << "/" << string << ".png";

                tempMap[string] = new sf::Texture();

                if(!tempMap[string]->loadFromFile(path.str()))
                    std::cout << "ERROR::COULD_NOT_OPEN_FROM_FILE::INIT_TEXTURES::Loaders/TexturesLoadingFile.txt\n";
            }
            this->textures[temp] = tempMap;
            tempMap.clear();
        }
    }

    //BACKGROUND
    tempMap["background"] = new sf::Texture();
    if(!tempMap["background"]->loadFromFile("Textures/Backgrounds/background.png"))
        std::cout << "ERROR::COULD_NOT_OPEN_FROM_FILE::INIT_TEXTURES::Textures/Backgrounds/background.png\n";
    this->textures["background"] = tempMap;
    tempMap.clear();
}


void Game::initMenu()
{
    auto initText = [this](const std::string& text){
        sf::Text t(text, this->font, 50);
        t.setFillColor(sf::Color::White);
        return t;
    };

    //load font
    if(!this->font.loadFromFile("Fonts/PixellettersFull.ttf"))
        std::cout << "ERROR::COULD_NOT_OPEN_FROM_FILE::INIT_MENU::Fonts/PixellettersFull.ttf\n";

    //initilize all captions
    this->texts["Title"] = initText("MEDIEVAL ADVENTTURE");
    this->texts["Game over"] = initText("GAME OVER");
    this->texts["Congratulations"] = initText("CONGRATULATIONS !");
    this->texts["You win"] = initText("YOU WIN");
    this->texts["Start"] = initText("START");
    this->texts["Options"] = initText("OPTIONS");
    this->texts["Quit"] = initText("QUIT");
    this->texts["Play again"] = initText("PLAY AGAIN");
    this->texts["MousePos"] =  initText("");

    this->texts["Title"].setCharacterSize(100);
    this->texts["Game over"].setCharacterSize(100);
    this->texts["You win"].setCharacterSize(100);
    this->texts["MousePos"].setCharacterSize(25);
    this->texts["Congratulations"].setCharacterSize(75);
    this->texts["MousePos"].setFillColor(sf::Color::Black);

    //Set captions position
    this->texts["Title"].setPosition(window->getSize().x / 2 - this->texts["Title"].getGlobalBounds().width / 2,
            window->getSize().y / 2 - 420);
    this->texts["You win"].setPosition(window->getSize().x / 2 - this->texts["You win"].getGlobalBounds().width / 2,
            window->getSize().y / 2 - 420);
    this->texts["Game over"].setPosition(window->getSize().x / 2 - this->texts["Game over"].getGlobalBounds().width / 2,
            window->getSize().y / 2 - 420);
    this->texts["Congratulations"].setPosition(window->getSize().x / 2 - this->texts["Congratulations"].getGlobalBounds().width / 2,
            window->getSize().y / 2 - 275);
    this->texts["Start"].setPosition(window->getSize().x / 2 - this->texts["Start"].getGlobalBounds().width / 2,
            window->getSize().y / 2 -100);
    this->texts["Play again"].setPosition(window->getSize().x / 2 - this->texts["Play again"].getGlobalBounds().width / 2,
            window->getSize().y / 2 - 100);
    this->texts["Options"].setPosition(window->getSize().x / 2 - this->texts["Options"].getGlobalBounds().width / 2,
            window->getSize().y / 2);
    this->texts["Quit"].setPosition(window->getSize().x / 2 - this->texts["Quit"].getGlobalBounds().width / 2,
            window->getSize().y / 2 + 100);
    this->texts["MousePos"].setPosition(20, 20);

    //initialize background
    this->background.setTexture(*textures["background"]["background"]);
    this->background.setScale
                (
                window->getSize().x / this->background.getGlobalBounds().width,
                window->getSize().y / this->background.getGlobalBounds().height
                );
}


void Game::initTileMap()
{
    //                    P - Player
    //                    E - Evil Wiazrd
    //                    S - Skeleton
    //                    F - Flying Eye
    //                    G - Goblin
    //                    M - Mushrom

    std::string string;
    std::fstream file;

    std::map<std::string, sf::Texture*>& temptex = textures["Tiles"];

    file.open("Loaders/Levels/Level1.txt", std::ios::in);
    if(file.is_open())
    {
        this->tileMap.resize(this->mapSize.y, std::vector<Tile>());

        for(int y = 0; y < this->mapSize.y; ++y)
        {
            this->tileMap[y].resize(this->mapSize.x, Tile());

            getline(file, string);
            for(int x = 0; x < this->mapSize.x; ++x)
            {
                switch(string[x])
                {
                    case '-':
                        this->tileMap[y][x].setTile(*temptex["grass_1"],this->gridSizeF, x, y, true);
                        break;
                    case 'L':
                        this->tileMap[y][x].setTile(*temptex["grass_2"],this->gridSizeF, x, y, true);
                        break;
                    case 'R':
                        this->tileMap[y][x].setTile(*temptex["grass_3"],this->gridSizeF, x, y, true);
                        break;
                    case '\\':
                        this->tileMap[y][x].setTile(*temptex["grass_4"],this->gridSizeF, x, y, true);
                        break;
                    case '/':
                        this->tileMap[y][x].setTile(*temptex["grass_5"],this->gridSizeF, x, y, true);
                        break;
                    case '1':
                        this->tileMap[y][x].setTile(*temptex["ground_1"],this->gridSizeF, x, y, true);
                        break;
                    case '2':
                        this->tileMap[y][x].setTile(*temptex["ground_2"],this->gridSizeF, x, y, true);
                        break;
                    case '3':
                        this->tileMap[y][x].setTile(*temptex["ground_3"],this->gridSizeF, x, y, true);
                        break;
                    case '4':
                        this->tileMap[y][x].setTile(*temptex["ground_4"],this->gridSizeF, x, y, true);
                        break;
                    case '5':
                        this->tileMap[y][x].setTile(*temptex["ground_5"],this->gridSizeF, x, y, true);
                        break;
                    case '6':
                        this->tileMap[y][x].setTile(*temptex["ground_6"],this->gridSizeF, x, y, true);
                        break;
                    case '7':
                        this->tileMap[y][x].setTile(*temptex["ground_7"],this->gridSizeF, x, y, true);
                        break;
                    case '8':
                        this->tileMap[y][x].setTile(*temptex["ground_8"],this->gridSizeF, x, y, true);
                        break;
                    case '9':
                        this->tileMap[y][x].setTile(*temptex["ground_9"],this->gridSizeF, x, y, true);
                        break;
                    case '0':
                        this->tileMap[y][x].setTile(*temptex["ground_10"],this->gridSizeF, x, y, true);
                        break;
                    case 'q':
                        this->tileMap[y][x].setTile(*temptex["ground_11"],this->gridSizeF, x, y, true);
                        break;
                    case 'P':
                        this->tileMap[y][x].setTile(*temptex["transparent"],this->gridSizeF, x, y, false);
                        this->player = new Player(this->textures["Player"], this->animSpeed, this->playerSpeed, sf::Vector2u(7, 16), this->jumpHeight);
                        this->player->setPosition(x * this->gridSizeF, y * this->gridSizeF);
                        break;
                    case 'E':
                        this->tileMap[y][x].setTile(*temptex["transparent"],this->gridSizeF, x, y, false);
                        this->enemies.push_back(new EvilWizard(this->textures["Evil Wizard"], this->animSpeed, this->playerSpeed, sf::Vector2f(this->gridSizeF*x, this->gridSizeF*y)));
                        break;
                    case 'S':
                        this->tileMap[y][x].setTile(*temptex["transparent"],this->gridSizeF, x, y, false);
                        this->enemies.push_back(new Skeleton(this->textures["Skeleton"], this->animSpeed, this->playerSpeed, sf::Vector2f(this->gridSizeF*x, this->gridSizeF*y)));
                        break;
                    case 'F':
                        this->tileMap[y][x].setTile(*temptex["transparent"],this->gridSizeF, x, y, false);
                        this->enemies.push_back(new FlyingEye(this->textures["Flying eye"], this->animSpeed, this->playerSpeed, sf::Vector2f(this->gridSizeF*x, this->gridSizeF*y)));
                        break;
                    case 'G':
                        this->tileMap[y][x].setTile(*temptex["transparent"],this->gridSizeF, x, y, false);
                        this->enemies.push_back(new Goblin(this->textures["Goblin"], this->animSpeed, this->playerSpeed, sf::Vector2f(this->gridSizeF*x, this->gridSizeF*y)));
                        break;
                    case 'M':
                        this->tileMap[y][x].setTile(*temptex["transparent"],this->gridSizeF, x, y, false);
                        this->enemies.push_back(new Mushroom(this->textures["Mushroom"], this->animSpeed, this->playerSpeed, sf::Vector2f(this->gridSizeF*x, this->gridSizeF*y)));
                        break;
                    default:
                        this->tileMap[y][x].setTile(*temptex["transparent"],this->gridSizeF, x, y, false);
                        break;
                }
            }
        }
        file.close();
    }
    this->tileSelector.setSize(sf::Vector2f(gridSizeF, gridSizeF));
    this->tileSelector.setFillColor(sf::Color::Transparent);
    this->tileSelector.setOutlineThickness(1.f);
    this->tileSelector.setOutlineColor(sf::Color::Green);
}


    //Constructor /Destructor
Game::Game()
{
    this->initVariables();
    this->initWindowAndView();
    this->initTextures();
    this->initMenu();
    this->initTileMap();
}


Game::~Game()
{
    this->textures.clear();
    delete this->player;
    delete this->window;
    this->enemies.clear();
    this->texts.clear();
}


    //Public functions
void Game::run()
{
    while(this->window->isOpen())
    {
        while(this->window->pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                this->window->close();
            }
        }
        switch(this->gameState)
        {
        case StartingMenu:
            this->updateMenu();
            this->renderMenu();
            break;
        case Playing:
            this->updatePlaying();
            this->renderPlaying();
            while(this->window->pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    this->window->close();
                if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    this->gameState = StartingMenu;
                    this->window->setTitle("Medieval Adventure! - Play in Fullscreen");
                }
            }
            break;
        case FinalScreenMenu:
            this->updateFinalScreen();
            this->renderFinalScreen();
            break;
        }
    }
}


    //Updating
void Game::updateMousePos()
{
    this->playerPos.x = player->getBounds().left;
    this->playerPos.y = player->getBounds().top;

    this->mousePosScreen = sf::Mouse::getPosition();
    this->mousePosWindow = sf::Mouse::getPosition(*window);

    this->window->setView(*view);

    this->mousePosView = window->mapPixelToCoords(mousePosWindow);

    if(mousePosView.x >= 0.f)
        this->mousePosGrid.x = mousePosView.x / gridSizeU;
    if(mousePosView.y >= 0.f)
        this->mousePosGrid.y = mousePosView.y / gridSizeU;
    this->window->setView(this->window->getDefaultView());

//    std::stringstream ss;
//    ss << "Screen: " << mousePosScreen.x << " " << mousePosScreen.y << "\n"
//       << "Window: " << mousePosWindow.x << " " << mousePosWindow.y << "\n"
//       << "View:   " << mousePosView.x << " " << mousePosView.y << "\n"
//       << "Grid:   " << mousePosGrid.x << " " << mousePosGrid.y << "\n"
//       << "Player: " << playerPos.x << " " << playerPos.y << "\n";
//    this->texts["MousePos"]->setString(ss.str());
}


void Game::updateEntities()
{
    this->dt = dtClock.restart().asSeconds();
    if(this->dt > 1.0f / 20.0f)
    {
        this->dt = 1.0f / 20.0f;
    }
    this->player->update(this->dt);


    this->viewBounds.left = this->view->getCenter().x - this->view->getSize().x / 2;
    this->viewBounds.top = this->view->getCenter().y - this->view->getSize().y / 2;
    this->viewBounds.width = this->view->getSize().x;
    this->viewBounds.height = this->view->getSize().y;

//    std::cout << "enemies.size: " << this->enemies.size() << "\n";

    if(this->enemies.empty())
    {
        this->gameState = FinalScreenMenu;
        this->win = true;
        // delete this->player;
        // this->tileMap.clear();
        // for(int y = 0; y < this->mapSize.y; ++y)
        // {
        //     this->tileMap[y].clear();
        // }
    }
    for(size_t i = 0; i < this->enemies.size(); ++i)
    {
        if(this->viewBounds.intersects(enemies[i]->getBounds()))
        {
            if(!(enemies[i]->isDead()))
            {
                enemies[i]->update(this->dt);
            }
            else
            {
                this->enemies.erase(this->enemies.begin() + i);
            }
        }
    }
}


void Game::updateView()
{
    this->view->setCenter(this->player->getBounds().left + this->player->getBounds().width / 2,
                          this->player->getBounds().top + this->player->getBounds().height / 2);

    //View not going out of map boundries

    //Left
    if(this->view->getCenter().x - this->window->getSize().x / 2 < 0)
    {
        this->view->setCenter(this->window->getSize().x / 2, this->view->getCenter().y);
    }
    //Right
    else if(this->view->getCenter().x + this->window->getSize().x / 2 > gridSizeF * mapSize.x)
    {
        this->view->setCenter(mapSize.x * gridSizeF - this->window->getSize().x / 2, this->view->getCenter().y);
    }
    //Top
    if(this->view->getCenter().y - this->window->getSize().y / 2 < 0)
    {
        this->view->setCenter(this->view->getCenter().x, this->window->getSize().y / 2);
    }
    //Botoom
    else if(this->view->getCenter().y + this->window->getSize().y / 2 > gridSizeF * mapSize.y)
    {
        this->view->setCenter(this->view->getCenter().x, mapSize.y * gridSizeF - this->window->getSize().y / 2);
    }
}


void Game::updateCollisions()
{
    //Player not going out of map boundries

    //Left
    if(this->player->getBounds().left < 0)
    {
        this->player->setPosition(0, this->player->getBounds().top);
    }
    //Right
    else if(this->player->getBounds().left + this->player->getBounds().width > this->gridSizeF * this->mapSize.x )
    {
        this->player->setPosition(this->gridSizeF * this->mapSize.x - this->player->getBounds().width, this->player->getBounds().top);
    }
    //Top
//    if(this->player->getBounds().top < 0)
//    {
//        this->player->setPosition(this->player->getBounds().left, 0);
//    }
    //Botoom
    else if(this->player->getBounds().top + this->player->getBounds().height > this->gridSizeF * this->mapSize.y)
    {
//        this->player->setPosition(this->player->getBounds().left, this->gridSizeF * this->mapSize.y - this->player->getBounds().height);
        this->player->setPosition(500.f, 500.f);
    }

    //Collisons with tiles

    for(int x = fromX; x < this->toX; ++x)
    {
        for(int y = fromY; y < this->toY; ++y)
        {
            if(tileMap[y][x].getCollison())
            {
                if(tileMap[y][x].GetCollider().CheckCollision(player->GetCollider(), this->playerDirection, 1.0f))
                    this->player->onCollison(this->playerDirection);

                for(auto &enemy : this->enemies)
                {
                    if(tileMap[y][x].GetCollider().CheckCollision(enemy->GetCollider(), this->enemyDirection , 1.0f))
                        enemy->onCollison(this->enemyDirection);
                }
            }
        }
    }
    for(auto &enemy : this->enemies)
    {
        enemy->GetCollider().CheckCollision(this->player->GetCollider(), this->playerDirection , 1.0f);

        // If enemy is touched then player dies so is respawned at (500, 500)
        // if(enemy->GetCollider().CheckCollision(this->player->GetCollider(), this->playerDirection , 1.0f))
        // {
        //    this->player->setPosition(500.f, 500.f);
        // }
    }
}


void Game::updateTileMap()
{
//    do ususniecia potem
    this->tileSelector.setPosition(mousePosGrid.x * this->gridSizeF, mousePosGrid.y * this->gridSizeF);

    fromX = this->view->getCenter().x / gridSizeF - (this->view->getSize().x/gridSizeU/2);
    toX = this->view->getCenter().x / gridSizeF + (this->view->getSize().x/gridSizeU/2) + 1;
    fromY = this->view->getCenter().y / gridSizeF - (this->view->getSize().y/gridSizeU/2);
    toY = this->view->getCenter().y / gridSizeF + (this->view->getSize().y/gridSizeU/2) + 1;

    if(fromX < 0)
        fromX = 0;
    else if(fromX >= mapSize.x)
        fromX = mapSize.x;

    if(toX < 0)
        toX = 0;
    else if(toX >= mapSize.x)
        toX = mapSize.x;

    if(fromY < 0)
        fromY = 0;
    else if(fromY >=mapSize.y)
        fromY = mapSize.y;

    if(toY < 0)
        toY = 0;
    else if(toY >=mapSize.y)
        toY = mapSize.y;
}


void Game::updateCombat()
{
    if(this->player->isAttacking())
    {
        for(auto &enemy : this->enemies)
        {
            if(enemy->getBounds().intersects(this->player->getAttackBounds()))
            {
                enemy->die();
            }
        }
    }
    for(auto &enemy : this->enemies)
    {
        if(enemy->isAttacking() && this->player->getBounds().intersects(enemy->getAttackBounds()))
        {
            this->player->setPosition(500.0f, 500.0f);
        }
    }
}


    //Whole state updating
void Game::updateMenu()
{
    this->updateMousePos();

    if(this->texts["Start"].getGlobalBounds().contains(mousePosWindow.x, mousePosWindow.y))
    {
        this->texts["Start"].setFillColor(sf::Color::Blue);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            this->gameState = Playing;
            this->window->setTitle("Medieval Adventure! - Esc to go back to the main menu");
        }
    }
    else if(this->texts["Options"].getGlobalBounds().contains(mousePosWindow.x, mousePosWindow.y))
    {
        this->texts["Options"].setFillColor(sf::Color::Blue);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            this->texts["Options"].setString("Coming Soon =]");
            this->texts["Options"].setPosition(window->getSize().x / 2 - this->texts["Options"].getGlobalBounds().width / 2,
            window->getSize().y / 2);
        }
    }
    else if(this->texts["Quit"].getGlobalBounds().contains(mousePosWindow.x, mousePosWindow.y))
    {
        this->texts["Quit"].setFillColor(sf::Color::Blue);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            this->window->close();
        }
    }
    else
    {
        this->texts["Start"].setFillColor(sf::Color::White);
        this->texts["Options"].setFillColor(sf::Color::White);
        this->texts["Quit"].setFillColor(sf::Color::White);
    }
}


void Game::updatePlaying()
{
    this->updateMousePos();
    this->updateTileMap();
    this->updateEntities();
    this->updateCollisions();
    this->updateCombat();
    this->updateView();
}


void Game::updateFinalScreen()
{
    this->updateMousePos();

    if(this->texts["Play again"].getGlobalBounds().contains(mousePosWindow.x, mousePosWindow.y))
    {
        this->texts["Play again"].setFillColor(sf::Color::Blue);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            this->gameState = Playing;
            this->win = false;

            // this->initTileMap(); W celu wgrania mapy i przeciwnikow na nowo, ale jest bug ktory sprawaia ze kazdy tile jest 10 razy mniejszy niz powinien
            
            this->window->setTitle("Medieval Adventure! - Esc to go back to menu");
        }

    }
    else if(this->texts["Options"].getGlobalBounds().contains(mousePosWindow.x, mousePosWindow.y))
    {
        this->texts["Options"].setFillColor(sf::Color::Blue);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            std::cout << "Options final screen" << "\n";
        }
    }
    else if(this->texts["Quit"].getGlobalBounds().contains(mousePosWindow.x, mousePosWindow.y))
    {
        this->texts["Quit"].setFillColor(sf::Color::Blue);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            this->window->close();
    }
    else
    {
        this->texts["Play again"].setFillColor(sf::Color::White);
        this->texts["Options"].setFillColor(sf::Color::White);
        this->texts["Quit"].setFillColor(sf::Color::White);
    }
}


    //Rendering
void Game::renderTileMap()
{
    for(int x = fromX; x < this->toX; ++x)
    {
        for(int y = fromY; y < this->toY; ++y)
        {
            tileMap[y][x].renderTile(this->window);
        }
    }
}


void Game::renderMenu()
{
    this->window->clear();

    this->window->draw(this->background);
    this->window->draw(texts["Title"]);
    this->window->draw(texts["Start"]);
    this->window->draw(texts["Options"]);
    this->window->draw(texts["Quit"]);

    this->window->display();
}


void Game::renderPlaying()
{
    this->window->clear();
    this->window->draw(this->background);

    //render things moving in view
    this->window->setView(*view);
    this->player->render(this->window);
    this->renderTileMap();
//    this->window->draw(tileSelector); //TODO usunac potem

    for(auto &enemy : enemies)
    {
        if(this->viewBounds.intersects(enemy->getBounds()))
        {
            enemy->render(this->window);
        }
    }

    //render GUI stuff
    this->window->setView(this->window->getDefaultView());
//    this->window->draw(*texts["MousePos"]);


    this->window->display();
}


void Game::renderFinalScreen()
{
    this->window->clear();

    this->window->draw(background);

    if(this->win)
    {
        window->draw(texts["Congratulations"]);
        window->draw(texts["You win"]);
        window->draw(texts["Play again"]);
        window->draw(texts["Options"]);
        window->draw(texts["Quit"]);
    }
    else
    {
        window->draw(texts["Game over"]);
        window->draw(texts["Play again"]);
        window->draw(texts["Options"]);
        window->draw(texts["Quit"]);
    }

    this->window->display();
}
