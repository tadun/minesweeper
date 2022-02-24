#include "minesweeper.hpp"

// saving score
// create a class for the game
// global variables
// memory leak

int width;
int height;
int mine_count;

int shown_tiles = 0;
int seconds = 0;
int flagged = 0;

const difficulty beginner = {9, 9, 10};
const difficulty intermediate = {16, 16, 40};
const difficulty expert = {30, 16, 99};

enum tile_number_options 
{   mine = 10, 
    empty = 0
};

enum hidden_kind_options 
{   covered = 0, 
    flag = 1, 
    questionmark = 2,
    uncovered = 3
};

enum id_options 
{   beginner_id = 1,
    intermediate_id = 2,
    expert_id = 3,
    timer_id = 4
};

wxIMPLEMENT_APP(App);

wxBEGIN_EVENT_TABLE(GameFrame, wxFrame)
    EVT_TIMER(timer_id, GameFrame::OnTimer)
    EVT_MENU(wxID_EXIT, GameFrame::OnExit)
wxEND_EVENT_TABLE();

GameFrame::GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size) 
    : wxFrame(NULL, wxID_ANY, title, pos, size),
    timer(this, timer_id)
{
    // Setting up the status bar
    CreateStatusBar();
    string output = "Time: " + to_string(seconds/60) + ":" + to_string(seconds%60) 
        + "\t  Mines: " + to_string(mine_count-flagged);
    SetStatusText(output);
    new_board = new wxPanel(this, wxID_ANY);

    // Creating the difficulty menu
    wxMenu *menu_game = new wxMenu;
    menu_game->Append(1, "&Beginner");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::SelectDifficulty, this, beginner_id);
    menu_game->Append(2, "&Intermediate");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::SelectDifficulty, this, intermediate_id);
    menu_game->Append(3, "&Expert");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::SelectDifficulty, this, intermediate_id);
    menu_game->AppendSeparator();
    menu_game->Append(wxID_EXIT);

    // Creating the score menu
    fstream file("scores_1.csv");
    string line;
    string word;
    string time;
    string name;
    wxMenu *menu_score = new wxMenu;
    while (file >> line) {
        stringstream s(line);
        getline(s, word, ',');
        time = to_string(stoi(word)/60) + ":" + to_string(stoi(word)%60);
        getline(s, name, ',');
        menu_score->Append(wxID_ANY, name + " " + time);
    }
    
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu_game, "&Game");
    menuBar->Append(menu_score, "&Score");
    SetMenuBar(menuBar);

    LoadBitmaps();
    GenerateButtons();
}

GameFrame::~GameFrame() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            field[i][j] = NULL;
        }
    }
}

bool App::OnInit() // Define frame size
{
    ::wxInitAllImageHandlers();

    width = beginner.width;
    height = beginner.height;
    mine_count = beginner.mine_count;

    GameFrame *frame = new GameFrame("Minesweeper", wxPoint(550, 275), wxSize(beginner.width*20, beginner.height*20+55));
    frame->Show();
    return true;
}

void GameFrame::SelectDifficulty(wxEvent& event) // Difficulty 1: 9*9 10 mines
{
    int id = event.GetId();
    Close(true);
    shown_tiles = 0;
    seconds = 0;
    flagged = 0;

    if (id == beginner_id) { // Difficulty 1: 9*9 10 mines
        width = beginner.width;
        height = beginner.height;
        mine_count = beginner.mine_count;
    }

    else if (id == intermediate_id) { // Difficulty 2: 16*16 40 mines 
        width = intermediate.width;
        height = intermediate.height;
        mine_count = intermediate.mine_count;
    }

    else if (id == expert_id) { // Difficulty 3: 30*16 99 mines
        width = expert.width; 
        height = expert.height;
        mine_count = expert.mine_count; 
    }

    GameFrame *frame = new GameFrame("Minesweeper", wxPoint(550, 275), wxSize(width*20, height*20+55));
    frame->Show();
}

