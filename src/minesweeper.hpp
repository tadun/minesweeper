#ifndef MINESWEEPER_HPP_
#define MINESWEEPER_HPP_

#include <iostream>

using namespace std;

class Field
{
    private:
        int side = 9;
        int mines = 9;

        int field [9] [9] = {};
        
    public:
        int generate(int side, int mines);
        int show();
        bool valid(int row, int col);
        void move();
        void recursive(int x, int y);

};

bool Field::valid(int row, int col)
{
    return (row >= 0) && (row < side) && (col >= 0) && (col < side);

}

int Field::generate(int side, int mines) {
    
    int mine_x;
    int mine_y;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < mines; i++) {
        mine_x = rand() % side;
        mine_y = rand() % side;
        while (field [mine_x] [mine_y] == 10) {
            mine_x = rand() % side;
            mine_y = rand() % side;
            cout << field [mine_x] [mine_y] << mine_x << mine_y << endl;
        }
        field [mine_x] [mine_y] = 10;

        if (valid(mine_x, mine_y-1) == true) {
            if (field [mine_x] [mine_y-1] != 10) {
                field [mine_x] [mine_y-1] ++;
            }
        }

        if (valid(mine_x, mine_y+1) == true) {
            if (field [mine_x] [mine_y+1] != 10) {
                field [mine_x] [mine_y+1] ++;
            }
        }

        if (valid(mine_x-1, mine_y-1) == true) {
            if (field [mine_x-1] [mine_y-1] != 10) {
                field [mine_x-1] [mine_y-1] ++;
            }
        }

        if (valid(mine_x-1, mine_y) == true) {
            if (field [mine_x-1] [mine_y] != 10) {
                field [mine_x-1] [mine_y] ++;
            }
        }

        if (valid(mine_x-1, mine_y+1) == true) {
            if (field [mine_x-1] [mine_y+1] != 10) {
                field [mine_x-1] [mine_y+1] ++;
            }
        }

        if (valid(mine_x+1, mine_y-1) == true) {
            if (field [mine_x+1] [mine_y-1] != 10) {
                field [mine_x+1] [mine_y-1] ++;
            }
        }

        if (valid(mine_x+1, mine_y) == true) {
            if (field [mine_x+1] [mine_y] != 10) {
                field [mine_x+1] [mine_y] ++;
            }
        }
        
        if (valid(mine_x+1, mine_y+1) == true) {
            if (field [mine_x+1] [mine_y+1] != 10) {
                field [mine_x+1] [mine_y+1] ++;
            }
        }
    }
}

void Field::recursive(int x, int y) {
    
    if (valid (x, y) == true) {
        if (field [x] [y] == 0) {
            recursive(x+1, y);
            recursive(x-1, y);
            recursive(x, y-1);
            recursive(x, y+1);
        }
    }
}

void Field::move()
{
    int mine_x = rand() % side;
    int mine_y = rand() % side;

    cout << mine_x << mine_y;

    if (field [mine_x] [mine_y] == 10) {
        cout << "boom" << endl;
    }

    else if (field [mine_x] [mine_y] > 0) {
        cout << field [mine_x] [mine_y] << endl;
    }

    else {
        cout << "empty" << endl;

    }
}

int Field::show() {
    
    for (int x = 0; x < side; x++) {
        for (int y = 0; y < side; y++) {
            if (field[x] [y] == 10) {
                cout << "*   ";
            }
            else {
                cout << field [x] [y] << "   ";
            }
        }
        cout << endl << endl;
    }
}

#endif