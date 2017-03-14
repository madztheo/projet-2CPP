#include <iostream>
#include <chrono>
#include <ctime>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <thread>
#include "game.h"
#include <mutex>
#include <string>

using namespace std;
using namespace std::chrono;

mutex showLock;

Game::Game()
{
    srand (time(NULL));

    emptyGameBoard();

    currentTetriminos = getRandomTetriminos();
    nextTetriminos = getRandomTetriminos();
}

void Game::emptyGameBoard()
{
    for(int i = 0; i < 20; i++){
        for(int j =0; j < 10; j++){
            if(gameBoard[i][j] != 2)
            {
                gameBoard[i][j] = 0;
            }
        }
    }
}

void Game::integrateATetriminos(Tetriminos* tetriminos, bool hasReachTheEnd = false)
{

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j <  5; j++)
        {
            if(i+tetriminos->getOffsetY() < 20 && j+tetriminos->getOffsetX() < 10)
            {
                if(gameBoard[i+tetriminos->getOffsetY()][j+tetriminos->getOffsetX()] == 2){
                    continue;
                }

                if(!hasReachTheEnd)
                {
                    gameBoard[i+tetriminos->getOffsetY()][j+tetriminos->getOffsetX()] = tetriminos->getValueFromBoard(j, i);
                }
                else
                {
                    int number = tetriminos->getValueFromBoard(j, i);
                    gameBoard[i+tetriminos->getOffsetY()][j+tetriminos->getOffsetX()] = number == 0 ? 0 : 2;
                }
            }
        }
    }
}

bool Game::canTetriminosMove()
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j <  5; j++)
        {
            if(i+currentTetriminos->getOffsetY() == 19)
            {
                if(currentTetriminos->getValueFromBoard(j, i) == 1)
                {
                    return false;
                }
            }
            else if(i+currentTetriminos->getOffsetY() < 19
                    && gameBoard[i+currentTetriminos->getOffsetY()+1][j+currentTetriminos->getOffsetX()] == 2
                    && currentTetriminos->getValueFromBoard(j, i) == 1)
            {
                return false;
            }
        }
    }

    return true;
}

void Game::addPoints(int points)
{
    showLock.lock();
    score += points;
    level = floor(score / 100);
    stepInMilliseconds = 1000 / (level + 1);
    showLock.unlock();
}


void Game::startGame()
{
    showTetris();

    //A reference to the current object must be passed as we want to execute a method of this one
    thread inputThread(detectInput, this);
    thread timerThread(moveTetriminos, this);

    inputThread.join();
    timerThread.join();
}

void Game::detectInput()
{
    HANDLE hIn; //The handler for the input
    //HANDLE hOut; //The handler for the output


    INPUT_RECORD InRec;
    DWORD NumRead; //The number of characters read

    hIn = GetStdHandle(STD_INPUT_HANDLE); //We get the standard input into our handler
    //hOut = GetStdHandle(STD_OUTPUT_HANDLE); //We get the standard output into our handler
    milliseconds lastInputTime = getCurrentMilliseconds();

    while (!isGameOver)
    {

        ReadConsoleInput(hIn,
                         &InRec,
                         1,
                         &NumRead);


        if(InRec.EventType == KEY_EVENT){

            //We limit the time interval between two inputs to make it more user-friendly
            if(getCurrentMilliseconds().count() - lastInputTime.count() < 200)
            {
                continue;
            }

            if (InRec.Event.KeyEvent.uChar.AsciiChar == 'q' || InRec.Event.KeyEvent.wVirtualKeyCode == 37)
            {
                if(canTetriminosGoLeft())
                {
                    lastInputTime = getCurrentMilliseconds();
                    currentTetriminos->setOffsetX(currentTetriminos->getOffsetX() - 1);
                    showTetris();
                }
            }
            else if(InRec.Event.KeyEvent.uChar.AsciiChar == 'd' || InRec.Event.KeyEvent.wVirtualKeyCode == 39)
            {
                if(canTetriminosGoRight())
                {
                    lastInputTime = getCurrentMilliseconds();
                    currentTetriminos->setOffsetX(currentTetriminos->getOffsetX() + 1);
                    showTetris();
                }
            }
            else if(InRec.Event.KeyEvent.uChar.AsciiChar == 's' || InRec.Event.KeyEvent.wVirtualKeyCode == 40)
            {
                isBoostOn = true;
            }
            else if((InRec.Event.KeyEvent.uChar.AsciiChar == 'h' || InRec.Event.KeyEvent.uChar.AsciiChar == 'H') && help == false){

                lastInputTime = getCurrentMilliseconds();
                help = true;
                showTetris();
            }
            else if((InRec.Event.KeyEvent.uChar.AsciiChar == 'h' || InRec.Event.KeyEvent.uChar.AsciiChar == 'H') && help == true){
                lastInputTime = getCurrentMilliseconds();
                help = false;
                showTetris();
            }
            else if(InRec.Event.KeyEvent.wVirtualKeyCode == 32) //Space bar
            {
                lastInputTime = getCurrentMilliseconds();
                currentTetriminos->rotateIt();
                showTetris();
            }
        }
    }
}

