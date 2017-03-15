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

//The lock use to prevent collision for the function showTetris called in both threads
mutex showLock;

Game::Game()
{
    //We seed our random number generator
    srand (time(NULL));

    //We make sure our board is full of zeros
    emptyGameBoard();

    //We get the first tetriminos, the current and the next one
    currentTetriminos = getRandomTetriminos();
    currentTetriminos->setOffsetY(-2);
    nextTetriminos = getRandomTetriminos();
    nextTetriminos->setOffsetY(-2);
}

//Empty the board of every 1s but not of its 2s
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

//Integrate the tetriminos in the board
void Game::integrateATetriminos(Tetriminos* tetriminos, bool hasReachTheEnd = false)
{

    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j <  5; j++)
        {
            //We make sure we're not making any overflow
            if(i+tetriminos->getOffsetY() < 20 && j+tetriminos->getOffsetX() < 10)
            {
                //If at the index we're looking at, there is a 2 or if for the tetriminos there's 0 we can just skip it
                if(gameBoard[i+tetriminos->getOffsetY()][j+tetriminos->getOffsetX()] == 2
                   || tetriminos->getValueFromBoard(j, i) == 0){
                    continue;
                }

                //Not at the bottom yet, so we just put the value of the tetriminos into the board
                if(!hasReachTheEnd)
                {
                    gameBoard[i+tetriminos->getOffsetY()][j+tetriminos->getOffsetX()] = tetriminos->getValueFromBoard(j, i);
                }
                //We reached the end, so we put a 2 as it was previously a 1 there
                else
                {
                    gameBoard[i+tetriminos->getOffsetY()][j+tetriminos->getOffsetX()] = 2;
                }
            }
        }
    }
}

