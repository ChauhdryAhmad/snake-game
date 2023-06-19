#include<iostream>
#include<conio.h>
#include<iomanip>
#include<Windows.h>
#include<time.h>
#include<string>

using namespace std;

#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define BROWN 5
#define LIGHTBROWN 6
#define LIGHTGREY 7
#define DARKGREY 8
#define YELLOW 9
#define DARKbROWN 10
#define GREEN 11
#define LIGHTGREEN 12
#define DARKGREEN 13
#define ORANGE 14
#define PURPLE 15

void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}

void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

void SetClr(int clr)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}

enum Direction
{
    Up, Down, Left, Right
};

struct Position
{
    int ri, ci;
};

struct Snake
{
    Position* pos;
    int size;
    int score;
    int clr;
    char sym;
    string pName;
    Direction dir;
    int UpKey, DownKey, LeftKey, RightKey;
};

struct Food
{
    Position fp;
    bool isAlive;
    char sym;
};

   
void T2B(int sri, int  eri, int ci)
{
    for (int i = sri; i <= eri; i++)
    {
        gotoRowCol(i, ci);
        cout << char(-37);
    }
}

void L2R(int sci,int eci, int ri)
{
    for (int i = sci; i <= eci; i++)
    {
        gotoRowCol(ri, i);
        cout << char(-37);
    }
}

void displayboundry(int height, int width)
{
    SetClr(8);

    T2B(0,height, 0);
    T2B(0,height, width);
    L2R(0,width, 0);
    L2R(0,width, height);
}

void displayboundryLvl2(int height, int width)
{
    SetClr(4);

    T2B(0, height, 0);
    T2B(0, height, width);
    L2R(0, width, 0);
    L2R(0, width, height);

    T2B(10, 35, 20);
    L2R(20, 60, 10);
    T2B(10, 35, 60);
    L2R(20, 30, 35);
    L2R(50, 60, 35);
}

void displayboundryLvl3(int height, int width)
{
    SetClr(4);

    T2B(0, height, 0);
    T2B(0, height, width);
    L2R(0, width, 0);
    L2R(0, width, height);

    T2B(10, 15, 15);
    T2B(10, 15, 65);
    T2B(35, 40, 25);
    T2B(40, 45, 40);

    L2R(10, 15, 25);
    L2R(50, 55, 30);
    L2R(60, 65, 40);
    L2R(40, 45, 15);

}

void init(Snake*& S, int nop, int height, int width, string Name[])
{
    S = new Snake[nop];
    int DirKeys[16] = { 72,80,75,77,119,115,97,100,53,50,49,51,121,104,103,106 };
    int clour[4] = { 1,2,3,4 };
    int sk = 0; int lmit = 4;
    for (int i = 0; i < nop; i++)
    {
        for (int j = sk; j < lmit; j += 4)
        {
            S[i].UpKey = DirKeys[j];
            S[i].DownKey = DirKeys[j + 1];
            S[i].LeftKey = DirKeys[j + 2];
            S[i].RightKey = DirKeys[j + 3];
        }
        S[i].clr = clour[i];
        S[i].dir = Right;
        S[i].pName = Name[i];
        S[i].sym = char(-37);
        S[i].size = 3;
        S[i].score = 0;
        S[i].pos = new Position[S[i].size];
        for (int j = 0; j < S[i].size; j++)
        {
            S[i].pos[j].ri = (height / 2) - 2 + i;
            S[i].pos[j].ci = (width / 2) - 2 + i - j;
        }
        sk += 4; lmit += 4;
    }

}

bool isLegalCordinateForBodyAndBoundry(Position p, Snake* S, int nop, int height, int width)
{
    if (p.ri == 0 || p.ri == height || p.ci == 0 || p.ci == width)
        return false;
    for (int i = 0; i < nop; i++)
    {
        for (int j = 0; j < S[i].size; j++)
        {
            if (S[i].pos[j].ri == p.ri && S[i].pos[j].ci == p.ci)
                return false;
        }
    }
    return true;
}

