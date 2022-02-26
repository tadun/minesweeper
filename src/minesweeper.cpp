#include "minesweeper.hpp"

const difficulty beginner = {9, 9, 10};
const difficulty intermediate = {16, 16, 40};
const difficulty expert = {30, 16, 99};

void Minesweeper::Game(int x, int y) 
{
    if (field[x][y]->hidden_kind != covered && field[x][y]->hidden_kind != questionmark) return;
    
    if (field[x][y]->number == 0) {
        ShowZeros(x,y);
        CheckWin();
    }

    else if (field[x][y]->tile_type == mine) { // Explode
        shown_tiles++;
        field[x][y]->hidden_kind = uncovered;
        cout << "BOOM!" << endl;
        for (int i = 0; i < dif.width; i++) {
            for (int j = 0; j < dif.height; j++) { // Show remaining mines
                if (field[i][j]->tile_type == mine) {
                    field[i][j]->hidden_kind = uncovered;
                }
                else if (field[i][j]->hidden_kind == flag && field[i][j]->tile_type != mine) { // Incorrect flag
                    field[i][j]->tile_type = wrong;
                }
            }
        }
        field[x][y]->tile_type = hit;
    }

    else {
        shown_tiles++;
        field[x][y]->hidden_kind = uncovered;
        CheckWin();
    }

}

Minesweeper::~Minesweeper() 
{
    for (int i = 0; i < dif.width; i++) {
        for (int j = 0; j < dif.height; j++) {
            field[i][j] = NULL;
        }
    }
}

void Minesweeper::ChangeKind(int x, int y) 
{
    if (field[x][y]->hidden_kind == covered) {
        field[x][y]->hidden_kind = flag;
        flagged++;
        }

    else if (field[x][y]->hidden_kind == flag) {
        field[x][y]->hidden_kind = questionmark;
        flagged--;
        }

    else if (field[x][y]->hidden_kind == questionmark) {
        field[x][y]->hidden_kind = covered;
    }
}

void Minesweeper::SelectDifficulty(int choice)
{
    shown_tiles = 0;
    seconds = 0;
    flagged = 0;

    if (choice == 1) { // Difficulty 1: 9*9 10 mines
        dif = beginner;
    }

    else if (choice == 2) { // Difficulty 2: 16*16 40 mines 
        dif = intermediate;
    }

    else if (choice == 3) { // Difficulty 3: 30*16 99 mines
        dif = expert;
    }
}

void Minesweeper::GenerateField() // Create a field of buttons
{
    for (int i = 0; i < dif.width; i++) {
        for (int j = 0; j < dif.height; j++) {
            tile *new_tile = new tile;
            new_tile->number = 0;
            new_tile->tile_type = number;
            new_tile->hidden_kind = covered;
            field[i][j] = new_tile;
        }
    }
}

void Minesweeper::GenerateMines() // Generate random mines
{
    int mine_x;
    int mine_y;

    srand(time(NULL));

    for (int k = 0; k < dif.mine_count; k++) {
        do {
          mine_x = rand() % dif.width;
          mine_y = rand() % dif.height;
        }
        // In case the there already is a mine or it was hit on the first click
        while (field[mine_x][mine_y]->tile_type == mine /*|| (mine_x == x && mine_y == y)*/);
        
        field[mine_x][mine_y]->tile_type = mine;
        SetSurroundingTiles(mine_x, mine_y);
    }
}

void Minesweeper::SetSurroundingTiles(int x, int y) // Add one to all eight surrounding tiles
{
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            if (ValidTile(x-1+i, y-1+j) && field[x-1+i][y-1+j]->tile_type != 10 && !(i == 1 && j == 1)) {
                field[x-1+i][y-1+j]->number++;
            }       
        }
    }
}

void Minesweeper::ShowZeros(int x, int y) // Uncover all surrounding empty tiles
{
    if (!ValidTile(x, y) || !(field[x][y]->hidden_kind == covered || field[x][y]->hidden_kind == questionmark)) return;
    field[x][y]->hidden_kind = uncovered;
    shown_tiles++;

    if (field[x][y]->tile_type == 0) {
        for (int i = 0; i <= 2; i++) {
            for (int j = 0; j <= 2; j++) {
                if (!(i == 1 && j == 1)) {
                    ShowZeros(x-1+i, y-1+j);
                }
            }
        }
    }
}

bool Minesweeper::ValidTile(int x, int y) // Checks if the coordinates exist in the field
{
    return (x >= 0) && (x < dif.width) && (y >= 0) && (y < dif.height);
}

void Minesweeper::CheckWin() // Flag all remaining mines and disable buttons
{
    if (shown_tiles != (dif.width)*(dif.height) - dif.mine_count) return; // Only mines remained hidden
    cout << "You won!" << endl;
    SaveScore();
    for (int i = 0; i < dif.width; i++) {
        for (int j = 0; j < dif.height; j++) {
            if (field[i][j]->tile_type == mine) {
                field[i][j]->hidden_kind = flag;
            }
        }
    }
}

void Minesweeper::SaveScore() // Save the score if it is one of the top five
{
    fstream file("scores_1.csv");
    int score;
    string temporary[5];
    string word;
    string line;
    string my_name;

    int i = 0;
    while (file >> line) {
        stringstream s(line);

        getline(s, word, ',');
        score = stoi(word);

        if (score > seconds) { // New high score
            cout << "Enter your name: ";
            cin >> my_name;
            string out = to_string(seconds) + "," + my_name;
            temporary[i] = out;
            i++;

            if (i <= 3) {
                temporary[i] = line;
                i++;
            }

            while (file >> line && i <= 4) {
                temporary[i] = line;
                i++;
            }
        }

        else {
            temporary[i] = line;
            i++;
        }
    }

    file.close();
    file.open("scores_1.csv", ios::out | ios::trunc); // Replace the old scores

    for (int i = 0; i <= 4; i++) {
        file << temporary[i] << endl;
    }

    file.close();
}

void Minesweeper::ShowField()
{

    for (int j = 0; j < dif.width; j++) {
        for (int i = 0; i < dif.height; i++) {
            if (field[i][j]->hidden_kind == covered) {
                cout << "-   ";
            }

            else if (field[i][j]->hidden_kind == flag) {
                cout << "f   ";
            }

            else if (field[i][j]->hidden_kind == questionmark) {
                cout << "?   ";
            }
            else if (field[i][j]->tile_type == wrong) {
                cout << "x   ";
            }

            else if (field[i][j]->hidden_kind == uncovered) {
                if (field[i][j]->tile_type == mine) {
                    cout << "*   ";
                }
                else {
                    cout << field[i][j]->number << "   ";
                } 
            }
        }
    cout << endl << endl;
    }
}
