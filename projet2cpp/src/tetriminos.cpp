#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include "tetriminos.h"

using namespace std;

//Empty the array containing the tetriminos, 1 for something and 0 for nothing
void Tetriminos::emptyArray()
{
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
           board[i][j] = 0;
        }
    }
}

//Show the tetriminos on the screen, was used only for test purposes
void Tetriminos::show()
{
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

//Rotate the tetriminos
void Tetriminos::rotateIt()
{
    //We add 90 degrees to the current rotation of the tetriminos make sure stay within the 360 with a modulus
    rotation = (rotation + 90) % 360;
    //We then empty the board to make the update afterwards in the children implementations of this function
    emptyArray();
}

//We get the index of the 1 which the furthest on the right
int Tetriminos::getFurthestIndexToTheRight()
{
    int index = 0;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(board[i][j] == 1 && j > index)
            {
                index = j;
            }
        }
    }
    return index;
}

//We get the index of the 1 which the furthest on the left
int Tetriminos::getFurthestIndexToTheLeft()
{
    int index = 4;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(board[i][j] == 1 && j < index)
            {
                index = j;
            }
        }
    }
    return index;
}

//We get the index of the 1 which the furthest on the bottom
int Tetriminos::getFurthestIndexToTheBottom()
{
    int index = 0;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(board[i][j] == 1 && i > index)
            {
                index = i;
            }
        }
    }
    return index;
}

//We get the index of the 1 which the furthest on the top
int Tetriminos::getFurthestIndexToTheTop()
{
    int index = 4;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(board[i][j] == 1 && i < index)
            {
                index = i;
            }
        }
    }
    return index;
}


//Tetriminos I
TetriminosI::TetriminosI() : Tetriminos()
{
    board[2][0] = 1;
    board[2][1] = 1;
    board[2][2] = 1;
    board[2][3] = 1;
}

void TetriminosI::rotateIt()
{
    //We call the parent implementation of the function
    Tetriminos::rotateIt();

    //We then make the rotation specific to the child
    if(rotation == 0)
    {
        board[2][0] = 1;
        board[2][1] = 1;
        board[2][2] = 1;
        board[2][3] = 1;

    }
    else if(rotation == 90)
    {
        board[0][2] = 1;
        board[1][2] = 1;
        board[2][2] = 1;
        board[3][2] = 1;
    }
    else if(rotation == 180)
    {
        board[2][1] = 1;
        board[2][2] = 1;
        board[2][3] = 1;
        board[2][4] = 1;
    }
    else if(rotation == 270)
    {
        board[1][2] = 1;
        board[2][2] = 1;
        board[3][2] = 1;
        board[4][2] = 1;
    }
}

//Tetriminos J
TetriminosJ::TetriminosJ() : Tetriminos()
{
    board[2][0] = 1;
    board[2][1] = 1;
    board[2][2] = 1;
    board[3][2] = 1;
}

void TetriminosJ::rotateIt()
{
    Tetriminos::rotateIt();
    if(rotation == 0)
    {
        board[2][0] = 1;
        board[2][1] = 1;
        board[2][2] = 1;
        board[3][2] = 1;
    }
    else if(rotation == 90)
    {
        board[0][2] = 1;
        board[1][2] = 1;
        board[2][2] = 1;
        board[2][1] = 1;
    }
    else if(rotation == 180)
    {
        board[1][2] = 1;
        board[2][2] = 1;
        board[2][3] = 1;
        board[2][4] = 1;
    }
    else if(rotation == 270)
    {
        board[2][2] = 1;
        board[2][3] = 1;
        board[3][2] = 1;
        board[4][2] = 1;
    }
}

//Tetriminos L
TetriminosL::TetriminosL() : Tetriminos()
{
    board[2][0] = 1;
    board[3][0] = 1;
    board[2][1] = 1;
    board[2][2] = 1;
}