bool isLegalCordinateForLvl2(Position p, Snake* S, int nop, int height, int width)
{
    if (!isLegalCordinateForBodyAndBoundry(p, S, nop, height, width))
        return false;
    for (int i = 0; i <= 25; i++)
    {
        if (p.ri == 10 + i && (p.ci == 20 || p.ci == 60))
            return false;
    }
    for (int i = 0; i <= 40; i++)
    {
        if (p.ri == 10 && p.ci == 20 + i)
            return false;
    }
    for (int i = 0; i <= 10; i++)
    {
        if (p.ri == 35 && p.ci == 20 + i)
            return false;
    }
    for (int i = 0; i <= 10; i++)
    {
        if (p.ri == 35 && p.ci == 50 + i)
            return false;
    }
    return true;
}

bool isLegalCordinateForLvl3(Position p, Snake* S, int nop, int height, int width)
{
    if (!isLegalCordinateForBodyAndBoundry(p, S, nop, height, width))
        return false;
    for (int i = 0; i <= 5; i++)
    {
        if (p.ri == 10 + i && (p.ci == 15 || p.ci == 65))
            return false;
        if (p.ri == i + 35 && p.ci == 25)
            return false;
        if (p.ri == i + 40 && p.ci == 40)
            return false;
    }
    for (int i = 0; i <= 5; i++)
    {
        if (p.ri == 25 && p.ci == i + 10)
            return false;
        if (p.ri == 30 && p.ci == i + 50)
            return false;
        if (p.ri == 40 && p.ci == i + 60)
            return false;
        if (p.ri == 15 && p.ci == i + 40)
            return false;
    }
    return true;
}

void generateFoodForLvl3(Food& F, Snake* S, int nop, int height, int width)
{
    do {
        F.fp.ri = rand() % height;
        F.fp.ci = rand() % width;
    } while (!isLegalCordinateForLvl3(F.fp, S, nop, height, width));
    F.isAlive = true;
    F.sym = '*';
}

void generateFoodForLvl2(Food& F, Snake* S, int nop, int height, int width)
{
    do {
        F.fp.ri = rand() % height;
        F.fp.ci = rand() % width;
    } while (!isLegalCordinateForLvl2(F.fp, S, nop, height, width));
    F.isAlive = true;
    F.sym = '*';
}

void generateFood(Food& F, Snake* S,int nop, int height, int width)
{
    do {
        F.fp.ri = rand() % height;
        F.fp.ci = rand() % width;
    } while (!isLegalCordinateForBodyAndBoundry(F.fp, S,nop, height, width));
    F.isAlive = true;
    F.sym = '*';
}

void displaySnake(Snake* S, int nop)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = 0; j < S[i].size; j++)
        {
            SetClr(S[i].clr);
            gotoRowCol(S[i].pos[j].ri, S[i].pos[j].ci);
            cout << S[i].sym;
        }
    }
}

void displayFood(Food F)
{
    SetClr(2);
    gotoRowCol(F.fp.ri, F.fp.ci);
    cout << F.sym;
}

void eraseSnake(Snake* S, int  nop)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = 0; j < S[i].size; j++)
        {
            SetClr(S[i].clr);
            gotoRowCol(S[i].pos[j].ri, S[i].pos[j].ci);
            cout << ' ';
        }
    }
}

void moveSnakeDuniyaGol(Snake*& S, int nop, int heigth, int width)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = S[i].size - 1; j > 0; j--)
        {
            S[i].pos[j].ri = S[i].pos[j - 1].ri;
            S[i].pos[j].ci = S[i].pos[j - 1].ci;
        }
        switch (S[i].dir)
        {
        case Right:
            S[i].pos[0].ci++;
            if (S[i].pos[0].ci == width)
                S[i].pos[0].ci = 1;
            break;
        case Left:
            S[i].pos[0].ci--;
            if (S[i].pos[0].ci == 0)
                S[i].pos[0].ci = width - 1;
            break;
        case Up:
            S[i].pos[0].ri--;
            if (S[i].pos[0].ri == 0)
                S[i].pos[0].ri = heigth - 1;
            break;
        case Down:
            S[i].pos[0].ri++;
            if (S[i].pos[0].ri == heigth)
                S[i].pos[0].ri = 1;
            break;
        }
    }
}

