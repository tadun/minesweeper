#ifndef GAMEFRAME_HPP_
#define GAMEFRAME_HPP_

// Standard libraries
#include <iostream>
#include "minesweeper.hpp"

// wxWidgwets libraries
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
        Minesweeper M;
        wxPanel *new_board;
        wxTimer timer;
        wxBitmapButton *buttons[30][16];
        wxBitmap type_bmp[12];
        wxBitmap kind_bmp[4];
        int width;
        int height;
        int mine_count;

        void ChooseDifficulty(wxEvent& event);
        void LoadBitmaps();
        void CreateButtons(int width, int height);
        void OnTimer(wxTimerEvent &);
        void OnExit(wxCommandEvent &);
        void OnLeftDown(wxMouseEvent &);
        void OnRightDown(wxMouseEvent &);

    wxDECLARE_EVENT_TABLE();
};

#endif
