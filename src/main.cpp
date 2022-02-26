#include "minesweeper.hpp"
#include "gameframe.hpp"

#include <wx/wx.h>
#include <wx/button.h>
#include <wx/event.h>
#include <wx/timer.h>

using namespace std;

wxIMPLEMENT_APP(App);

    wxBEGIN_EVENT_TABLE(GameFrame, wxFrame)
        EVT_TIMER(1/*timer_id*/, GameFrame::OnTimer)
        EVT_MENU(wxID_EXIT, GameFrame::OnExit)
    wxEND_EVENT_TABLE();

bool App::OnInit()
{
    ::wxInitAllImageHandlers();
    
    GameFrame *frame = new GameFrame("Minesweeper", wxPoint(550, 275), wxSize(300, 300/*beginner.width*20, beginner.height*20+55*/));
    frame->Show();

    // Minesweeper M;
    // int x;
    // int y;

    // M.SelectDifficulty(1);
    // M.GenerateField();
    // M.GenerateMines();

    // while(true) {
    //     M.ShowField();
    
    //     cout << "Enter x and y: " << endl;
    //     cin >> x; 
    //     cin >> y;
    //     cout << endl;

    //     M.Game(x,y);
    // }

    return true;
}
