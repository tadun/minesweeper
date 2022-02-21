#include "minesweeper.hpp"

const difficulty beginner = {9, 9, 10};
const difficulty intermediate = {16, 16, 40};
const difficulty expert = {30, 16, 99};

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size) 
    : wxFrame(NULL, wxID_ANY, title, pos, size),
    m_timer(this, 5)
{
    // Setting up the status bar
    CreateStatusBar();
    string output = "Time: " + to_string(seconds/60) + ":" + to_string(seconds%60) 
        + "\t  Mines: " + to_string(mine_count-flagged);
    SetStatusText(output);
    new_board = new wxPanel(this, wxID_ANY);

    // Creating the difficulty menu
    wxMenu *menuGame = new wxMenu;
    menuGame->Append(1, "&Beginner");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::Beginner, this, 1);
    menuGame->Append(2, "&Intermediate");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::Intermediate, this, 2);
    menuGame->Append(3, "&Expert");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MyFrame::Expert, this, 3);
    menuGame->AppendSeparator();
    menuGame->Append(wxID_EXIT);

    // Creating the score menu
    fstream file("scores_1.csv");
    string line;
    string word;
    string time;
    string name;
    wxMenu *menuScore = new wxMenu;
    while (file >> line) {
        stringstream s(line);
        getline(s, word, ',');
        time = to_string(stoi(word)/60) + ":" + to_string(stoi(word)%60);
        getline(s, name, ',');
        menuScore->Append(wxID_ANY, name + " " + time);
    }
    
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuGame, "&Game");
    menuBar->Append(menuScore, "&Score");
    SetMenuBar(menuBar);

    generateButtons();
}

bool  MyApp::OnInit() // Define frame size
{
    ::wxInitAllImageHandlers();

    width = beginner.width;
    height = beginner.height;
    mine_count = beginner.mine_count;

    MyFrame *frame = new MyFrame("Minesweeper", wxPoint(550, 275), wxSize(beginner.width*20, beginner.height*20+55));
    frame->Show();
    return true;
}

void MyFrame::Beginner(wxEvent& event) // Difficulty 1: 9*9 10 mines
{
    Close(true);
    width = beginner.width;
    height = beginner.height;
    mine_count = beginner.mine_count;
    shown_tiles = 0;
    seconds = 0;
    flagged = 0;
    MyFrame *frame = new MyFrame("Minesweeper", wxPoint(550, 275), wxSize(width*20, height*20+55));
    frame->Show();
}

void MyFrame::Intermediate(wxEvent& event) // Difficulty 2: 16*16 40 mines 
{
    Close(true);
    width = intermediate.width;
    height = intermediate.height;
    mine_count = intermediate.mine_count;
    shown_tiles = 0;
    seconds = 0;
    flagged = 0;
    MyFrame *frame = new MyFrame("Minesweeper", wxPoint(550, 275), wxSize(width*20, height*20+55));
    frame->Show();
}

void MyFrame::Expert(wxEvent& event) // Difficulty 3: 30*16 99 mines
{
    Close(true);
    width = expert.width;
    height = expert.height;
    mine_count = expert.mine_count;
    shown_tiles = 0;
    seconds = 0;
    flagged = 0;
    MyFrame *frame = new MyFrame("Minesweeper", wxPoint(550, 275), wxSize(width*20, height*20+55));
    frame->Show();
}

void MyFrame::generateButtons() // Create a field of buttons
{
    int button_id = 0;

    wxBitmap Bitmap;
    Bitmap.LoadFile("Images/down.png", wxBITMAP_TYPE_PNG);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            
            tile *new_tile = new tile;
            new_tile->type = 0;
            new_tile->kind = 0;
            
            new_tile->button = new wxBitmapButton (this, button_id, Bitmap, wxPoint(i*20, j*20), wxSize(20,20));
            new_tile->button->Bind(wxEVT_RIGHT_DOWN, &MyFrame::OnRightDown, this);
            new_tile->button->Bind(wxEVT_LEFT_DOWN, &MyFrame::OnLeftDown, this);

            field[i][j] = new_tile;
            button_id++;
        }
    }
}

int MyFrame::generateMines(int mines, int x, int y) // Generate random mines
{
    int mine_x;
    int mine_y;

    srand(time(NULL));

// Generate mines
    for (int k = 0; k < mines; k++) {
        mine_x = rand() % width;
        mine_y = rand() % height;

        // In case the there already is a mine or it was hit on the first click
        while (field[mine_x][mine_y]->type == 10 || (mine_x == x && mine_y == y)) { 
            mine_x = rand() % width;
            mine_y = rand() % height;
        }
        field[mine_x][mine_y]->type = 10;
        countAdjecentMines(mine_x, mine_y);
    }

    return 0;
}

void MyFrame::countAdjecentMines(int x, int y) // Add one to all eight surrounding tiles
{
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            if (validTile(x-1+i, y-1+j) && field[x-1+i][y-1+j]->type != 10 && !(i == 1 && j == 1)) {
                field[x-1+i][y-1+j]->type++;
            }       
        }
    }
}

