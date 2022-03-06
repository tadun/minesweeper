#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <wx/wx.h>
#include "gameframe.hpp"
#include "minesweeper.hpp"

class App : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif
