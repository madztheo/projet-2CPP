#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <thread>
#include "game.h"

using namespace std;

Game::Game()
{
    for(int i = 0; i < 20; i++){
        for(int j =0; j < 10; j++){
            gameBoard[i][j] = 0;
        }
    }
}


void Game::addPoints(int points)
{
    score += points;
    level = floor(score / 100);
}


void Game::startGame()
{
    showTetris();
    detectInput();
}

void Game::detectInput()
{
    int posX = 7;
    int posY = 0;

    HANDLE hIn; //The handler for the input
    HANDLE hOut; //The handler for the output


    INPUT_RECORD InRec;
    DWORD NumRead; //The number of characters read

    hIn = GetStdHandle(STD_INPUT_HANDLE); //We get the standard input into our handler
    hOut = GetStdHandle(STD_OUTPUT_HANDLE); //We get the standard output into our handler


    while (TRUE)
    {
        ReadConsoleInput(hIn,
                         &InRec,
                         1,
                         &NumRead);

        switch (InRec.EventType)
        {
            case KEY_EVENT:
            if (InRec.Event.KeyEvent.uChar.AsciiChar == 'q' || InRec.Event.KeyEvent.wVirtualKeyCode == 37)
            {
                gameBoard[posY][posX] = 0;
                gameBoard[posY][--posX] = 1;
                showTetris();
            }
            else if(InRec.Event.KeyEvent.uChar.AsciiChar == 'd' || InRec.Event.KeyEvent.wVirtualKeyCode == 39)
            {
                gameBoard[posY][posX] = 0;
                gameBoard[posY][++posX] = 1;
                showTetris();
            }
            else if(InRec.Event.KeyEvent.uChar.AsciiChar == 'z' || InRec.Event.KeyEvent.wVirtualKeyCode == 38)
            {
                gameBoard[posY][posX] = 0;
                gameBoard[--posY][posX] = 1;
                showTetris();
            }
            else if(InRec.Event.KeyEvent.uChar.AsciiChar == 's' || InRec.Event.KeyEvent.wVirtualKeyCode == 40)
            {
                gameBoard[posY][posX] = 0;
                gameBoard[++posY][posX] = 1;
                showTetris();
            }
            break;
        }
    }

}

void Game::showTetris()
{
    system("cls"); //Clear the console
    cout << getBorder() << endl;
    for(int i = 0; i < 20; i++)
    {
        string line = getLine(i);
        if(i != 1 && i != 2 && i != 4 && i != 6)
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
        else if(i == 6)
        {
           cout << "||" << line << "||" << "     ";
            cout << "[][][][]" << endl;
        }

    }
    cout << getBorder() << endl;
    cout << endl;
}

string Game::getLine(int index)
{
    string line = "";
    for(int j = 0; j < 10; j++)
    {
        if(gameBoard[index][j] == 0){
            line += "  ";
        } else if(gameBoard[index][j] == 1){
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

