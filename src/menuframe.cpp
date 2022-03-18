#include "menuframe.hpp"
#include "gameframe.hpp"

// Create a new window with given parameters
MenuFrame::MenuFrame(const wxString &title, const wxPoint &pos, const wxSize &size) 
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxButton *button_1 = new wxButton(this, 1, "Beginner", wxPoint(0, 0), wxSize(150, 20));
    button_1->Bind(wxEVT_LEFT_DOWN, &MenuFrame::selectedOption, this);
    wxButton *button_2 = new wxButton(this, 2, "Intermediate", wxPoint(0, 30), wxSize(150, 20));
    button_2->Bind(wxEVT_LEFT_DOWN, &MenuFrame::selectedOption, this);
    wxButton *button_3 = new wxButton(this, 3, "Expert", wxPoint(0, 60), wxSize(150, 20));
    button_3->Bind(wxEVT_LEFT_DOWN, &MenuFrame::selectedOption, this);

    Fit();
}

// Start the game with a given difficulty
void MenuFrame::selectedOption(wxMouseEvent& event)
{
    int choice = event.GetId();

    GameFrame *frame = new GameFrame("Minesweeper", wxPoint(550, 275), wxSize(300, 300), choice);
    frame->Show();

    Close(true);
}

