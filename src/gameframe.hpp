#ifndef GAMEFRAME_HPP_
#define GAMEFRAME_HPP_

// Standard libraries
#include <iostream>

// wxWidgwets libraries
#include <wx/wx.h>
#include <wx/button.h>
#include <wx/event.h>
#include <wx/timer.h>

using namespace std;

class GameFrame : public wxFrame
{
    public:
        GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
        ~GameFrame();
        void LoadBitmaps();
        void OnTimer(wxTimerEvent &);
        wxBitmap  ReturnNumber(int type);
    
    private:
        wxPanel *new_board;
        wxTimer timer;
        int seconds;
        void OnExit(wxCommandEvent &);
        void OnLeftDown(wxMouseEvent &);
        void OnRightDown(wxMouseEvent &);

        wxBitmap empty_bmp;
        wxBitmap one_bmp;
        wxBitmap two_bmp;
        wxBitmap three_bmp;
        wxBitmap four_bmp;
        wxBitmap five_bmp;
        wxBitmap six_bmp;
        wxBitmap seven_bmp;
        wxBitmap eight_bmp;
        wxBitmap down_bmp;
        wxBitmap flag_bmp;
        wxBitmap hit_bmp;
        wxBitmap mine_bmp;
        wxBitmap questionmark_bmp;
        wxBitmap wrong_bmp;

    wxDECLARE_EVENT_TABLE();
};

class App : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif
