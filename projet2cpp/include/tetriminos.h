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
    void emptyArray();
public:
    Tetriminos() {};
    void virtual rotateIt();
    void show();
    ~Tetriminos() {};
};


class TetriminosI : public Tetriminos
{
public:
    TetriminosI();
    void rotateIt();
    ~TetriminosI(){};
};

#endif // DEF_TETRIMINOS_H