void moveSnakeInLvl(Snake*& S, int nop, int heigth, int width)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = S[i].size - 1; j > 0; j--)
        {
            S[i].pos[j].ri = S[i].pos[j - 1].ri;
            S[i].pos[j].ci = S[i].pos[j - 1].ci;
        }
        switch (S[i].dir)
        {
        case Right:
            S[i].pos[0].ci++;
           // if (S[i].pos[0].ci == width)
             //   S[i].pos[0].ci = 1;
            break;
        case Left:
            S[i].pos[0].ci--;
           // if (S[i].pos[0].ci == 0)
             //   S[i].pos[0].ci = width - 1;
            break;
        case Up:
            S[i].pos[0].ri--;
           // if (S[i].pos[0].ri == 0)
             //   S[i].pos[0].ri = heigth - 1;
            break;
        case Down:
            S[i].pos[0].ri++;
           // if (S[i].pos[0].ri == heigth)
             //   S[i].pos[0].ri = 1;
            break;
        }
    }
}

void changeDirection(Snake*& S, int nop, char key)
{
    for (int i = 0; i < nop; i++)
    {
        if (key == S[i].LeftKey)
        {
            if (S[i].dir != Right)
                S[i].dir = Left;
        }
        if (key == S[i].RightKey)
        {
            if (S[i].dir != Left)
                S[i].dir = Right;
        }
        if (key == S[i].UpKey)
        {
            if (S[i].dir != Down)
                S[i].dir = Up;
        }
        if (key == S[i].DownKey)
        {
            if (S[i].dir != Up)
                S[i].dir = Down;
        }
    }
}

bool foodIsEaten(Snake* S, int nop, Food F)
{
    for (int i = 0; i < nop; i++)
    {
        if (S[i].pos[0].ri == F.fp.ri && S[i].pos[0].ci == F.fp.ci)
        {
            return true;
        }
    }
    return false;
}

int whoAteFood(Snake* S, int nop, Food F)
{
    for (int i = 0; i < nop; i++)
    {
        if (S[i].pos[0].ri == F.fp.ri && S[i].pos[0].ci == F.fp.ci)
        {
            return i;
        }
    }
}

void expandSnake(Snake*& S, int nop, Food F)
{
    int sn = whoAteFood(S, nop, F);
    Position* Hp = new Position[S[sn].size + 1];
    for (int i = 0; i < S[sn].size; i++)
    {
        Hp[i] = S[sn].pos[i];
    }
    Hp[S[sn].size] = S[sn].pos[S[sn].size - 1];
    S[sn].size++;
    S[sn].score++;
    delete[] S[sn].pos;
    S[sn].pos = Hp;
}

void displayScore(Snake* S, int nop)
{
    for (int i = 0; i < nop; i++)
    {
        SetClr(7);
        gotoRowCol(10 + i, 100);
        cout << S[i].pName << " 's score : " << S[i].score;
    }
}

bool sucide(Snake* S, int nop)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = 1; j < S[i].size; j++)
        {
            if (S[i].pos[0].ri == S[i].pos[j].ri && S[i].pos[0].ci == S[i].pos[j].ci)
                return true;
        }
    }
    return false;
}

int whichSnakeSeucide(Snake* S, int nop)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = 1; j < S[i].size; j++)
        {
            if (S[i].pos[0].ri == S[i].pos[j].ri && S[i].pos[0].ci == S[i].pos[j].ci)
                return i;
        }
    }
}

void deleteSucidalSnake(Snake*& S, int nop)
{
    int sn = whichSnakeSeucide(S, nop);
    S[sn].size = 0;
    S[sn].pos[0].ri = 100;
    S[sn].pos[0].ci = 100;
    SetClr(7);
    gotoRowCol(20 + sn, 100);
    cout << "                                ";
    gotoRowCol(20 + sn, 100);
    cout << S[sn].pName << " sucide";
}

bool diedAtBoundry(Snake* S, int nop, int height, int width)
{
    for (int i = 0; i < nop; i++)
    {
        if (S[i].pos[0].ri == 0 || S[i].pos[0].ci == 0 || S[i].pos[0].ri == height || S[i].pos[0].ci == width)
            return true;
    }
    return false;
}

int whichSnakeDiedAtBoundry(Snake* S, int nop, int height, int width)
{
    for (int i = 0; i < nop; i++)
    {
        if (S[i].pos[0].ri == 0 || S[i].pos[0].ci == 0 || S[i].pos[0].ri == height || S[i].pos[0].ci == width)
            return i;
    }
}

void deleteSnakeAtBoundry(Snake*& S, int nop, int height, int width)
{
    int sn = whichSnakeDiedAtBoundry(S, nop, height, width);
    S[sn].size = 0;
    S[sn].pos[0].ri = 100;
    S[sn].pos[0].ci = 100;
    SetClr(7);
    gotoRowCol(30 + sn, 100);
    cout << "                                ";
    gotoRowCol(30 + sn, 100);
    cout << S[sn].pName << " died at boundry";
}

