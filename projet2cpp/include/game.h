#include "tetriminos.h"
#ifndef DEF_GAME_H
#define DEF_GAME_H

class Game
{
private:
    int score = 0;
    int level = 1;
    int gameBoard[20][10];
    int stepInMilliseconds = 1000;
    Tetriminos* currentTetriminos;
    Tetriminos* nextTetriminos;

    void emptyGameBoard();
    bool canTetriminosMove();

    std::string getLine(int index);
    std::string getBorder();
    void showTetris();
    void detectInput();
    int getRandomNb(int mini, int maxi);
    Tetriminos* getRandomTetriminos();
    Tetriminos* getATetriminos(int nb);
    void integrateATetriminos(Tetriminos* tetriminos);

    std::chrono::milliseconds getCurrentMilliseconds();
    void moveTetriminos();
public:
    Game();
    void startGame();
    void addPoints(int points);
};

#endif // DEF_GAME_H
