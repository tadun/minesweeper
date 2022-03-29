// Minesweeper using wxWidgets by Tadeas Horn for MacOS

#include "gameframe.hpp"
#include "minesweeper.hpp"

enum id_options // IDs for wxEvents
{   
    beginner_id = 1,
    intermediate_id = 2,
    expert_id = 3,
    new_game_id = 4,
    timer_id = 5
};

// Create a new window with given parameters
GameFrame::GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size, int dif) 
    : wxFrame(NULL, wxID_ANY, title, pos, size),
    timer(this, timer_id)
{
    seconds = 0; // Reset timer
    M.selectDifficulty(dif);
    
    loadTheBestScore();
    CreateStatusBar();
    SetStatusText(generateOutput());
    createMenu();

    loadBitmaps();
    createButtons();
    Fit(); // Adjust the window size to its children
}

// Load the best score for this difficulty
void GameFrame::loadTheBestScore() 
{
    
    string input, name;
    top_score = 0;
    // Finding out the folder of the binary
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());
    name = appPath + "/scores/" + to_string(M.getDifChoice()) + ".txt";
    fstream file(name, ios::in);
    
    while (file >> input) {
        top_score = stoi(input);
    }

    file.close();
}

// Returns a string with time, ramining mines and best score
string GameFrame::generateOutput() 
{
    string output = "Time: " + to_string(seconds/60) + ":" + to_string(seconds%60) 
    + "  Mines: " + to_string(M.getMineCount()-M.getFlagged());

    if (top_score != 0) {
        output += "  Best: " + to_string(top_score/60) + ":" + to_string(top_score%60);
    }

    return output;
}

// Creating the difficulty menu
void GameFrame::createMenu()
{
    wxMenu *menu_difficulty = new wxMenu;
    menu_difficulty->Append(beginner_id, "Beginner");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, beginner_id);
    menu_difficulty->Append(intermediate_id, "Intermediate");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, intermediate_id);
    menu_difficulty->Append(expert_id, "Expert");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, expert_id);
    menu_difficulty->AppendSeparator();
    menu_difficulty->Append(wxID_EXIT);

    wxMenu *menu_game = new wxMenu;
    menu_game->Append(new_game_id, "New Game");
    Bind(wxEVT_COMMAND_MENU_SELECTED, &GameFrame::chooseDifficulty, this, new_game_id);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menu_game, "Game");
    menuBar->Append(menu_difficulty, "Difficulty");
    SetMenuBar(menuBar);
}

// Save all images into an array
void GameFrame::loadBitmaps() 
{
    string path;
    int i;

    // Getting the folder of the binary
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());

    for (i = 0; i <= 14; i++) { // Load or bitmaps
        path = appPath + "/images/" + to_string(i) + ".png";
        bitmaps[i].LoadFile(path, wxBITMAP_TYPE_PNG);
    }
}

// Create the propoper number of buttons a bind them to left and right click
void GameFrame::createButtons() 
{
    int button_id = 0;

    for (int i = 0; i < M.getWidth(); i++) {
        for (int j = 0; j < M.getHeight(); j++) {
            wxBitmapButton *button = new wxBitmapButton(this, button_id, bitmaps[covered], wxPoint(i*20, j*20), wxSize(20,20));
            button->Bind(wxEVT_LEFT_DOWN, &GameFrame::OnLeftDown, this);
            button->Bind(wxEVT_RIGHT_DOWN, &GameFrame::OnRightDown, this);
            buttons[i][j] = button;
            button_id++;
        }
    }
}

// Show all uncovered tiles and disable if true
void GameFrame::showUncovered(bool disable) 
{
    for (int i = 0; i < M.getWidth(); i++) {
        for (int j = 0; j < M.getHeight(); j++) {
            if (M.field[i][j]->hidden_kind == uncovered) {
                buttons[i][j]->SetBitmap(bitmaps[M.field[i][j]->number]);
            }
            if (disable) {buttons[i][j]->Disable();}
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
    result output;

    if (M.getShownTiles() == 0) {timer.Start(1000);} // Start the time
    
    output = M.gameLogic(x, y);
    if (output != incorrect) {buttons[x][y]->SetBitmap(bitmaps[M.field[x][y]->number]);}

    switch (output) {
    case zero:
        showUncovered(false);
        break;

    case win:
        timer.Stop();
        saveScore();
        text = "You won!  Time: " + to_string(seconds/60) + ":" + to_string(seconds%60);
        if (seconds < top_score) {text += "  New best!";}
        SetStatusText(text); // Show output
        showUncovered(true); // Flag mines
        break;

    case loss:
        timer.Stop();
        SetStatusText("Good luck next time!");
        showUncovered(true); // Show remaining mines
        buttons[x][y]->SetBitmap(bitmaps[hit]);
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
        buttons[x][y]->SetBitmap(bitmaps[M.field[x][y]->hidden_kind]);
    }
}

// Create a new game window, when seleced from the menu
void GameFrame::chooseDifficulty(wxEvent& event) 
{
    int id = event.GetId();
    M.selectDifficulty(id);

    GameFrame *frame = new GameFrame("Minesweeper", wxPoint(550, 275), wxSize(M.getWidth()*20, M.getHeight()*20+55), M.getDifChoice());
    frame->Show();

    Close(true); // Close the old game
}

// Save the score if it is the new best
void GameFrame::saveScore() 
{
    fstream file;
    // Getting the folder of the binary
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());
    string name = "/scores/" + to_string(M.getDifChoice()) + ".txt"; // File matching the difficulty
    file.open(appPath + name, ios::out);

    if (seconds < top_score || top_score == 0) {
        file << seconds;
    }

    else {
        file << top_score;
    }

    file.close();
}

// Update time and remaining mines each second
void GameFrame::OnTimer(wxTimerEvent &) 
{
    seconds++;
    SetStatusText(generateOutput());
}
