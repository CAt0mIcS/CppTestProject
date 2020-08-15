#pragma once
#include "pch.h"


#define CREATE_MAIN_WINDOW() \
	WNDCLASS wc = {};\
	wc.lpfnWndProc = MainWindowProc;\
	wc.lpszClassName = L"MAINWINDOWCLASS";\
	wc.hInstance = hInstance;\
	RegisterClass(&wc);\
	HWND hWnd = CreateWindowEx(0, L"MAINWINDOWCLASS", L"MainWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 350, 250, 0, 0, hInstance, 0);\
	ShowWindow(hWnd, nCmdShow)


#define ENTER_MAIN_MESSAGE_LOOP()\
	MSG msg{};\
	while (GetMessage(&msg, 0, 0, 0))\
	{\
		TranslateMessage(&msg);\
		DispatchMessage(&msg);\
	}\
	UnregisterClass(L"MAINWINDOWCLASS", hInstance);\
	return 0
