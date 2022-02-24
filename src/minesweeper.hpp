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
    int tile_type;
    int hidden_kind;
    wxBitmapButton *button;
};

struct difficulty
{
    int width;
    int height;
    int mine_count;
};

class GameFrame : public wxFrame
{
    public:
        tile *field[30][16];
        GameFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
        ~GameFrame();
        void LoadBitmaps();
        void GenerateButtons();
        void GenerateMines(int x, int y);
        void CountAdjecentMines(int x, int y);
        bool ValidTile(int x, int y);
        void ShowZeros(int x, int y);
        void GameWon();
        void OnTimer(wxTimerEvent &);
        void SaveScore();
        wxBitmap  ReturnNumber(int type);
        
    
    private:
        wxPanel *new_board;
        wxTimer timer;
        void OnExit(wxCommandEvent &);
        void OnLeftDown(wxMouseEvent &);
        void OnRightDown(wxMouseEvent &);
        void SelectDifficulty(wxEvent &event);

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