bool diedAtHurdelsLvl2(Snake* S, int nop)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = 0; j <= 25; j++)
        {
            if (S[i].pos[0].ri == 10 + j && (S[i].pos[0].ci == 20 || S[i].pos[0].ci == 60))
                return true;
        }
        for (int j = 0; j <= 40; j++)
        {
            if (S[i].pos[0].ri == 10 && S[i].pos[0].ci == 20 + j)
                return true;
        }
        for (int j = 0; j <= 10; j++)
        {
            if (S[i].pos[0].ri == 35 && S[i].pos[0].ci == 20 + j)
                return true;
        }
        for (int j = 0; j <= 10; j++)
        {
            if (S[i].pos[0].ri == 35 && S[i].pos[0].ci == 50 + j)
                return true;
        }
    }
    return false;
}

int whichSnakeDiedAtHurdelsOfLvl2(Snake* S, int nop)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = 0; j <= 25; j++)
        {
            if (S[i].pos[0].ri == 10 + j && (S[i].pos[0].ci == 20 || S[i].pos[0].ci == 60))
                return i;
        }
        for (int j = 0; j <= 40; j++)
        {
            if (S[i].pos[0].ri == 10 && S[i].pos[0].ci == 20 + j)
                return i;
        }
        for (int j = 0; j <= 10; j++)
        {
            if (S[i].pos[0].ri == 35 && S[i].pos[0].ci == 20 + j)
                return i;
        }
        for (int j = 0; j <= 10; j++)
        {
            if (S[i].pos[0].ri == 35 && S[i].pos[0].ci == 50 + j)
                return i;
        }
    }
}

void deleteSnakeAtHurdelOfLvl2(Snake*& S, int nop)
{
    int sn = whichSnakeDiedAtHurdelsOfLvl2(S, nop);
    S[sn].size = 0;
    S[sn].pos[0].ri = 100;
    S[sn].pos[0].ci = 100;

    SetClr(7);
    gotoRowCol(40 + sn, 100);
    cout << "                                ";
    gotoRowCol(40 + sn, 100);
    cout << S[sn].pName << " died at Hurdels";
}

bool diedAtHurdelsLvl3(Snake* S, int nop)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = 0; j <= 5; j++)
        {
            if (S[i].pos[0].ri == 10 + j && (S[i].pos[0].ci == 15 || S[i].pos[0].ci == 65))
                return true;
            if (S[i].pos[0].ri == j + 35 && S[i].pos[0].ci == 25)
                return true;
            if (S[i].pos[0].ri == j + 40 && S[i].pos[0].ci == 40)
                return true;
        }
        for (int j = 0; j <= 5; j++)
        {
            if (S[i].pos[0].ri == 25 && S[i].pos[0].ci == j + 10)
                return true;
            if (S[i].pos[0].ri == 30 && S[i].pos[0].ci == j + 50)
                return true;
            if (S[i].pos[0].ri == 40 && S[i].pos[0].ci == j + 60)
                return true;
            if (S[i].pos[0].ri == 15 && S[i].pos[0].ci == j + 40)
                return true;
        }
    }
    return false;
}

int whichSnakeDiedAtHurdelsOfLvl3(Snake* S, int nop)
{
    for (int i = 0; i < nop; i++)
    {
        for (int j = 0; j <= 5; j++)
        {
            if (S[i].pos[0].ri == 10 + j && (S[i].pos[0].ci == 15 || S[i].pos[0].ci == 65))
                return i;
            if (S[i].pos[0].ri == j + 35 && S[i].pos[0].ci == 25)
                return i;
            if (S[i].pos[0].ri == j + 40 && S[i].pos[0].ci == 40)
                return i;
        }
        for (int j = 0; j <= 5; j++)
        {
            if (S[i].pos[0].ri == 25 && S[i].pos[0].ci == j + 10)
                return i;
            if (S[i].pos[0].ri == 30 && S[i].pos[0].ci == j + 50)
                return i;
            if (S[i].pos[0].ri == 40 && S[i].pos[0].ci == j + 60)
                return i;
            if (S[i].pos[0].ri == 15 && S[i].pos[0].ci == j + 40)
                return i;
        }
    }
}

