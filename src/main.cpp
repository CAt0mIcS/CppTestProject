#include <X11/Xlib.h>
#include <unistd.h>

#include <iostream>

#include <Window.h>


int main()
{
    MyWindow win;
    while(!win.ShouldClose())
    {
        
    }
    std::cin.get();
}