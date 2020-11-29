// https://stackoverflow.com/questions/29091028/windows-api-write-to-screen-as-on-screen-display


#include <Windows.h>
#include <iostream>
// ---------------------------------------------------------
// GLOBALS
void Shutdown();
void Setup();
void DrawRectangle(int r, int g, int b);
void CALLBACK WinHookProc(
	HWINEVENTHOOK hook, DWORD e, HWND hWnd,
	LONG idObject, LONG idChild,
	DWORD dwEventThread, DWORD dwmsEventTime
);

static HWINEVENTHOOK g_Hook = 0;
static RECT g_Rc{ 5, 5, 1000, 500 };



// ---------------------------------------------------------
// WINDOW SHOULD RESIZE TO RECT
bool MouseIsInRectAndWindowIsMoving()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	if (PtInRect(&g_Rc, mousePos))
	{
		return true;
	}

	return false;
}

// ---------------------------------------------------------
// RESIZING LOGIC
void ResizeMovingWindow()
{

}

// ---------------------------------------------------------
// MAIN FUNCTION
int main()
{
	Setup();

	while (true)
	{
		if (MouseIsInRectAndWindowIsMoving())
		{
			DrawRectangle(255, 0, 0);
			ResizeMovingWindow();
		}
		else
		{
			DrawRectangle(0, 255, 0);
		}
	}

	Shutdown();
}

// ---------------------------------------------------------
// SETUP PROCEDURE
void Setup()
{
	if(FAILED(CoInitialize(NULL)))
		std::cout << "Failed to initialize COM: 0x" << std::hex << std::uppercase << GetLastError() << '\n';

	g_Hook = SetWinEventHook(
		EVENT_MIN,
		EVENT_MAX, NULL, WinHookProc,
		0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
	);

	std::cout << "Event Hook GetLastError: 0x" << std::hex << std::uppercase << GetLastError() << '\n';
	if (!g_Hook)
	{
		std::cout << "Failed to set event hook: 0x" << std::hex << std::uppercase << GetLastError() << '\n';
	}
}

// ---------------------------------------------------------
// DRAWING LOGIC
void DrawRectangle(int r, int g, int b)
{
	HDC hDC = GetDC(NULL);
	FrameRect(hDC, &g_Rc, CreateSolidBrush(RGB(r, g, b)));
	ReleaseDC(NULL, hDC);
}


// ---------------------------------------------------------
// WINDOWS HOOK
void CALLBACK WinHookProc(
	HWINEVENTHOOK hook, DWORD e, HWND hWnd,
	LONG idObject, LONG idChild,
	DWORD dwEventThread, DWORD dwmsEventTime
)
{
	std::cout << "Window Hook Entered!\n";

	int len = GetWindowTextLength(hWnd);
	wchar_t* name = new wchar_t[len];

	GetWindowText(hWnd, name, len);
	std::cout << "Window with name " << name << " moved!\n";
}


// ---------------------------------------------------------
// SHUTDOWN PROCEDURE
void Shutdown()
{
	UnhookWinEvent(g_Hook);
	CoUninitialize();
}