void MyFrame::OnLeftDown(wxMouseEvent& event) // Uncover the tile and react appropriately
{
    int id = event.GetId();
    int x = id/height;
    int y = id%height;

    if (shown_tiles == 0) {
        m_timer.Start(1000);
        generateMines(mine_count, x, y);
    }
    
    wxBitmapButton *button = reinterpret_cast <wxBitmapButton*> (event.GetEventObject());

    if (field[x][y]->kind == 0 || field[x][y]->kind == 2) { // Down or a questionmark
        switch (field[x][y]->type) {  
        case 0: // Recursively show surronding empty tiles
            showZeros(x,y);
            gameWon();
            break;
        
        case 10: // Hit a mine
            shown_tiles++;
            field[x][y]->kind = 3;
            button->SetBitmap(wxBitmap(("Images/hit.png"), wxBITMAP_TYPE_PNG));
            SetStatusText("Good luck next time!");
            m_timer.Stop();
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) { // Show remaining mines
                    if ((field[i][j]->kind == 0 || field[i][j]->kind == 2) && field[i][j]->type == 10) { 
                    field[i][j]->button->SetBitmap(wxBitmap(("Images/mine.png"), wxBITMAP_TYPE_PNG));
                    }
                    else if (field[i][j]->kind == 1 && field[i][j]->type != 10) { // Incorrect flag
                        field[i][j]->button->SetBitmap(wxBitmap(("Images/wrong.png"), wxBITMAP_TYPE_PNG));
                    }
                    field[i][j]->button->Disable(); // Deactivate all buttons
                }
            }
            break;

        default: // Show apropriate mine
            shown_tiles++;
            field[x][y]->kind = 3;
            string name = "Images/" + to_string(field[x][y]->type) + ".png";
            button->SetBitmap(wxBitmap((name), wxBITMAP_TYPE_PNG));
            gameWon();
            break;
        }
        
    }
    event.Skip();
}

void MyFrame::OnRightDown(wxMouseEvent& event) // Swich between a flag, a questionmark and a neutral
{
    int id = event.GetId();
    int x = id/height;
    int y = id%height;

    if (field[x][y]->kind == 0) {
        field[x][y]->kind = 1;
        field[x][y]->button->SetBitmap(wxBitmap(("Images/flag.png"), wxBITMAP_TYPE_PNG));
        flagged++;
        }

    else if (field[x][y]->kind == 1) {
        field[x][y]->kind = 2;
        field[x][y]->button->SetBitmap(wxBitmap(("Images/questionmark.png"), wxBITMAP_TYPE_PNG));
        flagged--;
        }

    else if (field[x][y]->kind == 2) {
        field[x][y]->kind = 0;
        field[x][y]->button->SetBitmap(wxBitmap(("Images/down.png"), wxBITMAP_TYPE_PNG));
    }
}

void MyFrame::showZeros(int x, int y) // Uncover all surrounding empty tiles
{
    if (!validTile(x, y) || (field[x][y]->kind != 0 && field[x][y]->kind != 2)) return;
    field[x][y]->kind = 3;

    if (field[x][y]->type == 0) {
        shown_tiles++;
        field[x][y]->button->SetBitmap(wxBitmap(("Images/0.png"), wxBITMAP_TYPE_PNG));
        for (int i = 0; i <= 2; i++) {
            for (int j = 0; j <= 2; j++) {
                showZeros(x-1+i, y-1+j);
            }
        }
    }

    else if (field[x][y]->type != 10) {
        string name = "Images/" + to_string(field[x][y]->type) + ".png";
        field[x][y]->button->SetBitmap(wxBitmap((name), wxBITMAP_TYPE_PNG));
        shown_tiles++;
    }
}

bool MyFrame::validTile(int row, int col) // Checks if the coordinates exist in the field
{
    return (row >= 0) && (row < width) && (col >= 0) && (col < height);
}

void MyFrame::gameWon() // Flag all remaining mines and disable buttons
{
    if (shown_tiles != (width)*(height) - mine_count) return; // Only mines remained hidden
    m_timer.Stop();
    string time = to_string(seconds/60) + ":" + to_string(seconds%60);
    SetStatusText("You won!\t\t Time: " + time);
    saveScore();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (field[i][j]->type == 10) {
                field[i][j]->button->SetBitmap(wxBitmap(("Images/flag.png"), wxBITMAP_TYPE_PNG));
            }
            field[i][j]->button->Disable();
        }
    }
    
}

void MyFrame::saveScore() // Save the score if it is one of the top five
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

void MyFrame::onExit(wxCommandEvent &event)
{
    Close(true);
}

void MyFrame::OnTimer(wxTimerEvent& event) // Update time and remaining mines each second
{
    seconds++;
    string output = "Time: " + to_string(seconds/60) + ":" + to_string(seconds%60) + "\t  Mines: " + to_string(mine_count-flagged);
    SetStatusText(output);
}

wxIMPLEMENT_APP(MyApp);

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_TIMER(5, MyFrame::OnTimer)
    EVT_MENU(wxID_EXIT, MyFrame::onExit)
wxEND_EVENT_TABLE();