void GameFrame::LoadBitmaps() {
    empty_bmp.LoadFile("Images/0.png", wxBITMAP_TYPE_PNG);
    one_bmp.LoadFile("Images/1.png", wxBITMAP_TYPE_PNG);
    two_bmp.LoadFile("Images/2.png", wxBITMAP_TYPE_PNG);
    three_bmp.LoadFile("Images/3.png", wxBITMAP_TYPE_PNG);
    four_bmp.LoadFile("Images/4.png", wxBITMAP_TYPE_PNG);
    five_bmp.LoadFile("Images/5.png", wxBITMAP_TYPE_PNG);
    six_bmp.LoadFile("Images/6.png", wxBITMAP_TYPE_PNG);
    seven_bmp.LoadFile("Images/7.png", wxBITMAP_TYPE_PNG);
    eight_bmp.LoadFile("Images/8.png", wxBITMAP_TYPE_PNG);
    down_bmp.LoadFile("Images/down.png", wxBITMAP_TYPE_PNG);
    flag_bmp.LoadFile("Images/flag.png", wxBITMAP_TYPE_PNG);
    hit_bmp.LoadFile("Images/hit.png", wxBITMAP_TYPE_PNG);
    mine_bmp.LoadFile("Images/mine.png", wxBITMAP_TYPE_PNG);
    questionmark_bmp.LoadFile("Images/questionmark.png", wxBITMAP_TYPE_PNG);
    wrong_bmp.LoadFile("Images/wrong.png", wxBITMAP_TYPE_PNG);
}

void GameFrame::GenerateButtons() // Create a field of buttons
{
    int button_id = 0;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            
            tile *new_tile = new tile;
            new_tile->tile_type = empty;
            new_tile->hidden_kind = covered;
            
            new_tile->button = new wxBitmapButton (this, button_id, down_bmp, wxPoint(i*20, j*20), wxSize(20,20));
            new_tile->button->Bind(wxEVT_RIGHT_DOWN, &GameFrame::OnRightDown, this);
            new_tile->button->Bind(wxEVT_LEFT_DOWN, &GameFrame::OnLeftDown, this);

            field[i][j] = new_tile;
            button_id++;
        }
    }
}

void GameFrame::GenerateMines(int x, int y) // Generate random mines
{
    int mine_x;
    int mine_y;

    srand(time(NULL));

    for (int k = 0; k < mine_count; k++) {
        do {
          mine_x = rand() % width;
          mine_y = rand() % height;
        }
        // In case the there already is a mine or it was hit on the first click
        while (field[mine_x][mine_y]->tile_type == mine || (mine_x == x && mine_y == y));
        
        field[mine_x][mine_y]->tile_type = mine;
        CountAdjecentMines(mine_x, mine_y);
    }
}

void GameFrame::CountAdjecentMines(int x, int y) // Add one to all eight surrounding tiles
{
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            if (ValidTile(x-1+i, y-1+j) && field[x-1+i][y-1+j]->tile_type != mine && !(i == 1 && j == 1)) {
                field[x-1+i][y-1+j]->tile_type++;
            }       
        }
    }
}

void GameFrame::OnLeftDown(wxMouseEvent& event) // Uncover the tile and react appropriately
{
    int id = event.GetId();
    int x = id/height;
    int y = id%height;

    if (shown_tiles == 0) {
        timer.Start(1000);
        GenerateMines(x, y);
    }
    
    wxBitmapButton *button = reinterpret_cast <wxBitmapButton*> (event.GetEventObject());

    if (field[x][y]->hidden_kind != covered && field[x][y]->hidden_kind != questionmark) return;
    if (field[x][y]->tile_type != 0) {
        shown_tiles++;
        field[x][y]->hidden_kind = uncovered;
    }

    switch (field[x][y]->tile_type) {   
    case empty: // Recursively show surronding empty tiles
        ShowZeros(x,y);
        break;
    
    case mine: // Explode
        button->SetBitmap(hit_bmp);
        SetStatusText("Good luck next time!");
        timer.Stop();
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) { // Show remaining mines
                if ((field[i][j]->hidden_kind == covered || field[i][j]->hidden_kind == questionmark) && field[i][j]->tile_type == mine) { 
                field[i][j]->button->SetBitmap(mine_bmp);
                }
                else if (field[i][j]->hidden_kind == flag && field[i][j]->tile_type != mine) { // Incorrect flag
                    field[i][j]->button->SetBitmap(wrong_bmp);
                }
                field[i][j]->button->Disable(); // Deactivate all buttons
            }
        }
        break;

    default:
        field[x][y]->button->SetBitmap(ReturnNumber(field[x][y]->tile_type)); 
    }

    if (field[x][y]->tile_type != mine) {
        GameWon();
    }
}

