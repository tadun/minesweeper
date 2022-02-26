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
        wxBitmapButton *buttons[30][16];
        GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
        ~GameFrame();
        void LoadBitmaps();
        void CreateButtons(int width, int height);
        void OnTimer(wxTimerEvent &);
        wxBitmap  ReturnNumber(int type);
        Minesweeper M;
    
    private:
        wxPanel *new_board;
        wxTimer timer;
        int seconds;
        int width;
        int height;
        void OnExit(wxCommandEvent &);
        void OnLeftDown(wxMouseEvent &);
        void OnRightDown(wxMouseEvent &);

        wxBitmap type_bmp[12];
        wxBitmap kind_bmp[4];

    wxDECLARE_EVENT_TABLE();
};

class App : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif
