#include "Game.h"


int main()
{
    Game game;

    game.run();

    return 0;

//    Destruktor obiektu game wywola sie sam po zakonczeniu maina dlatego,
//    ze jesli run skonczy sie wywolywac to znaczy ze gra powinna byc wylaczona.
}
