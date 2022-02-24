#include "minesweeper.hpp"

using namespace std;

int main() {
    Minesweeper M;
    
    int x;
    int y;
    int dif;

    cout << "Choose difficulty[1/2/3]: " << endl;
    cin >> dif;


    M.SelectDifficulty(dif);
    M.GenerateField();
    M.GenerateMines();

    while(true) {
        M.ShowField();
        
        cout << "Enter x and y: " << endl;
        cin >> x; 
        cin >> y;
        cout << endl;

        M.Game(x, y);
    }
}