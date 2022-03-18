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
        int seconds;
        int top_score;
        Minesweeper M; // Object of the game
        wxTimer timer;
        wxBitmapButton *buttons[30][16];
        wxBitmap bitmaps[15];

        string generateOutput();
        void loadTheBestScore();
        void createMenu();
        void loadBitmaps();
        void createButtons();
        void showUncovered(bool disable);
        void chooseDifficulty(wxEvent& event);
        void saveScore();

        // wxWidgets functions
        void OnTimer(wxTimerEvent &);
        void OnLeftDown(wxMouseEvent &);
        void OnRightDown(wxMouseEvent &);
        void OnExit(wxCommandEvent &) {Close(true);};

        wxDECLARE_EVENT_TABLE();
};

#endif
