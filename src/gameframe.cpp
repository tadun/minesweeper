// Minesweeper using wxWidgets by Tadeas Horn

#include "gameframe.hpp"
#include "minesweeper.hpp"

enum id_options // IDs for wxEvents
{   
    beginner_id = 1,
    intermediate_id = 2,
    expert_id = 3,
    timer_id = 4
};

// Create a new window with given parameters
GameFrame::GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size, int dif) 
    : wxFrame(NULL, wxID_ANY, title, pos, size),
    timer(this, timer_id)
{
    M.selectDifficulty(dif);
    
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
    menu_game->Append(1, "Beginner");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, beginner_id);
    menu_game->Append(2, "Intermediate");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, intermediate_id);
    menu_game->Append(3, "Expert");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, expert_id);
    menu_game->AppendSeparator();
    menu_game->Append(wxID_EXIT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu_game, "New Game");
    SetMenuBar(menuBar);

    loadBitmaps();
    createButtons();
    Fit(); // Adjust the window size to its children
}

// Save all images into arrays
void GameFrame::loadBitmaps() 
{
    string path;
    int i;

    for (i = 0; i <= 12; i++) { // Options for uncovered
        path = "Images/" + to_string(i) + ".png";
        under_type_bmp[i].LoadFile(path, wxBITMAP_TYPE_PNG);
    }

    for (i = 12; i <= 14; i++) { // Three kinds of covered
        path = "Images/" + to_string(i) + ".png";
        under_type_bmp[i].LoadFile(path, wxBITMAP_TYPE_PNG); // Are you serious??!
    }
}

// Create the propoper number of buttons a bind them to keys
void GameFrame::createButtons() 
{
    int button_id = 0;

    for (int i = 0; i < M.getWidth(); i++) {
        for (int j = 0; j < M.getHeight(); j++) {
            wxBitmapButton *button = new wxBitmapButton (this, button_id, above_kind_bmp[covered], wxPoint(i*20, j*20), wxSize(20,20));
            button->Bind(wxEVT_RIGHT_DOWN, &GameFrame::OnRightDown, this);
            button->Bind(wxEVT_LEFT_DOWN, &GameFrame::OnLeftDown, this);
            buttons[i][j] = button;
            button_id++;
        }
    }
}

// Uncover the tile and react appropriately
void GameFrame::OnLeftDown(wxMouseEvent& event)
{
    int id = event.GetId();
    int x = id/M.getHeight();
    int y = id%M.getHeight();

    string text;
    result output = M.gameLogic(x, y);

    if (M.getShownTiles() == 0) {timer.Start(1000);} // Start the time

    if (M.field[x][y]->tile_type == number) { // Regular number
        buttons[x][y]->SetBitmap(under_type_bmp[M.field[x][y]->number]);
    }

    switch (output) {
    case zero:
        for (int i = 0; i < M.getWidth(); i++) {
            for (int j = 0; j < M.getHeight(); j++) {
                if (M.field[i][j]->hidden_kind == uncovered) {
                    buttons[i][j]->SetBitmap(under_type_bmp[M.field[i][j]->number]);
                }
            }
        }
        break;

    case win:
        // Set status bar
        timer.Stop();
        text = "You won!  Time: " + to_string(M.seconds/60) + ":" + to_string(M.seconds%60);
        if (M.seconds < M.getTop()) {text += "  New best!";}
        SetStatusText(text);
        
        // Flag mines
        for (int i = 0; i < M.getWidth(); i++) {
            for (int j = 0; j < M.getHeight(); j++) {
                if (M.field[i][j]->tile_type == mine) {
                    buttons[i][j]->SetBitmap(above_kind_bmp[flag]);
                }
                buttons[i][j]->Disable(); // Deactivate all buttons
            }
        }
        break;

    case lost:
        timer.Stop();
        SetStatusText("Good luck next time!");

        for (int i = 0; i < M.getWidth(); i++) {
            for (int j = 0; j < M.getHeight(); j++) { // Show remaining mines
                if (M.field[i][j]->tile_type == mine && M.field[i][j]->hidden_kind != flag) {
                    buttons[i][j]->SetBitmap(under_type_bmp[M.field[i][j]->tile_type]);
                }
                if (M.field[i][j]->tile_type == wrong) { // Incorrect flag
                    buttons[i][j]->SetBitmap(under_type_bmp[wrong]);
                }
                buttons[i][j]->Disable(); // Deactivate all buttons
            }
        }
        buttons[x][y]->SetBitmap(under_type_bmp[hit]);
    break;
    default:
    break;
    }
}

// Swich between a flag, a questionmark and a neutral
void GameFrame::OnRightDown(wxMouseEvent& event)
{
    int id = event.GetId();
    int x = id/M.getHeight();
    int y = id%M.getHeight();

    M.changeKind(x, y);

    if (M.field[x][y]->hidden_kind != uncovered) {
        buttons[x][y]->SetBitmap(above_kind_bmp[M.field[x][y]->hidden_kind]);
    }
}

// Create a new game window, when seleced from the menu
void GameFrame::chooseDifficulty(wxEvent& event) 
{
    int id = event.GetId();
    M.seconds = 0; // Reset timer
    M.selectDifficulty(id);

    GameFrame *frame = new GameFrame("Minesweeper", wxPoint(550, 275), wxSize(M.getWidth()*20, M.getHeight()*20+55), id);
    frame->Show();

    Close(true); // Close the old game
}

void GameFrame::OnExit(wxCommandEvent &)
{
    Close(true);
}

// Update time and remaining mines each second
void GameFrame::OnTimer(wxTimerEvent &) {
    M.seconds++;
    string output = "Time: " + to_string(M.seconds/60) + ":" + to_string(M.seconds%60) 
    + "   Mines: " + to_string(M.getMineCount()-M.getFlagged());

    if (M.getTop() != 0) {
        output += "   Best: " + to_string(M.getTop()/60) + ":" + to_string(M.getTop()%60);
    }

    SetStatusText(output);
}
