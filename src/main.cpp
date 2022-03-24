#include "main.hpp"

using namespace std;

wxIMPLEMENT_APP(App); // Used instead of main

    wxBEGIN_EVENT_TABLE(GameFrame, wxFrame) // Creates a table that handles events based on their ID
        EVT_TIMER(5, GameFrame::OnTimer) 
        EVT_MENU(wxID_EXIT, GameFrame::OnExit)
    wxEND_EVENT_TABLE();

bool App::OnInit()
{
    ::wxInitAllImageHandlers(); // Necessary for loading bitmaps

    MenuFrame *menu = new MenuFrame("Menu", wxPoint(550, 275), wxSize(150, 140)); // Size is adjusted once the butttons are created
    menu->Show();

    return true;
}
