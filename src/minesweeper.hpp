#ifndef MINESWEEPER_HPP_
#define MINESWEEPER_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

enum above_kind // How the tile looks when covered
{   
    covered = 0, 
    flag = 1, 
    questionmark = 2,
    uncovered = 3,
};

enum under_type // What is hidden below
{
    hit = 9,
    mine = 10,
    wrong = 11,
    number = 12
};

enum result // Ways the move can result
{
    zero = 0,
    other = 1,
    lost = 2,
    win = 3
};

struct tile // Properties of each tile
{  
    int number;
    under_type tile_type;
    above_kind hidden_kind;
};

struct difficulty // Game parameters
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
        int seconds;
    
        void selectDifficulty(int num);
        void generateMines(int x, int y);
        void changeKind(int x, int y);
        void saveScore();
        bool checkWin();
        result gameLogic(int x, int y);

        // Functions returning variables used by other classes
        int getWidth() {return dif.width;};
        int getHeight() {return dif.height;};
        int getMineCount() {return dif.mine_count;};
        int getShownTiles() {return shown_tiles;};
        int getFlagged() {return flagged;};
        int getTop() {return top_score;};

    private:
        difficulty dif;
        int shown_tiles;
        int flagged;
        int top_score;
        int choice;

        void setSurroundingTiles(int x, int y);
        void showZeros(int x, int y);
        void showField();
        bool validTile(int x, int y);
};

#endif
