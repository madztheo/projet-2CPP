#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include "tetriminos.h"

using namespace std;

void Tetriminos::emptyArray()
{
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
           board[i][j] = 0;
        }
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


void Tetriminos::rotateIt()
{
    rotation = (rotation + 90) % 360;
    emptyArray();
}

int Tetriminos::getFurthestIndexOnTheRight()
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

int Tetriminos::getFurthestIndexOnTheLeft()
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

int Tetriminos::getFurthestIndexOnTheBottom()
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
    Tetriminos::rotateIt();
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


