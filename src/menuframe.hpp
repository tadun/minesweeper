#ifndef MENUFRAME_HPP_
#define MENUFRAME_HPP_

#include <iostream>
#include <wx/wx.h>
#include <wx/button.h>
#include <wx/event.h>

using namespace std;

class MenuFrame : public wxFrame
{
    public:
        MenuFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
        void selectedOption(wxMouseEvent& event);
};

#endif