void GameFrame::OnRightDown(wxMouseEvent& event) // Swich between a flag, a questionmark and a neutral
{
    int id = event.GetId();
    int x = id/height;
    int y = id%height;

    if (field[x][y]->hidden_kind == covered) {
        field[x][y]->hidden_kind = flag;
        field[x][y]->button->SetBitmap(flag_bmp);
        flagged++;
        }

    else if (field[x][y]->hidden_kind == flag) {
        field[x][y]->hidden_kind = questionmark;
        field[x][y]->button->SetBitmap(questionmark_bmp);
        flagged--;
        }

    else if (field[x][y]->hidden_kind == questionmark) {
        field[x][y]->hidden_kind = covered;
        field[x][y]->button->SetBitmap(down_bmp);
    }
}

void GameFrame::ShowZeros(int x, int y) // Uncover all surrounding empty tiles
{
    if (!ValidTile(x, y) || !(field[x][y]->hidden_kind == covered || field[x][y]->hidden_kind == questionmark)) return;
    field[x][y]->hidden_kind = uncovered;
    shown_tiles++;

    if (field[x][y]->tile_type == empty) {
        field[x][y]->button->SetBitmap(empty_bmp);
        for (int i = 0; i <= 2; i++) {
            for (int j = 0; j <= 2; j++) {
                if (!(i == 1 && j == 1)) {
                    ShowZeros(x-1+i, y-1+j);
                }
            }
        }
    }

    else if (field[x][y]->tile_type != mine) {
       field[x][y]->button->SetBitmap(ReturnNumber(field[x][y]->tile_type)); 
    }
}

wxBitmap GameFrame::ReturnNumber(int type) {
    
    switch (type) {
    case 1:
        return one_bmp;
        break;
    case 2:
        return two_bmp;
        break;
    case 3:
        return three_bmp;
        break;
    case 4:
        return four_bmp;
        break;
    case 5:
        return five_bmp;
        break;
    case 6:
        return six_bmp;
        break;
    case 7:
        return seven_bmp;
        break;
    case 8:
        return eight_bmp;
        break;
    default:
        return empty_bmp;
    }
}

bool GameFrame::ValidTile(int x, int y) // Checks if the coordinates exist in the field
{
    return (x >= 0) && (x < width) && (y >= 0) && (y < height);
}

void GameFrame::GameWon() // Flag all remaining mines and disable buttons
{
    if (shown_tiles != (width)*(height) - mine_count) return; // Only mines remained hidden
    timer.Stop();
    string time = to_string(seconds/60) + ":" + to_string(seconds%60);
    SetStatusText("You won!\t\t Time: " + time);
    SaveScore();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (field[i][j]->tile_type == 10) {
                field[i][j]->button->SetBitmap(wxBitmap(("Images/flag.png"), wxBITMAP_TYPE_PNG));
            }
            field[i][j]->button->Disable();
        }
    }
    
}

void GameFrame::SaveScore() // Save the score if it is one of the top five
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

void GameFrame::OnExit(wxCommandEvent &)
{
    Close(true);
}

void GameFrame::OnTimer(wxTimerEvent &) // Update time and remaining mines each second
{
    seconds++;
    string output = "Time: " + to_string(seconds/60) + ":" + to_string(seconds%60) + "\t  Mines: " + to_string(mine_count-flagged);
    SetStatusText(output);
}