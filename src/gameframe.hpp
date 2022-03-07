#ifndef GAMEFRAME_HPP_
#define GAMEFRAME_HPP_

#include "minesweeper.hpp"
#include <iostream>
#include <wx/wx.h>
#include <wx/button.h>
#include <wx/event.h>
#include <wx/timer.h>

using namespace std;

class GameFrame : public wxFrame
{
    public:
        GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size, int dif);

    private:
        Minesweeper M; // Object of the game
        wxTimer timer;
        wxBitmapButton *buttons[30][16];
        wxBitmap under_type_bmp[12];
        wxBitmap above_kind_bmp[4];

        void saveScore();
        void loadBitmaps();
        void createButtons();
        void chooseDifficulty(wxEvent& event);

        // wxWidgets functions
        void OnTimer(wxTimerEvent &);
        void OnExit(wxCommandEvent &);
        void OnLeftDown(wxMouseEvent &);
        void OnRightDown(wxMouseEvent &);

        wxDECLARE_EVENT_TABLE();
};

#endif
