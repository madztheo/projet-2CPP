#ifndef DEF_TETRIMINOS_H
#define DEF_TETRIMINOS_H

class Tetriminos
{
protected:
    int board[5][5] =
    {
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    };
    int rotation = 0;
    int offsetX = 0;
    int offsetY = 0;

    void emptyArray();
public:
    Tetriminos() {};
    void virtual rotateIt();
    void show();
    int getOffsetX() { return offsetX; }
    int getOffsetY() { return offsetY; }
    void setOffsetX(int value) { offsetX = value; }
    void setOffsetY(int value) { offsetY = value; }
    int getValueFromBoard(int x, int y) { return board[y][x]; }
    int getFurthestIndexToTheRight();
    int getFurthestIndexToTheLeft();
    int getFurthestIndexToTheBottom();
    int getFurthestIndexToTheTop();
    int getCurrentRotation() { return rotation; }

    ~Tetriminos() {};
};


class TetriminosI : public Tetriminos
{
public:
    TetriminosI();
    void rotateIt();
    ~TetriminosI(){};
};

class TetriminosJ : public Tetriminos
{
public:
    TetriminosJ();
    void rotateIt();
    ~TetriminosJ(){};
};

class TetriminosL : public Tetriminos
{
public:
    TetriminosL();
    void rotateIt();
    ~TetriminosL(){};
};

class TetriminosO : public Tetriminos
{
public:
    TetriminosO();
    void rotateIt();
    ~TetriminosO(){};
};

class TetriminosT : public Tetriminos
{
public:
    TetriminosT();
    void rotateIt();
    ~TetriminosT(){};
};

class TetriminosS : public Tetriminos
{
public:
    TetriminosS();
    void rotateIt();
    ~TetriminosS(){};
};

class TetriminosZ : public Tetriminos
{
public:
    TetriminosZ();
    void rotateIt();
    ~TetriminosZ(){};
};
#endif // DEF_TETRIMINOS_H