void deleteSnakeAtHurdelOfLvl3(Snake*& S, int nop)
{
    int sn = whichSnakeDiedAtHurdelsOfLvl3(S, nop);
    S[sn].size = 0;
    S[sn].pos[0].ri = 100;
    S[sn].pos[0].ci = 100;
    SetClr(7);
    gotoRowCol(40 + sn, 100);
    cout << "                                ";
    gotoRowCol(40 + sn, 100);
    cout << S[sn].pName << " died at Hurdels";
}

bool elligibleForLvlChange(Snake* S, int nop, int s)
{
    for (int i = 0; i < nop; i++)
    {
        if (S[i].score == s)
            return true;
    }
    return false;
}
















void duniyaGolHai(int nop, string name[], int sp)
{

    srand(time(0));
    Snake* S;
    Food F;
    char ch;
    int height = 50, width = 80;
    int c = nop;
    system("cls");
    init(S, nop, height, width, name);
    generateFood(F, S,nop, height, width);
    displaySnake(S, nop);
    displayFood(F);
    displayboundry(height, width);
    while (true)
    {
        if (_kbhit())
        {
              ch = _getch();
              if (ch == -32)
              {
                  ch = _getch();
                  changeDirection(S, nop, ch);
              }
              else
                  changeDirection(S, nop, ch);
        }
        displayScore(S, nop);
        displaySnake(S, nop);
        Sleep(sp);
        eraseSnake(S, nop);
        moveSnakeDuniyaGol(S, nop, height, width);
        if (foodIsEaten(S, nop, F))
        {
            expandSnake(S, nop, F);
            generateFood(F, S,nop, height, width);
            displayFood(F);
        }
        if (sucide(S, nop))
        {
            deleteSucidalSnake(S, nop);
            c--;
        }
        if (c == 0)
        {
            system("cls");
            gotoRowCol(height / 2, width / 2);
            cout << "All Snakes Died";
            PlaySound(TEXT("All Snakes Died"), NULL, SND_SYNC);
            break;
        }
    }
}

void lvl1(int nop, string name[], int sp, int s)
{
    srand(time(0));
    Snake* S;
    Food F;
    char ch;
    int height = 50, width = 80;
    int c = nop;
    system("cls");
    init(S, nop, height, width, name);
    generateFood(F, S,nop, height, width);
    displaySnake(S, nop);
    displayFood(F);
    displayboundry(height, width);
    while (true)
    {
        if (_kbhit())
        {
              ch = _getch();
              if (ch == -32)
              {
                  ch = _getch();
                  changeDirection(S, nop, ch);
              }
              else
                  changeDirection(S, nop, ch);
        }
        displayScore(S, nop);
        displaySnake(S, nop);
        Sleep(sp);
        eraseSnake(S, nop);
        moveSnakeInLvl(S, nop, height, width);
        if (foodIsEaten(S, nop, F))
        {
            expandSnake(S, nop, F);
            generateFood(F, S,nop, height, width);
            displayFood(F);
        }
        if (sucide(S, nop))
        {
            deleteSucidalSnake(S, nop);
            c--;
        }
        if (diedAtBoundry(S, nop, height, width))
        {
            deleteSnakeAtBoundry(S, nop, height, width);
            c--;
        }
        if (elligibleForLvlChange(S, nop, s))
        {
            break;
        }
        if (c == 0)
        {
            system("cls");
            gotoRowCol(height / 2, width / 2);
            cout << "All Snakes Died";
            PlaySound(TEXT("All Snakes Died"), NULL, SND_SYNC);
            _getch();
            exit(0);
        }
    }
}

