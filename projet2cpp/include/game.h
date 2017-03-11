#ifndef DEF_GAME_H
#define DEF_GAME_H

class Game
{
private:
    int score = 0;
    int level = 1;
    int gameBoard[20][10];

    std::string getLine(int index);
    std::string getBorder();
    void showTetris();
    void detectInput();
public:
    Game();
    void startGame();
    void addPoints(int points);
};

#endif // DEF_GAME_H
