#ifndef MINESWEEPER_HPP_
#define MINESWEEPER_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

enum above_kind // How the tile looks when covered
{   
    covered = 12, 
    flag = 13, 
    questionmark = 14,
    uncovered = 15,
};

enum under_type // What is hidden below
{
    hit = 9,
    mine = 10,
    wrong = 11
};

enum result // Ways the move can result
{
    zero = 0,
    loss = 1,
    win = 2,
    regular = 3,
    incorrect = 4
};

struct tile // Properties of each tile
{  
    int number;
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
    
        result gameLogic(int x, int y);
        void selectDifficulty(int num);
        void generateMines(int x, int y);
        void changeKind(int x, int y);
        bool checkWin();

        // Functions returning variables used by other classes
        int getWidth() {return dif.width;};
        int getHeight() {return dif.height;};
        int getMineCount() {return dif.mine_count;};
        int getShownTiles() {return shown_tiles;};
        int getFlagged() {return flagged;};
        int getDifChoice() {return difficulty_choice;};

    private:
        difficulty dif;
        int shown_tiles;
        int flagged;
        int difficulty_choice;

        void reactToHit();
        void setSurroundingTiles(int x, int y);
        void showZeros(int x, int y);
        bool validTile(int x, int y);
};

#endif
