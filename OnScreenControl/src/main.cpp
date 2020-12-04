#include <Windows.h>
#include <stdio.h>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    // create a window class:
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Hooking";

    // register class with operating system:
    RegisterClass(&wc);

    // create and show window:
    HWND hwnd = CreateWindow(L"Hooking", L"Hooking", WS_OVERLAPPEDWINDOW, 0, 0, 500, 400, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) 
    {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);

    HINSTANCE hinstDLL = LoadLibrary(L"Hooker.dll");

    //void (*AttachHookProc)(DWORD);
    //AttachHookProc = (void (*)(DWORD)) GetProcAddress(hinstDLL, "AttachHook");
    //AttachHookProc(0);

    // handle messages:
    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //void (*DetachHook)();
    //DetachHook = (void(*)())GetProcAddress(hinstDLL, "DetachHook");
    //DetachHook();

    FreeLibrary(hinstDLL);
    FreeConsole();
    return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}