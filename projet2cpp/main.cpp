#include <iostream>
#include <cmath>
#include <stdlib.h>
using namespace std;

void showTetris(int score, int level);
string getBlank();
string getBorder();

int main(){
    showTetris(0,0);
    showTetris(1234, 2);
    return 0;
}

void showTetris(int score, int level)
{
    system("cls");
    cout << getBorder() << endl;
    for(int i = 0; i < 20; i++)
    {
        string blank = getBlank();
        if(i != 1 && i != 2 && i != 4 && i != 6)
        {
            cout << "||" << blank << "||" << endl;
        }
        else if(i == 1)
        {
            cout << "||" << blank << "||" << "   ";
            cout << "Score : " << score << endl;
        }
        else if(i == 2)
        {
            cout << "||" << blank << "||" << "   ";
            cout << "Level : " << level << endl;
        }
        else if(i == 4)
        {
            cout << "||" << blank << "||" << "   ";
            cout << "Next : " << endl;
        }
        else if(i == 6)
        {
           cout << "||" << blank << "||" << "     ";
            cout << "[][][][]" << endl;
        }

    }
    cout << getBorder() << endl;
    cout << endl;
}

string getBlank()
{
    string blank = "";
    for(int j = 0; j < 17; j++)
    {
        blank += " ";
    }
    return blank;
}

string getBorder()
{
    string border = " ";
    for(int i = 0; i < 19; i++)
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