void lvl2(int nop, string name[], int sp, int s)
{
    srand(time(0));
    Snake* S;
    Food F;
    char ch;
    int height = 50, width = 80;
    int c = nop;
    system("cls");
    init(S, nop, height, width, name);
    generateFoodForLvl2(F, S, nop, height, width);
    displaySnake(S, nop);
    displayFood(F);
    displayboundryLvl2(height, width);
    while (true)
    {
        if (_kbhit())
        {
            ch = _getch();
            if (ch == -32)
            {
                ch = _getch();
                changeDirection(S, nop, ch);
            }
            else
                changeDirection(S, nop, ch);
        }
        displayScore(S, nop);
        displaySnake(S, nop);
        Sleep(sp);
        eraseSnake(S, nop);
        moveSnakeInLvl(S, nop, height, width);
        if (foodIsEaten(S, nop, F))
        {
            expandSnake(S, nop, F);
            generateFoodForLvl3(F, S, nop, height, width);
            displayFood(F);
        }
        if (sucide(S, nop))
        {
            deleteSucidalSnake(S, nop);
            c--;
        }
        if (diedAtBoundry(S, nop, height, width))
        {
            deleteSnakeAtBoundry(S, nop, height, width);
            c--;
        }
        if (diedAtHurdelsLvl2(S, nop))
        {
            deleteSnakeAtHurdelOfLvl2(S, nop);
            c--;
        }
        if (elligibleForLvlChange(S, nop, s))
        {
            break;
        }
        if (c == 0)
        {
            system("cls");
            gotoRowCol(height / 2, width / 2);
            cout << "All Snakes Died";
            PlaySound(TEXT("All Snakes Died"), NULL, SND_SYNC);
            _getch();
            exit(0);
        }
    }
}

void lvl3(int nop, string name[], int sp, int s)
{
    srand(time(0));
    Snake* S;
    Food F;
    char ch;
    int height = 50, width = 80;
    int c = nop;
    system("cls");
    init(S, nop, height, width, name);
    generateFoodForLvl3(F, S, nop, height, width);
    displaySnake(S, nop);
    displayFood(F);
    displayboundryLvl3(height, width);
    while (true)
    {
        if (_kbhit())
        {
            ch = _getch();
            if (ch == -32)
            {
                ch = _getch();
                changeDirection(S, nop, ch);
            }
            else
                changeDirection(S, nop, ch);
        }
        displayScore(S, nop);
        displaySnake(S, nop);
        Sleep(sp);
        eraseSnake(S, nop);
        moveSnakeInLvl(S, nop, height, width);
        if (foodIsEaten(S, nop, F))
        {
            expandSnake(S, nop, F);
            generateFoodForLvl2(F, S, nop, height, width);
            displayFood(F);
        }
        if (sucide(S, nop))
        {
            deleteSucidalSnake(S, nop);
            c--;
        }
        if (diedAtBoundry(S, nop, height, width))
        {
            deleteSnakeAtBoundry(S, nop, height, width);
            c--;
        }
        if (diedAtHurdelsLvl3(S, nop))
        {
            deleteSnakeAtHurdelOfLvl3(S, nop);
            c--;
        }
        if (elligibleForLvlChange(S, nop, s))
        {
            break;
        }
        if (c == 0)
        {
            system("cls");
            gotoRowCol(height / 2, width / 2);
            cout << "All Snakes Died";
            PlaySound(TEXT("All Snakes Died"), NULL, SND_SYNC);
            _getch();
            exit(0);
        }
    }
}

void allLevel(int nop, string name[], int sp)
{
    int s;
    cout << "\nEnter Score At Which lvl change :";
    cin >> s;
    lvl1(nop, name, sp, s);
    lvl2(nop, name, sp, s);
    lvl3(nop, name, sp, s);
    system("cls");
    gotoRowCol(50 / 2, 80 / 2);
    cout << "All Stages cleared";
    PlaySound(TEXT("Live"), NULL, SND_SYNC);

}














int main()
{
    cout << "INSTRUCTIONS\n* Maximum 4 player can play\n*Player 1 is control by direction arrow key\n*Player 2 is control by AWSD key\n*Player 3 is control by 5123 key\n*Player 4 is control by YGHJ key\nPress any key to continue";
    _getch();
    system("cls");
    int nop, sp, l;
    string name[4] = {};
    cout << "\nEnter number of Player : ";
    cin >> nop;
    cout << "\n\nChoose Difficulty :\n1-Easy\n2-Medium\n3-Hard\n";
    cin >> sp;
    if (sp == 1)
        sp = 100;
    else if (sp == 2)
        sp = 80;
    else
        sp = 60;
    for (int i = 0; i < nop; i++)
    {
        cout << "\n\nEnter player " << i + 1 << " name : ";
        cin >> name[i];
    }
    cout << "\n\nWhich phase you want to play :\n1-Duniya Gol Hai\n2-Levl\n";
    cin >> l;
    if (l == 1)
    {
        duniyaGolHai(nop, name, sp);
    }
    else
    {
          allLevel(nop, name, sp);
    }


    return _getch();
}

