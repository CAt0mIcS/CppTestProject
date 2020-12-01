#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main() {
    // create a window class:
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"hooking";

    // register class with operating system:
    RegisterClass(&wc);

    // create and show window:
    HWND hwnd = CreateWindow(L"hooking", L"hooking", WS_OVERLAPPEDWINDOW, 0, 0, 500, 400, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);

    //DWORD threadID = GetWindowThreadProcessId(notepad, NULL);

    HINSTANCE hinstDLL = LoadLibrary(TEXT("Hooker.dll"));

    void (*AttachHookProc)(DWORD);
    AttachHookProc = (void (*)(DWORD)) GetProcAddress(hinstDLL, "AttachHook");
    AttachHookProc(0);

    // handle messages:
    MSG msg = {};

    while (GetMessage(&msg, hwnd, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    printf("Done execution... press any key to exit");
    char garbage = getchar();
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