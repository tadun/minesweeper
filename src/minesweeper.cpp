// The Minesweeper Game by Tadeas Horn

#include "minesweeper.hpp"

const difficulty beginner = {9, 9, 10};
const difficulty intermediate = {16, 16, 40};
const difficulty expert = {30, 16, 99};
difficulty dif_array[3] = {beginner, intermediate, expert};

// Reacts baset on the selected tile and returns the result
result Minesweeper::gameLogic(int x, int y) 
{
    if (field[x][y]->hidden_kind != covered && field[x][y]->hidden_kind != questionmark) return incorrect;
    
    if (getShownTiles() == 0) { // Make sure the mine was not hit on the first click
        generateMines(x, y);
    }

    switch (field[x][y]->number) {
    case 0:
        showZeros(x,y); // Recursively run for all surrounding zeros
        if (checkWin()) {return win;}
        return zero;
        break;

    case mine: // Explode
        shown_tiles++;
        field[x][y]->hidden_kind = uncovered;
        reactToHit();
        return loss;
        break;

    default: // Just a regular number
        shown_tiles++;
        field[x][y]->hidden_kind = uncovered;
        if (checkWin()) {return win;}
        return regular;
        break;
    }
}

// Show remaining mines and mark incorrect flags
void Minesweeper::reactToHit()
{
    for (int i = 0; i < dif.width; i++) {
        for (int j = 0; j < dif.height; j++) {
            if (field[i][j]->number == mine && field[i][j]->hidden_kind != flag) {
                field[i][j]->hidden_kind = uncovered;
                field[i][j]->number = mine; // Show remaining mines

            }
            else if (field[i][j]->hidden_kind == flag && field[i][j]->number != mine) {
                field[i][j]->hidden_kind = uncovered;
                field[i][j]->number = wrong; // Incorrect flag
            }
        }
    }
}

// Change the kind of the tile when covered
void Minesweeper::changeKind(int x, int y) 
{
    if (field[x][y]->hidden_kind == covered) {
        field[x][y]->hidden_kind = flag; // Protects the tile
        flagged++;
        }

    else if (field[x][y]->hidden_kind == flag) {
        field[x][y]->hidden_kind = questionmark; // Can be uncovered
        flagged--;
        }

    else if (field[x][y]->hidden_kind == questionmark) {
        field[x][y]->hidden_kind = covered;
    }
}

// Choose parameters, load score, prepare field
void Minesweeper::selectDifficulty(int difficulty_input)
{
    shown_tiles = 0;
    flagged = 0;

    if (difficulty_input != 4) {
        difficulty_choice = difficulty_input;
        dif = dif_array[difficulty_choice-1];
    }
   
    // Generate the field without any values
    for (int i = 0; i < dif.width; i++) {
        for (int j = 0; j < dif.height; j++) {
            tile *new_tile = new tile;
            new_tile->number = 0;
            new_tile->hidden_kind = covered;
            field[i][j] = new_tile;
        }
    }
}

// Generate random mines based on mine_count
void Minesweeper::generateMines(int x, int y) 
{
    int mine_x, mine_y;

    srand(time(NULL)); // Seed for the rand function

    for (int k = 0; k < dif.mine_count; k++) {
        do {
          mine_x = rand() % dif.width;
          mine_y = rand() % dif.height;
        }
        // In case the there already is a mine or it was hit on the first click
        while (field[mine_x][mine_y]->number == mine || (mine_x == x && mine_y == y));
        
        field[mine_x][mine_y]->number = mine;
        setSurroundingTiles(mine_x, mine_y); // Add one to all eight closest tiles
    }
}

// Increment the value of all surrounding tiles that are not a mine
void Minesweeper::setSurroundingTiles(int x, int y)
{
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            if (validTile(x-1+i, y-1+j) && field[x-1+i][y-1+j]->number != mine && !(i == 1 && j == 1)) {
                field[x-1+i][y-1+j]->number++;
            }       
        }
    }
}

// Uncover all surrounding empty tiles
void Minesweeper::showZeros(int x, int y) 
{
    if (!validTile(x, y) || !(field[x][y]->hidden_kind == covered || field[x][y]->hidden_kind == questionmark)) return; // Unable to uncover
    field[x][y]->hidden_kind = uncovered;
    shown_tiles++;

    if (field[x][y]->number == 0) {
        for (int i = 0; i <= 2; i++) {
            for (int j = 0; j <= 2; j++) {
                if (!(i == 1 && j == 1)) {
                    showZeros(x-1+i, y-1+j); // Recursive call
                }
            }
        }
    }
}

// Checks if the coordinates exist in the field
bool Minesweeper::validTile(int x, int y)
{
    return (x >= 0) && (x < dif.width) && (y >= 0) && (y < dif.height);
}

// Flag all remaining mines and save the compare the score
bool Minesweeper::checkWin()
{    
    if (shown_tiles != (dif.width)*(dif.height) - dif.mine_count) return false; // Only mines remained hidden
    for (int i = 0; i < dif.width; i++) {
        for (int j = 0; j < dif.height; j++) {
            if (field[i][j]->number == mine) {
                field[i][j]->hidden_kind = uncovered;
                field[i][j]->number = flag;
            }
        }
    }
    return true;
}

Minesweeper::~Minesweeper() 
{
    for (int i = 0; i < dif.width; i++) {
        for (int j = 0; j < dif.height; j++) {
            delete field[i][j];
        }
    }
}
