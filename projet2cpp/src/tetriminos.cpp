#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include "tetriminos.h"

using namespace std;

TetriminosI::TetriminosI() : Tetriminos()
{
    board[2][0] = 1;
    board[2][1] = 1;
    board[2][2] = 1;
    board[2][3] = 1;
}

void Tetriminos::emptyArray()
{
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
           board[i][j] = 0;
        }
    }
}


void Tetriminos::rotateIt()
{
    rotation = (rotation + 90) % 360;
}

void TetriminosI::rotateIt()
{
    Tetriminos::rotateIt();
    if(rotation == 0)
    {
        emptyArray();
        board[2][0] = 1;
        board[2][1] = 1;
        board[2][2] = 1;
        board[2][3] = 1;
        /*board[5][5] =
        {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {1,1,1,1,0},
            {0,0,0,0,0},
            {0,0,0,0,0},
        };*/
    }
    else if(rotation == 90)
    {
        emptyArray();
        board[0][2] = 1;
        board[1][2] = 1;
        board[2][2] = 1;
        board[3][2] = 1;
        /*board[5][5] =
        {
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,0,0,0},
        };*/
    }
    else if(rotation == 180)
    {
        emptyArray();
        board[2][1] = 1;
        board[2][2] = 1;
        board[2][3] = 1;
        board[2][4] = 1;
        /*board[5][5] =
        {
            {0,0,0,0,0},
            {0,0,0,0,0},
            {0,1,1,1,1},
            {0,0,0,0,0},
            {0,0,0,0,0},
        };*/
    }
    else if(rotation == 270)
    {
        emptyArray();
        board[1][2] = 1;
        board[2][2] = 1;
        board[3][2] = 1;
        board[4][2] = 1;
        /*board[5][5] =
        {
            {0,0,0,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
            {0,0,1,0,0},
        };*/
    }
}

void Tetriminos::show()
{
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}