bool Game::canTetriminosGoLeft()
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j <  5; j++)
        {
            //To avoid overlapping with other tetriminos
            if(i+currentTetriminos->getOffsetY() <= 19 && j+currentTetriminos->getOffsetX() > 0
                    && gameBoard[i+currentTetriminos->getOffsetY()][j+currentTetriminos->getOffsetX()-1] == 2
                    && currentTetriminos->getValueFromBoard(j, i) == 1)
            {
                return false;
            }
            //To prevent the tetriminos from getting out the game board
            else if(currentTetriminos->getOffsetX()+currentTetriminos->getFurthestIndexOnTheLeft() <= 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool Game::canTetriminosGoRight()
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j <  5; j++)
        {
            if(i+currentTetriminos->getOffsetY() <= 19 && j+currentTetriminos->getOffsetX() < 9
                    && gameBoard[i+currentTetriminos->getOffsetY()][j+currentTetriminos->getOffsetX()+1] == 2
                    && currentTetriminos->getValueFromBoard(j, i) == 1)
            {
                return false;
            }
            else if(currentTetriminos->getOffsetX()+currentTetriminos->getFurthestIndexOnTheRight() >= 9)
            {
                return false;
            }
        }
    }
    return true;
}

void Game::showTetris()
{
    //The lock prevents the two threads of calling this method at the same time
    showLock.lock();
    system("cls"); //Clear the console
    emptyGameBoard();
    integrateATetriminos(currentTetriminos);
    cout << getBorder() << endl;
    for(int i = 0; i < 20; i++)
    {
        string line = getLine(i);

        if(i == 1)
        {
            cout << "||" << line << "||" << "   ";
            cout << "Score : " << score << endl;
        }
        else if(i == 2)
        {
            cout << "||" << line << "||" << "   ";
            cout << "Level : " << level << endl;
        }
        else if(i == 4)
        {
            cout << "||" << line << "||" << "   ";
            cout << "Next : " << endl;
        }
        else if(i >= 5 && i <= 9)
        {
            cout << "||" << line << "||" << "     ";
            for(int j = 0; j < 5; j++)
            {
                if(nextTetriminos->getValueFromBoard(j, i-5) == 1)
                {
                    cout << "[]";
                }
                else
                {
                    cout << "  ";
                }
            }
            cout << endl;
        }
        else if(i == 12 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "HELP" << endl;
        }
        else if(i == 14 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "Tetriminos will fall from the top of the screen." << endl;
        }
        else if(i == 15 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "They can only be moved laterally or rotated." << endl;
        }
        else if(i == 16 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "The goal is to fit the tetriminos in the board" << endl;
        }
        else if(i == 17 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "so as to create complete rows. Lines will disappear" << endl;
        }
        else if(i == 18 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "once they're full. The game will end when" << endl;
        }
        else if(i == 19 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "a piece hit the roof." << endl;
        }
        else
        {
            cout << "||" << line << "||" << endl;
        }

    }
    cout << getBorder() << endl;
    cout << endl;
    showLock.unlock();
}

