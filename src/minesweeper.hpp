#ifndef MINESWEEPER_HPP_
#define MINESWEEPER_HPP_

// Standard libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// wxWidgwets libraries
#include <wx/wx.h>
#include <wx/button.h>
#include <wx/event.h>
#include <wx/timer.h>

using namespace std;

struct tile 
{  
    int type; // 10 = mine, otherwise the number of surrounding mines
    int kind; // 0 = down, 1 = flag, 2 = questionmark, 3 = uncovered
    wxBitmapButton *button;
};

struct difficulty
{
    int width;
    int height;
    int mine_count;
};

int shown_tiles = 0;
int seconds = 0;
int flagged = 0;

int width;
int height;
int mine_count;

class MyFrame : public wxFrame
{
    public:
        tile *field[30][16];
        MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
        void generateButtons();
        int generateMines(int mine_count, int x, int y);
        void countAdjecentMines(int x, int y);
        bool validTile(int row, int col);
        void showZeros(int x, int y);
        void gameWon();
        void OnTimer(wxTimerEvent& event);
        void saveScore();
    
    private:
        wxPanel* new_board;
        wxTimer m_timer;
        void onExit(wxCommandEvent &event);
        void OnLeftDown(wxMouseEvent&);
        void OnRightDown(wxMouseEvent&);
        void Beginner(wxEvent& event);
        void Intermediate(wxEvent& event);
        void Expert(wxEvent& event);

    wxDECLARE_EVENT_TABLE();
};

class MyApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif
