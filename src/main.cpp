#include "main.hpp"

using namespace std;

wxIMPLEMENT_APP(App);

    wxBEGIN_EVENT_TABLE(GameFrame, wxFrame)
        EVT_TIMER(4, GameFrame::OnTimer)
        EVT_MENU(wxID_EXIT, GameFrame::OnExit)
    wxEND_EVENT_TABLE();

bool App::OnInit()
{
    ::wxInitAllImageHandlers();

    MenuFrame *menu = new MenuFrame("Menu", wxPoint(550, 275), wxSize(150, 140));
    menu->Show();

    return true;
}