string Game::getLine(int index)
{
    string line = "";
    for(int j = 0; j < 10; j++)
    {
        if(gameBoard[index][j] == 0){
            line += "  ";
        } else if(gameBoard[index][j] == 1 || gameBoard[index][j] == 2){
            line += "[]";
        }
    }
    return line;
}

string Game::getBorder()
{
    string border = " ";
    for(int i = 0; i < 22; i++)
    {
        if(i % 2 == 0)
        {
            border += "-";
        }
        else
        {
            border += "_";
        }
    }
    return border;
}

int Game::getProperTimeSpan()
{
    return isBoostOn ? stepInMilliseconds / 4 : stepInMilliseconds;
}

void Game::moveTetriminos()
{
    milliseconds startInMs = getCurrentMilliseconds();

    bool hasReachedTheStep = false;
    while(!isGameOver){

        int timepsan = getProperTimeSpan();
        if((getCurrentMilliseconds().count() - startInMs.count()) % timepsan == 0 && !hasReachedTheStep)
        {

            if(canTetriminosMove())
            {
                currentTetriminos->setOffsetY(currentTetriminos->getOffsetY() + 1);
            }
            else
            {
                integrateATetriminos(currentTetriminos, true);
                checkForLineCompletion();
                checkForGameStatus();
                currentTetriminos = nextTetriminos;
                nextTetriminos = getRandomTetriminos();
            }
            showTetris();
            hasReachedTheStep = true;
            isBoostOn = false;
        }
        else if((getCurrentMilliseconds().count() - startInMs.count()) % timepsan != 0 && hasReachedTheStep)
        {
            hasReachedTheStep = false;
        }
    }
    system("cls");
    cout << "Game over" << endl;
    char c;
    cin >> &c;
}


std::chrono::milliseconds Game::getCurrentMilliseconds()
{
    return duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
}

int Game::getRandomNb(int mini, int maxi)
{
    return rand() % maxi + mini;
}

Tetriminos* Game::getATetriminos(int nb)
{
    switch(nb)
    {
        case 0:
            return new TetriminosI();

        case 1:
            return new TetriminosJ();

        case 2:
            return new TetriminosL();

        case 3:
            return new TetriminosO();

        case 4:
            return new TetriminosT();

        case 5:
            return new TetriminosS();

        case 6:
            return new TetriminosZ();
    }
    return nullptr;
}

Tetriminos* Game::getRandomTetriminos()
{
    int number = getRandomNb(0, 7);
    return getATetriminos(number);
}

void Game::checkForLineCompletion()
{
    bool canContinue = true;
    while(canContinue)
    {
        int linenb = -1;
        for(int i = 0; i < 20; i++)
        {
            int total = 0;
            for(int j = 0; j < 10; j++)
            {
                if(gameBoard[i][j] == 2)
                {
                    total += 1;
                }
            }
            if(total == 10)
            {
                linenb = i;
                break;
            }
        }
        if(linenb >= 0)
        {
            canContinue = true;
            for(int i = linenb; i >= 0; i--)
            {
                for(int j = 0; j < 10; j++)
                {
                    if(i >  0)
                    {
                        gameBoard[i][j] = gameBoard[i-1][j];
                    }
                    else
                    {
                        gameBoard[i][j] = 0;
                    }

                }
            }
            addPoints(10);
        }
        else
        {
            canContinue = false;
        }
    }

}

void Game::checkForGameStatus()
{
    for(int i = 0; i < 10; i++)
    {
        if(gameBoard[2][i] == 2)
        {
            isGameOver = true;
        }
    }
}
