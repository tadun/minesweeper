#ifndef MINESWEEPER_HPP_
#define MINESWEEPER_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

enum kind {   
    covered = 0, 
    flag = 1, 
    questionmark = 2,
    uncovered = 3,
};

enum type {
    hit = 9,
    mine = 10,
    wrong = 11,
    number = 12
};

struct tile {  
    int number;
    type tile_type;
    kind hidden_kind;
};

struct difficulty {
    int width;
    int height;
    int mine_count;
};

class Minesweeper
{
    public:
        tile *field[30][16];
        difficulty dif;
        int shown_tiles;
        int seconds;
        int flagged;

        void SelectDifficulty(int choice);
        void GenerateField();
        void GenerateMines(int x, int y);
        void GameLogic(int x, int y);
        void ChangeKind(int x, int y);
        bool CheckWin();
        int GetWidth() {return dif.width;};
        int GetHeight() {return dif.height;};
        int GetMineCount() {return dif.mine_count;};
        ~Minesweeper();

    private:
        bool validTile(int x, int y);
        void setSurroundingTiles(int x, int y);
        void showZeros(int x, int y);
        void showField();
        void saveScore();
};

#endif
