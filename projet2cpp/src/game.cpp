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
    score += points;
    level = floor(score / 100);
    stepInMilliseconds = 1000 - level * 50;
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

        switch (InRec.EventType)
        {
            case KEY_EVENT:
            //We limit the time interval between two inputs to make it more user-friendly
            if(getCurrentMilliseconds().count() - lastInputTime.count() < 100)
            {
                continue;
            }
            if (InRec.Event.KeyEvent.uChar.AsciiChar == 'q' || InRec.Event.KeyEvent.wVirtualKeyCode == 37)
            {
                lastInputTime = getCurrentMilliseconds();
                currentTetriminos->setOffsetX(currentTetriminos->getOffsetX() - 1);
                showTetris();
            }
            else if(InRec.Event.KeyEvent.uChar.AsciiChar == 'd' || InRec.Event.KeyEvent.wVirtualKeyCode == 39)
            {
                lastInputTime = getCurrentMilliseconds();
                currentTetriminos->setOffsetX(currentTetriminos->getOffsetX() + 1);
                showTetris();
            }
            else if(InRec.Event.KeyEvent.wVirtualKeyCode == 32) //Space bar
            {
                lastInputTime = getCurrentMilliseconds();
                currentTetriminos->rotateIt();
                showTetris();
            }
            break;
        }
    }
    cout << "Game over" << endl;
    char c;
    cin >> &c;
}

void Game::showTetris()
{
    //The lock allows to prevent the two threads of calling this method at the same time
    showLock.lock();
    system("cls"); //Clear the console
    emptyGameBoard();
    integrateATetriminos(currentTetriminos);
    cout << getBorder() << endl;
    for(int i = 0; i < 20; i++)
    {
        string line = getLine(i);
        if(i != 1 && i != 2 && i != 4 && (i < 5 || i > 9))
        {
            cout << "||" << line << "||" << endl;
        }
        else if(i == 1)
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

void Game::moveTetriminos()
{

    milliseconds startInMs = getCurrentMilliseconds();

    bool hasReachedTheStep = false;
    while(!isGameOver){
        if((getCurrentMilliseconds().count() - startInMs.count()) % stepInMilliseconds == 0 && !hasReachedTheStep)
        {
            if(canTetriminosMove())
            {
                currentTetriminos->setOffsetY(currentTetriminos->getOffsetY() + 1);
            }
            else
            {
                integrateATetriminos(currentTetriminos, true);
                checkForLineCompletion();
                currentTetriminos = nextTetriminos;
                nextTetriminos = getRandomTetriminos();
            }
            showTetris();
            hasReachedTheStep = true;
        }
        else if((getCurrentMilliseconds().count() - startInMs.count()) % stepInMilliseconds != 0 && hasReachedTheStep)
        {
            hasReachedTheStep = false;
        }
    }
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
        if(gameBoard[0][i] == 2)
        {
            isGameOver = true;
        }
    }
}
