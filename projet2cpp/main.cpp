#include <thread>
#include "game.h"

using namespace std;

/*
/!\ Important note : This project rely on Windows specific libraries provided by Microsoft.
Therefore, this program will not work to its full extent on other platforms than Windows.
As there were no mention of a multi-platform support in the instructions, it has then be made for Windows only,
and not for neither Linux nor Mac OS, considering that platform specific code has been expressly needed.
*/


int main(){
    //Initialize the game
    Game *game = new Game();
    //Start the game
    game->startGame();

    return 0;
}

