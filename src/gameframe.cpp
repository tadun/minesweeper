#include "gameframe.hpp"

// saving score
// create a class for the game
// global variables
// memory leak

// udelej ze me arrray

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
    // string output = "Time: " + to_string(seconds/60) + ":" + to_string(seconds%60) 
    //     + "\t  Mines: " + to_string(mine_count-flagged);
    //SetStatusText(output);
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

    LoadBitmaps();
}

GameFrame::~GameFrame() {
    // for (int i = 0; i < width; i++) {
    //     for (int j = 0; j < height; j++) {
    //         field[i][j] = NULL;
    //     }
    // }
}

bool App::OnInit() // Define frame size
{
    ::wxInitAllImageHandlers();

    GameFrame *frame = new GameFrame("Minesweeper", wxPoint(550, 275), wxSize(300, 300/*beginner.width*20, beginner.height*20+55*/));
    frame->Show();
    return true;
}

void GameFrame::LoadBitmaps() 
{
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

void GameFrame::OnLeftDown(wxMouseEvent& event) // Uncover the tile and react appropriately
{
    int id = event.GetId();
    int x = id/*/height*/;
    int y = id/*%height*/;

    // if (shown_tiles == 0) {
    //     timer.Start(1000);
    // }
    
    wxBitmapButton *button = reinterpret_cast <wxBitmapButton*> (event.GetEventObject());

}

void GameFrame::OnRightDown(wxMouseEvent& event) // Swich between a flag, a questionmark and a neutral
{
    int id = event.GetId();
    int x = id/*/height*/;
    int y = id/*%height*/;
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
