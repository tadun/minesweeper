#include "gameframe.hpp"
#include "minesweeper.hpp"

// saving score
// memory leak

enum id_options 
{   beginner_id = 1,
    intermediate_id = 2,
    expert_id = 3,
    timer_id = 4
};

// enum tile_type_options {
//     down = 9,
//     mine = 10,
//     flag = 11,
//     questionmark = 12,
//     hit = 13,
//     wrong = 14
// };

GameFrame::GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size) 
    : wxFrame(NULL, wxID_ANY, title, pos, size),
    timer(this, timer_id)
{
    // Setting up the status bar
    // CreateStatusBar();
    // string output = "Time: " + to_string(seconds/60) + ":" + to_string(seconds%60) 
    //     + "\t  Mines: " + to_string(mine_count-flagged);
    // SetStatusText(output);
    new_board = new wxPanel(this, wxID_ANY);

    // Creating the difficulty menu
    wxMenu *menu_game = new wxMenu;
    menu_game->Append(1, "&Beginner");
    //Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::SelectDifficulty, this, beginner_id);
    menu_game->Append(2, "&Intermediate");
    //Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::SelectDifficulty, this, intermediate_id);
    menu_game->Append(3, "&Expert");
    //Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::SelectDifficulty, this, intermediate_id);
    menu_game->AppendSeparator();
    menu_game->Append(wxID_EXIT);

    M.SelectDifficulty(1);
    M.GenerateField();
    M.GenerateMines();

    width = M.GetWidth();
    height = M.GetHeight();
    int mine_count = M.GetMineCount();

    LoadBitmaps();
    CreateButtons(width, height);
}

GameFrame::~GameFrame() 
{
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            M.field[i][j] = NULL;
            buttons[i][j] = NULL;
        }
    }
}

void GameFrame::LoadBitmaps() 
{
    string path;

    int i = 0;
    for (i; i <= 12; i++) {
        path = "Images/" + to_string(i) + ".png";
        type_bmp[i].LoadFile(path, wxBITMAP_TYPE_PNG);
    }

    for (i; i <= 14; i++) {
        path = "Images/" + to_string(i) + ".png";
        type_bmp[i].LoadFile(path, wxBITMAP_TYPE_PNG);
    }
}

void GameFrame::CreateButtons(int width, int height) 
{
    int button_id = 0;
    
    // wxBitmapButton *button1 = new wxBitmapButton (this, button_id, kind_bmp[covered], wxPoint(0, 0), wxSize(20,20));
    // wxBitmapButton *button2 = new wxBitmapButton (this, button_id, kind_bmp[flag], wxPoint(0, 20), wxSize(20,20));
    // wxBitmapButton *button3 = new wxBitmapButton (this, button_id, kind_bmp[questionmark], wxPoint(0, 40), wxSize(20,20));
    // wxBitmapButton *button4 = new wxBitmapButton (this, button_id, type_bmp[0], wxPoint(0, 60), wxSize(20,20));
    // wxBitmapButton *button5 = new wxBitmapButton (this, button_id, type_bmp[1], wxPoint(0, 80), wxSize(20,20));
    // wxBitmapButton *button6 = new wxBitmapButton (this, button_id, type_bmp[2], wxPoint(0, 100), wxSize(20,20));
    // wxBitmapButton *button7 = new wxBitmapButton (this, button_id, type_bmp[3], wxPoint(0, 120), wxSize(20,20));
    // wxBitmapButton *button8 = new wxBitmapButton (this, button_id, type_bmp[4], wxPoint(0, 140), wxSize(20,20));
    // wxBitmapButton *button9 = new wxBitmapButton (this, button_id, type_bmp[5], wxPoint(0, 160), wxSize(20,20));
    // wxBitmapButton *button10 = new wxBitmapButton (this, button_id, type_bmp[6], wxPoint(0, 180), wxSize(20,20));
    // wxBitmapButton *button11 = new wxBitmapButton (this, button_id, type_bmp[7], wxPoint(20, 0), wxSize(20,20));
    // wxBitmapButton *button12 = new wxBitmapButton (this, button_id, type_bmp[8], wxPoint(20, 20), wxSize(20,20));
    // wxBitmapButton *button13 = new wxBitmapButton (this, button_id, type_bmp[hit], wxPoint(20, 40), wxSize(20,20));
    // wxBitmapButton *button14 = new wxBitmapButton (this, button_id, type_bmp[mine], wxPoint(20, 60), wxSize(20,20));
    // wxBitmapButton *button15 = new wxBitmapButton (this, button_id, type_bmp[wrong], wxPoint(20, 80), wxSize(20,20));

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            wxBitmapButton *button = new wxBitmapButton (this, button_id, kind_bmp[covered], wxPoint(i*20, j*20), wxSize(20,20));
            button->Bind(wxEVT_RIGHT_DOWN, &GameFrame::OnRightDown, this);
            button->Bind(wxEVT_LEFT_DOWN, &GameFrame::OnLeftDown, this);
            buttons[i][j] = button;
            button_id++;
        }
    }
}

void GameFrame::OnLeftDown(wxMouseEvent& event) // Uncover the tile and react appropriately
{
    int id = event.GetId();
    int x = id/height;
    int y = id%height;

    // if (shown_tiles == 0) {
    //     timer.Start(1000);
    // }
    
    M.Game(x, y);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (M.field[i][j]->hidden_kind == uncovered) {
                if (M.field[i][j]->tile_type == number) {
                     buttons[i][j]->SetBitmap(type_bmp[M.field[i][j]->number]);
                }
                else {
                    buttons[i][j]->SetBitmap(type_bmp[M.field[i][j]->tile_type]);
                }
            }
        }
    }

    if (M.field[x][y]->tile_type == hit) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                buttons[i][j]->Disable();
            }
        }                 
    }
}

void GameFrame::OnRightDown(wxMouseEvent& event) // Swich between a flag, a questionmark and a neutral
{
    int id = event.GetId();
    int x = id/height;
    int y = id%height;

    M.ChangeKind(x, y);

    buttons[x][y]->SetBitmap(kind_bmp[M.field[x][y]->hidden_kind]);
}

void GameFrame::OnExit(wxCommandEvent &)
{
    Close(true);
}

void GameFrame::OnTimer(wxTimerEvent &) // Update time and remaining mines each second
{
    seconds++;
    string output = "Time: " + to_string(seconds/60) + ":" + to_string(seconds%60) + "\t  Mines: "/* + to_string(mine_count-flagged)*/;
    SetStatusText(output);
}
