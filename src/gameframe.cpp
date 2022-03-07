#include "gameframe.hpp"
#include "minesweeper.hpp"

// type kind
// generate mines
// On left down

enum id_options 
{   
    beginner_id = 1,
    intermediate_id = 2,
    expert_id = 3,
    timer_id = 4
};

GameFrame::GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size, int dif) 
    : wxFrame(NULL, wxID_ANY, title, pos, size),
    timer(this, timer_id)
{
    M.selectDifficulty(dif);
    //M.generateField();
    
    // Setting up the status bar
    CreateStatusBar();
    M.seconds = 0;
    string output = "Time: " + to_string(M.seconds/60) + ":" + to_string(M.seconds%60) 
    + "   Mines: " + to_string(M.getMineCount()-M.getFlagged());

    if (M.getTop() != 0) {
        output += "   Best: " + to_string(M.getTop()/60) + ":" + to_string(M.getTop()%60);
    }
    
    SetStatusText(output);

    // Creating the difficulty menu
    wxMenu *menu_game = new wxMenu;
    menu_game->Append(1, "&Beginner");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, beginner_id);
    menu_game->Append(2, "&Intermediate");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, intermediate_id);
    menu_game->Append(3, "&Expert");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, expert_id);
    menu_game->AppendSeparator();
    menu_game->Append(wxID_EXIT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu_game, "&New Game");
    SetMenuBar(menuBar);

    loadBitmaps();
    createButtons();
    Fit();
}

void GameFrame::loadBitmaps() 
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

void GameFrame::createButtons() 
{
    int button_id = 0;

    for (int i = 0; i < M.getWidth(); i++) {
        for (int j = 0; j < M.getHeight(); j++) {
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
    int x = id/M.getHeight();
    int y = id%M.getHeight();
    string time;

    if (M.getShownTiles() == 0) {
        timer.Start(1000);
        M.generateMines(x, y);
    }

    result output = M.gameLogic(x, y);

    if (M.field[x][y]->tile_type == number) {
        buttons[x][y]->SetBitmap(type_bmp[M.field[x][y]->number]);
    }
    else {
        buttons[x][y]->SetBitmap(type_bmp[M.field[x][y]->tile_type]);
    }
   
    switch (output) {
    case zero:
        for (int i = 0; i < M.getWidth(); i++) {
            for (int j = 0; j < M.getHeight(); j++) {
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
        break;

    case win:
        timer.Stop();
        time = to_string(M.seconds/60) + ":" + to_string(M.seconds%60);
        
        SetStatusText("You won!  Time: " + time);

        if (M.seconds < M.getTop()) {
            SetStatusText("You won!  Time: " + time + "  New Best!");
        }
        
        for (int i = 0; i < M.getWidth(); i++) {
            for (int j = 0; j < M.getHeight(); j++) {
                if (M.field[i][j]->tile_type == mine) {
                    buttons[i][j]->SetBitmap(kind_bmp[flag]);
                }
                buttons[i][j]->Disable();
            }
        }
        break;

    case lost:
        SetStatusText("Good luck next time!");
        timer.Stop();

        for (int i = 0; i < M.getWidth(); i++) {
            for (int j = 0; j < M.getWidth(); j++) { // Show remaining mines
                if (M.field[i][j]->tile_type == mine && M.field[i][j]->hidden_kind != flag) {
                    buttons[i][j]->SetBitmap(type_bmp[M.field[i][j]->tile_type]);
                }
                if (M.field[i][j]->tile_type == wrong) { // Incorrect flag
                    buttons[i][j]->SetBitmap(type_bmp[wrong]);
                }
                buttons[i][j]->Disable(); // Deactivate all buttons
            }
        }
        buttons[x][y]->SetBitmap(type_bmp[hit]);
    break;
    default:
    break;
    }
}

void GameFrame::OnRightDown(wxMouseEvent& event) // Swich between a flag, a questionmark and a neutral
{
    int id = event.GetId();
    int x = id/M.getHeight();
    int y = id%M.getHeight();

    M.changeKind(x, y);

    if (M.field[x][y]->hidden_kind != 3) {
        buttons[x][y]->SetBitmap(kind_bmp[M.field[x][y]->hidden_kind]);
    }
}

void GameFrame::chooseDifficulty(wxEvent& event) 
{
    int id = event.GetId();
    M.seconds = 0;
    M.selectDifficulty(id);

    GameFrame *frame = new GameFrame("Minesweeper", wxPoint(550, 275), wxSize(M.getWidth()*20, M.getHeight()*20+55), id);
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
    string output = "Time: " + to_string(M.seconds/60) + ":" + to_string(M.seconds%60) 
    + "   Mines: " + to_string(M.getMineCount()-M.getFlagged());

    if (M.getTop() != 0) {
        output += "   Best: " + to_string(M.getTop()/60) + ":" + to_string(M.getTop()%60);
    }

    SetStatusText(output);
}