void TetriminosL::rotateIt()
{
    Tetriminos::rotateIt();
    if(rotation == 0)
    {
        board[2][0] = 1;
        board[3][0] = 1;
        board[2][1] = 1;
        board[2][2] = 1;
    }
    else if(rotation == 90)
    {
        board[0][1] = 1;
        board[0][2] = 1;
        board[1][2] = 1;
        board[2][2] = 1;
    }
    else if(rotation == 180)
    {
        board[2][2] = 1;
        board[2][3] = 1;
        board[2][4] = 1;
        board[1][4] = 1;
    }
    else if(rotation == 270)
    {
        board[2][2] = 1;
        board[3][2] = 1;
        board[4][2] = 1;
        board[4][3] = 1;
    }
}

//Tetriminos O
TetriminosO::TetriminosO() : Tetriminos()
{
    board[2][0] = 1;
    board[2][1] = 1;
    board[3][0] = 1;
    board[3][1] = 1;
}

void TetriminosO::rotateIt()
{

}


//Tetriminos T
TetriminosT::TetriminosT() : Tetriminos()
{
    board[2][1] = 1;
    board[2][2] = 1;
    board[2][3] = 1;
    board[3][2] = 1;
}

void TetriminosT::rotateIt()
{
    Tetriminos::rotateIt();
    if(rotation == 0)
    {
        board[2][1] = 1;
        board[2][2] = 1;
        board[2][3] = 1;
        board[3][2] = 1;
    }
    else if(rotation == 90)
    {
        board[2][1] = 1;
        board[2][2] = 1;
        board[3][2] = 1;
        board[1][2] = 1;
    }
    else if(rotation == 180)
    {
        board[2][1] = 1;
        board[2][2] = 1;
        board[2][3] = 1;
        board[1][2] = 1;
    }
    else if(rotation == 270)
    {
        board[2][3] = 1;
        board[2][2] = 1;
        board[3][2] = 1;
        board[1][2] = 1;
    }
}

//Tetriminos S
TetriminosS::TetriminosS() : Tetriminos()
{
    board[2][2] = 1;
    board[2][3] = 1;
    board[3][1] = 1;
    board[3][2] = 1;
}

void TetriminosS::rotateIt()
{
    Tetriminos::rotateIt();
    if(rotation == 0)
    {
        board[2][2] = 1;
        board[2][3] = 1;
        board[3][1] = 1;
        board[3][2] = 1;
    }
    else if(rotation == 90)
    {
        board[2][1] = 1;
        board[1][1] = 1;
        board[2][2] = 1;
        board[3][2] = 1;
    }
    else if(rotation == 180)
    {
        board[2][1] = 1;
        board[1][2] = 1;
        board[2][2] = 1;
        board[1][3] = 1;
    }
    else if(rotation == 270)
    {
        board[1][2] = 1;
        board[2][2] = 1;
        board[2][3] = 1;
        board[3][3] = 1;
    }
}

//Tetriminos Z
TetriminosZ::TetriminosZ() : Tetriminos()
{
    board[2][1] = 1;
    board[2][2] = 1;
    board[3][2] = 1;
    board[3][3] = 1;
}

void TetriminosZ::rotateIt()
{
    Tetriminos::rotateIt();
    if(rotation == 0)
    {
        board[2][1] = 1;
        board[2][2] = 1;
        board[3][2] = 1;
        board[3][3] = 1;
    }
    else if(rotation == 90)
    {
        board[2][1] = 1;
        board[2][2] = 1;
        board[3][1] = 1;
        board[1][2] = 1;
    }
    else if(rotation == 180)
    {
        board[1][1] = 1;
        board[1][2] = 1;
        board[2][2] = 1;
        board[2][3] = 1;
    }
    else if(rotation == 270)
    {
        board[2][2] = 1;
        board[2][3] = 1;
        board[1][3] = 1;
        board[3][2] = 1;
    }
}