//Get if the tetriminos can continue to move towards the bottom of the board
bool Game::canTetriminosMove()
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j <  5; j++)
        {
            if(i+currentTetriminos->getOffsetY() == 19)
            {
                //In this case we've reached the end and the value at this location from the tetriminos is a 1,
                //so it can't move further.
                if(currentTetriminos->getValueFromBoard(j, i) == 1)
                {
                    return false;
                }
            }
            //There's already a tetriminos right below blocking the way, so it can't move anymore
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

//Get if the tetriminos rotate
bool Game::canTetriminosRotate()
{
    //The tip of the tetriminos on the left will be the tip on its top after the rotation
    if(currentTetriminos->getCurrentRotation() == 0)
    {
        //2 is the index of the rotation's pivot
        int gap = 2 - currentTetriminos->getFurthestIndexToTheLeft();
        if(currentTetriminos->getOffsetY() + 2 - gap >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    //The tip of the tetriminos on the top will be the tip on its right after the rotation
    else if(currentTetriminos->getCurrentRotation() == 90)
    {
        int gap = 2 - currentTetriminos->getFurthestIndexToTheTop();
        if(currentTetriminos->getOffsetX() + 2 + gap <= 9)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    //The tip of the tetriminos on the right will be the tip on its bottom after the rotation
    else if(currentTetriminos->getCurrentRotation() == 180)
    {
        int gap = currentTetriminos->getFurthestIndexToTheRight() - 2;
        if(currentTetriminos->getOffsetY() + 2 + gap <= 19)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    //The tip of the tetriminos on the bottom will be the tip on its left after the rotation
    else if(currentTetriminos->getCurrentRotation() == 270)
    {
        int gap = currentTetriminos->getFurthestIndexToTheBottom() - 2;
        if(currentTetriminos->getOffsetX() + 2 - gap >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

//Add points to the score
void Game::addPoints(int points)
{
    //We add the points to the score
    score += points;
    //We set the level according to the score, 1 for every 100 points
    level = floor(score / 100);
    //We also update the speed of the tetriminos according to the level
    stepInMilliseconds = 1000 / (level + 1);
}

//Put the game into motion
void Game::startGame()
{
    //Show the tetris
    showTetris();

    //We start two threads, one to detect the input and one to pull the tetriminos to the bottom
    //We pass a pointer to the current object as the function passed is a method of the current object
    thread inputThread(detectInput, this);
    thread timerThread(moveTetriminos, this);

    //We make sure that the threads are stopped when the main thread is
    inputThread.join();
    timerThread.join();
}

//Detect the user's input
void Game::detectInput()
{
    HANDLE hIn; //The handler for the input


    INPUT_RECORD InRec; //Storing the info about the event
    DWORD NumRead; //The number of characters read (not useful in our case but a required parameter of the function below)

    hIn = GetStdHandle(STD_INPUT_HANDLE); //We get the standard input into our handler

    milliseconds lastInputTime = getCurrentMilliseconds(); //We get the current time in milliseconds since the UNIX epoch

    while (!isGameOver)
    {

        //We listen to the input constantly
        ReadConsoleInput(hIn,&InRec,1,&NumRead);


        //We detect if the input event is key input
        if(InRec.EventType == KEY_EVENT){

            //We limit the time interval between two inputs to make it more user-friendly
            if(getCurrentMilliseconds().count() - lastInputTime.count() < 200)
            {
                continue;
            }


            //We detect which key has been pressed

            //Left arrow or q (for AZERTY keyboards) to move on the left
            if (InRec.Event.KeyEvent.uChar.AsciiChar == 'q' || InRec.Event.KeyEvent.wVirtualKeyCode == 37)
            {
                if(canTetriminosGoLeft())
                {
                    lastInputTime = getCurrentMilliseconds();
                    currentTetriminos->setOffsetX(currentTetriminos->getOffsetX() - 1);
                    showTetris();
                }
            }
            //Right arrow or d (for AZERTY keyboards) to move on the right
            else if(InRec.Event.KeyEvent.uChar.AsciiChar == 'd' || InRec.Event.KeyEvent.wVirtualKeyCode == 39)
            {
                if(canTetriminosGoRight())
                {
                    lastInputTime = getCurrentMilliseconds();
                    currentTetriminos->setOffsetX(currentTetriminos->getOffsetX() + 1);
                    showTetris();
                }
            }
            //Down arrow or s (for AZERTY keyboards) for manual acceleration
            else if(InRec.Event.KeyEvent.uChar.AsciiChar == 's' || InRec.Event.KeyEvent.wVirtualKeyCode == 40)
            {
                lastInputTime = getCurrentMilliseconds();
                isBoostOn = true;
            }
            //H to toggle the help
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
            //Space bar for rotation
            else if(InRec.Event.KeyEvent.wVirtualKeyCode == 32) //Space bar
            {
                lastInputTime = getCurrentMilliseconds();
                if(canTetriminosRotate())
                {
                    currentTetriminos->rotateIt();
                    showTetris();
                }

            }
        }
    }
}

//Test if the current tetriminos can go on its left
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
            else if(currentTetriminos->getOffsetX()+currentTetriminos->getFurthestIndexToTheLeft() <= 0)
            {
                return false;
            }
        }
    }
    return true;
}


//Test if the current tetriminos can go on its right
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
            else if(currentTetriminos->getOffsetX()+currentTetriminos->getFurthestIndexToTheRight() >= 9)
            {
                return false;
            }
        }
    }
    return true;
}

//Show the tetris in the console
void Game::showTetris()
{
    //The lock prevents the two threads of calling this method at the same time
    showLock.lock();
    system("cls"); //Clear the console
    emptyGameBoard(); //Make sure to work on a clean board
    integrateATetriminos(currentTetriminos); //Reintegrate the current tetriminos at its right location
    cout << getBorder() << endl; //Draw the top border
    //Draw the rest of the board, which is 20 lines high
    for(int i = 0; i < 20; i++)
    {
        string line = getLine(i); //We get the line which contains the tetriminos

        if(i == 1)
        {
            //First we get the line
            cout << "||" << line << "||" << "   ";
            //Then just a few spaces after we show the score on the side
            cout << "Score : " << score << endl;
        }
        else if(i == 2)
        {
            //The line
            cout << "||" << line << "||" << "   ";
            //...the we show the level
            cout << "Level : " << level << endl;
        }
        else if(i == 4)
        {
            //The line
            cout << "||" << line << "||" << "   ";
            //...then label next to indicate the next tetriminos which is going to appear
            cout << "Next : " << endl;
        }
        else if(i >= 5 && i <= 9)
        {
            //The line
            cout << "||" << line << "||" << "     ";
            //And we go through the next tetriminos to show it on the side of the board
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
        //After we scatter the help on several lines
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
            cout << "They can only be rotated or moved laterally." << endl;
        }
        else if(i == 16 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "The game's goal is to fit the tetriminos in the" << endl;
        }
        else if(i == 17 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "board as tight as possible so as to fill the rows." << endl;
        }
        else if(i == 18 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "Lines will then disappear once they are full." << endl;
        }
        else if(i == 19 && help == true){
            cout << "||" << line << "||" << "   ";
            cout << "The game ends when a piece hit the top of the board." << endl;
        }
        //Then for the rest just a normal line
        else
        {
            cout << "||" << line << "||" << endl;
        }

    }
    cout << getBorder() << endl; //We show the bottom border
    cout << endl;
    showLock.unlock(); //We unlock our lock as we're leaving the function
}


//Get the line to show from the board
string Game::getLine(int index)
{
    string line = "";
    for(int j = 0; j < 10; j++)
    {
        //If it's 0, there's nothing, so we show a double space (because the brackets are two characters)
        if(gameBoard[index][j] == 0){
            line += "  ";
        }
        //If it's 1 or 2, we show the brackets as it matches with a tetriminos.
        //1 indicate a tetriminos in movement and 2, a tetriminos that can no longer move, hence set to dwell in the board
        //at the same position until the end of the game.
        else if(gameBoard[index][j] == 1 || gameBoard[index][j] == 2){
            line += "[]";
        }
    }
    return line;
}


//Get the border of of the board
string Game::getBorder()
{
    string border = " ";
    for(int i = 0; i < 22; i++)
    {
        //We alternate dashes and underscores
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

//Get the interval of time between each downwards movement of the tetriminos
int Game::getProperTimeSpan()
{
    return isBoostOn ? stepInMilliseconds / 4 : stepInMilliseconds;
}

//Move the tetriminos towards the bottom of the board
void Game::moveTetriminos()
{
    //Get the current time in milliseconds to set a start for our counter
    milliseconds startInMs = getCurrentMilliseconds();

    //To known if a step has been reached
    bool hasReachedTheStep = false;
    while(!isGameOver){

        //Get the proper interval
        int timepsan = getProperTimeSpan();
        //If the modulus of the interval between the current number of milliseconds and the start in milliseconds by the
        //the timepsan previously gotten is equal to 0, we then completed a cycle
        //We also checked that this step has not already been reached
        if((getCurrentMilliseconds().count() - startInMs.count()) % timepsan == 0 && !hasReachedTheStep)
        {
            //If the tetriminos can move downwards
            if(canTetriminosMove())
            {
                //We add one to the vertical offset of the tetriminos to move it one row below
                currentTetriminos->setOffsetY(currentTetriminos->getOffsetY() + 1);
            }
            //The tetriminos can't move
            else
            {
                //The tetriminos journey is over, he's gonna settle for a while, so we act accordingly
                //Thus, we integrate it into prosperity by passing true to the second parameter which will
                //convert every 1s associated with that tetriminos to 2s
                integrateATetriminos(currentTetriminos, true);
                //We check if a row has been completed
                checkForLineCompletion();
                //We check whether the game is over or not
                checkForGameStatus();
                //We make the current tetriminos points to the next tetriminos
                currentTetriminos = nextTetriminos;
                //We set an offset of -2 to make it start on the first row
                currentTetriminos->setOffsetY(-2);
                //We then choose a new random tetriminos for the next one to come
                nextTetriminos = getRandomTetriminos();
                //We set an offset of -2 to make it start on the first row
                nextTetriminos->setOffsetY(-2);
            }
            //We show the board
            showTetris();
            //The step has been reached, so we make sure to make that known for forthcoming checks
            hasReachedTheStep = true;
            //We make sure that boost is off, to deactivate if the user has released, after pressing, the down arrow or 's' key
            isBoostOn = false;
        }
        //When the we go on for another cycle, we make sure to set the boolean, indicating if we've reached or not, to false
        else if((getCurrentMilliseconds().count() - startInMs.count()) % timepsan != 0 && hasReachedTheStep)
        {
            hasReachedTheStep = false;
        }
    }
    //When the game is over, the previous loop is interrupted and we then clear everything and show game over
    system("cls");
    cout << "Game over" << endl;
    cout << "Type any character and then enter to close the windows..." << endl;
    //We wait for user input to make avoid the window to be closed too promptly
    char c;
    cin >> &c;
}


//Get the current time in milliseconds since the UNIX epoch
std::chrono::milliseconds Game::getCurrentMilliseconds()
{
    return duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
}

//Get a random number between a lower and upper limit
int Game::getRandomNb(int mini, int maxi)
{
    return rand() % maxi + mini;
}

//Get a tetriminos according to its associated number
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

//Get a random tetriminos
Tetriminos* Game::getRandomTetriminos()
{
    int number = getRandomNb(0, 7);
    return getATetriminos(number);
}

//Check if a line is full of 2s, hence complete
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
            if(total == 10) //10 being a size of a line
            {
                linenb = i;
                break;
            }
        }
        //At least one line is full
        if(linenb >= 0)
        {
            canContinue = true; //We set to true to do another loop to look for other possible full lines
            //We then delete the row and make everything on top of it go down of one row
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
            //We then add 10 points to the score
            addPoints(10);
        }
        else
        {
            //No full line has been retrieved, we notify that loop must stop
            canContinue = false;
        }
    }

}


//Check whether the is over or not
void Game::checkForGameStatus()
{
    for(int i = 0; i < 10; i++)
    {
        //There's a 2 on the first row, it's over
        if(gameBoard[0][i] == 2)
        {
            isGameOver = true;
        }
    }
}
