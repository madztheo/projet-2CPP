#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <thread>
#include "game.h"
#include "tetriminos.h"
#include <mutex>

using namespace std;

int gameBoard[20][10];


int main(){
    Game *game = new Game();
    game->startGame();

    return 0;
}

