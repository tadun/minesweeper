#include "gameframe.hpp"
#include "minesweeper.hpp"
#include <cstdlib> 

// saving score
// memory leak

enum id_options 
{   beginner_id = 1,
    intermediate_id = 2,
    expert_id = 3,
    timer_id = 4
};

GameFrame::GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size, int dif) 
    : wxFrame(NULL, wxID_ANY, title, pos, size),
    timer(this, timer_id)
{
    M.SelectDifficulty(dif);
   
    width = M.GetWidth();
    height = M.GetHeight();
    mine_count = M.GetMineCount();
   
    M.GenerateField();

    // Setting up the status bar
    CreateStatusBar();
    string output = "Time: " + to_string(M.seconds/60) + ":" + to_string(M.seconds%60) + "\t  Mines: " + to_string(M.dif.mine_count-M.flagged);
    SetStatusText(output);
    new_board = new wxPanel(this, wxID_ANY);

    // Creating the difficulty menu
    wxMenu *menu_game = new wxMenu;
    menu_game->Append(1, "&Beginner");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::ChooseDifficulty, this, beginner_id);
    menu_game->Append(2, "&Intermediate");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::ChooseDifficulty, this, intermediate_id);
    menu_game->Append(3, "&Expert");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::ChooseDifficulty, this, expert_id);
    menu_game->AppendSeparator();
    menu_game->Append(wxID_EXIT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu_game, "&New Game");
    SetMenuBar(menuBar);

    LoadBitmaps();
    CreateButtons(width, height);
    Fit();
}

void GameFrame::LoadBitmaps() 
{
    string path;

    int i;
    for (i = 0; i <= 12; i++) {
        path = "Images/" + to_string(i) + ".png";
        type_bmp[i].LoadFile(path, wxBITMAP_TYPE_PNG);
    }

    for (i = 12; i <= 14; i++) {
        path = "Images/" + to_string(i) + ".png";
        type_bmp[i].LoadFile(path, wxBITMAP_TYPE_PNG);
    }
 }

void GameFrame::CreateButtons(int width, int height) 
{
    int button_id = 0;

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

    if (M.shown_tiles == 0) {
        timer.Start(1000);
        M.GenerateMines(x, y);
    }

    M.GameLogic(x, y);

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

    if (M.CheckWin()) {
        timer.Stop();
        string time = to_string(M.seconds/60) + ":" + to_string(M.seconds%60);
        SetStatusText("You won!\t Time: " + time);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                buttons[i][j]->Disable();
            }
        }
        //SaveScore();
    }

    if (M.field[x][y]->tile_type == hit) {
        SetStatusText("Good luck next time!");
        timer.Stop();
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

    if (M.field[x][y]->hidden_kind != 3) {
        buttons[x][y]->SetBitmap(kind_bmp[M.field[x][y]->hidden_kind]);
    }
    
}

void GameFrame::ChooseDifficulty(wxEvent& event) {
    int id = event.GetId();

    M.SelectDifficulty(id);

    GameFrame *frame = new GameFrame("Minesweeper", wxPoint(550, 275), wxSize(M.dif.width*20, M.dif.height*20+55), id);
    frame->Show();

    Close(true);

}

void GameFrame::OnExit(wxCommandEvent &)
{
    Close(true);
}

void GameFrame::OnTimer(wxTimerEvent &) // Update time and remaining mines each second
{
    M.seconds++;
    string output = "Time: " + to_string(M.seconds/60) + ":" + to_string(M.seconds%60) + "\t  Mines: " + to_string(mine_count-M.flagged);
    SetStatusText(output);
}
