#pragma once

#include <string>
#include "X11/Xlib.h"


class MyWindow
{
public:
    MyWindow();

    bool ShouldClose();

private:
    Display* m_pDisplay;
    Window m_Window;
    int m_Screen;
};