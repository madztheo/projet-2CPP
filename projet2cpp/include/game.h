#include "tetriminos.h"
#ifndef DEF_GAME_H
#define DEF_GAME_H

class Game
{
private:
    int score = 0;
    int level = 0;
    int gameBoard[20][10];
    int stepInMilliseconds = 1000;
    Tetriminos* currentTetriminos;
    Tetriminos* nextTetriminos;
    bool isGameOver = false;
    bool help = false;
    bool isBoostOn = false;


    void emptyGameBoard();
    bool canTetriminosMove();
    bool canTetriminosGoLeft();
    bool canTetriminosGoRight();
    bool canTetriminosRotate();

    int getProperTimeSpan();

    std::string getLine(int index);
    std::string getBorder();
    void showTetris();
    void detectInput();
    int getRandomNb(int mini, int maxi);
    Tetriminos* getRandomTetriminos();
    Tetriminos* getATetriminos(int nb);
    void integrateATetriminos(Tetriminos* tetriminos, bool hasReachTheEnd);
    void checkForLineCompletion();
    void checkForGameStatus();
    void addPoints(int points);

    std::chrono::milliseconds getCurrentMilliseconds();
    void moveTetriminos();
public:
    Game();
    void startGame();

};


#endif // DEF_GAME_H
