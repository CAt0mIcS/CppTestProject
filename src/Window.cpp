#include "Window.h"

#include <iostream>


MyWindow::MyWindow()
{
    m_pDisplay = XOpenDisplay(0);
    m_Screen = DefaultScreen(m_pDisplay);
    if (!m_pDisplay)
    {
        std::cerr << "Failed to open the Display\n";
    }

    m_Window = XCreateWindow(m_pDisplay, DefaultRootWindow(m_pDisplay), 0, 0, 200, 200, 0, CopyFromParent, CopyFromParent, CopyFromParent, 0, 0);
    XSelectInput(m_pDisplay, m_Window, 
        ExposureMask | 
        KeyPressMask | KeyReleaseMask |
        Button1Mask | Button2Mask | Button3Mask | Button4Mask | Button5Mask | ButtonMotionMask | 
        ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
        ControlMask | EnterWindowMask | FocusChangeMask | LeaveWindowMask | ResizeRedirectMask | SubstructureNotifyMask //, ...
    );

    XMapWindow(m_pDisplay, m_Window);
    XFlush(m_pDisplay);
}
#include <iostream>
bool MyWindow::ShouldClose()
{
    XEvent e;

    while(true)
    {
        XNextEvent(m_pDisplay, &e);
        if(e.type == Expose)
        {
            XDrawLine(m_pDisplay, m_Window, DefaultGC(m_pDisplay, m_Screen), 10, 10, 60, 60);
        }
    }

    return false;
}