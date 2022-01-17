#include <wx/wx.h>
#include <wx/button.h>
#include <iostream>
#include <fstream>

#define SIDE  9
#define MINES 10 

using namespace std;

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
    wxBitmapButton **btn;

private:
    void OnExit(wxCommandEvent &event);
    void OnPaint(wxPaintEvent& event);
    static void Event(wxEvent &);

        wxPanel* m_board;
        wxBitmap mine_image;
        wxBitmap empty_image;

    wxDECLARE_EVENT_TABLE();
};

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
    {
    CreateStatusBar();
    SetStatusText("A classic Minesweeper game!");

    m_board = new wxPanel(this, wxID_ANY);
    m_board->Bind(wxEVT_PAINT, &MyFrame::OnPaint, this);

    // Load the image.
    wxImage img1("Images/m.png", wxBITMAP_TYPE_PNG);
    wxBitmap a(img1);
    mine_image = a.GetSubBitmap(wxRect(0,0,20,20));

    wxImage img2("Images/0.png", wxBITMAP_TYPE_PNG);
    wxBitmap b(img2);
    empty_image = b.GetSubBitmap(wxRect(0,0,20,20));

    wxBitmap Bitmap;
    Bitmap.LoadFile("Images/d.png", wxBITMAP_TYPE_PNG);

    btn = new wxBitmapButton*[SIDE * SIDE];
    wxGridSizer *grid = new wxGridSizer(SIDE, SIDE, 0, 0);

    // for (int i=0; i<SIDE; i++) {
    //     for (int j=0; j<SIDE; j++){
    //         //wxBitmapButton* button = new wxBitmapButton (this, button_id, Bitmap, wxPoint(i*20, j*20), wxSize(20,20));
    //         btn[i*SIDE + j] = new wxBitmapButton(this, 10000 + i*SIDE + j, Bitmap);
    //         grid->Add(btn[i*SIDE + j], 1, wxEXPAND | wxALL);
    //         Bind(wxEVT_BUTTON, MyFrame::Event, 10000 + i*SIDE + j);

    //     }
    // }

    // this->SetSizer(grid);
    // grid->Layout();
    
    // int button_id = 0;
    // for (int i=0; i<9; i++) {
    //     for (int j=0; j<9; j++){
    //         wxBitmapButton* button = new wxBitmapButton (this, button_id, Bitmap, wxPoint(i*20, j*20), wxSize(20,20));
    //         Bind(wxEVT_BUTTON, MyFrame::Event, button_id);
    //         button_id++;
    //         cout << "created a button" << endl;
    //     }
    // }

//  wxBitmapButton* button = new wxBitmapButton (this, 1, Bitmap, wxPoint(20, 20), wxSize(20,20));
}

void MyFrame::OnPaint(wxPaintEvent&)
{
    wxPaintDC dc(m_board);
    dc.Clear();

    int field [SIDE] [SIDE] = {};

    // dc.DrawBitmap(mine_image, 2, 2, true); // je jedno kdy to vykresli, ale kresli to jenom do horniho rohu
    // cout << "drew a mine" << endl;

    // dc.DrawBitmap(empty_image, wxPoint(0, 0), true);
    // cout << "drew a empty" << endl;

    for (int k = 0; k < MINES; k++) {
        int mine_x = rand() % SIDE;
        int mine_y = rand() % SIDE;
        field [mine_x] [mine_y] = 10;
    }

    for (int l = 0; l < SIDE; l++) {
        for (int m = 0; m < SIDE; m++) {
            dc.DrawBitmap(empty_image, wxPoint(l*20, m*20), true);
            if (field[l] [m] == 10) {
                //dc.DrawBitmap(empty_image, wxPoint(l*20, m*20), true);
                dc.DrawBitmap(mine_image, wxPoint(l*20, m*20), true); // tady je chyba, vykresli to jenom poprve
                cout << "drew a mine" << l << m << endl;
            }
            else {
                //dc.DrawBitmap(empty_image, wxPoint(l*20, m*20), true);
                cout << "drew a empty" << l << m << endl;
            }
        }
    }
}

void MyFrame::Event(wxEvent &evt)
{
    cout << "pressed" << endl;
    wxBitmapButton* button = reinterpret_cast <wxBitmapButton*> (evt.GetEventObject());
    button->Destroy();
    evt.Skip();
}

void MyFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

class MyApp : public wxApp
{
    public:
        virtual bool OnInit()
        {
            ::wxInitAllImageHandlers();
            MyFrame* frame = new MyFrame("Minesweeper", wxPoint(50, 50), wxSize(180, 235));
            frame->Show();
            return true;
        }
};

wxIMPLEMENT_APP(MyApp);

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    wxEND_EVENT_TABLE();

