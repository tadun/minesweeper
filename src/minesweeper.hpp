#ifndef MINESWEEPER_HPP_
#define MINESWEEPER_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

enum kind 
{   
    covered = 0, 
    flag = 1, 
    questionmark = 2,
    uncovered = 3,
};

enum type 
{
    hit = 9,
    mine = 10,
    wrong = 11,
    number = 12
};

struct tile 
{  
    int number;
    type tile_type;
    kind hidden_kind;
};

struct difficulty 
{
    int width;
    int height;
    int mine_count;
};

class Minesweeper
{
    public:
        tile *field[30][16];
        ~Minesweeper();
    
        void selectDifficulty(int choice);
        void generateField();
        void generateMines(int x, int y);
        void gameLogic(int x, int y);
        void changeKind(int x, int y);
        bool checkWin();
        void saveScore();

        int getWidth() {return dif.width;};
        int getHeight() {return dif.height;};
        int getMineCount() {return dif.mine_count;};
        int getShownTiles() {return shown_tiles;};
        int getFlagged() {return flagged;};
        int getTop() {return top;};

    private:
        difficulty dif;
        int shown_tiles;
        int flagged;
        int top;
        bool validTile(int x, int y);
        void setSurroundingTiles(int x, int y);
        void showZeros(int x, int y);
        void showField();
};

#endif
