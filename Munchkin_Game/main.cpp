/**
* EGR 327 Project	CBU
* main.cpp			Source File for the main window
* Created 11-01-15	RSR
* This is user interface window.
* REVISION HISTORY:
*/

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "Deck.h"
#include "Tester.h"

using namespace std;

// Name of Window class
const char lpszClassName[] = "MunchkinGame";

// This function carries out the messages that are sent to the window, i.e., click, dblclick, resize, etc...
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC; // handle to Device Context

	switch (message)                  
	{
	case WM_DESTROY:
	case WM_CLOSE:
		// If we destroy/close the window, post a quit message so the program ends.
		PostQuitMessage(0);		
		break;
	case WM_CTLCOLORSTATIC:
		hDC = (HDC)wParam;
		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);
		return (LRESULT)GetStockObject(NULL_BRUSH);
	default:
		// Otherwise, execute the default behaviour.
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	};

	// This should never happen, but better safe than sorry. 
	return 0;
};


int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
	// Create the variables to create a Win32 Window
	WNDCLASSEX windowClass;
	HWND hWnd;
	bool bDone = false;
	MSG msg;

	// Button Controls
	HWND btnFight;

	// Labels
	HWND lblPlayerTitle, lblLevelTitle, lblStrengthTitle, lblClassTitle, lblRaceTitle;
	HWND lblPlayerAttrib[4][5];	// Labels for the player data; [player][attribute]

	// Fill out the window information
	windowClass.hInstance = hThisInstance;
	windowClass.lpszClassName = lpszClassName;
	windowClass.lpfnWndProc = WndProc;
	windowClass.style = CS_DBLCLKS;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszMenuName = NULL;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hbrBackground = GetSysColorBrush(COLOR_WINDOW);

	// Register the class so Windows knows it's name.
	if (!RegisterClassEx(&windowClass))
		return false;

	// Create the Window
	hWnd = CreateWindowEx(0, lpszClassName, "Munchkin Game!", WS_OVERLAPPED|WS_SYSMENU, CW_USEDEFAULT,
						  CW_USEDEFAULT, 500, 500, HWND_DESKTOP, NULL, hThisInstance, NULL);

	// Create Labels
	lblPlayerTitle = CreateWindow("static", "Player", WS_CHILD | WS_VISIBLE, 8, 8, 64, 16, hWnd, 0, hThisInstance, NULL);
	lblLevelTitle = CreateWindow("static", "Level", WS_CHILD | WS_VISIBLE, 72, 8, 64, 16, hWnd, 0, hThisInstance, NULL);
	lblStrengthTitle = CreateWindow("static", "Strength", WS_CHILD | WS_VISIBLE, 136, 8, 64, 16, hWnd, 0, hThisInstance, NULL);
	lblClassTitle = CreateWindow("static", "Class", WS_CHILD | WS_VISIBLE, 200, 8, 64, 16, hWnd, 0, hThisInstance, NULL);
	lblRaceTitle = CreateWindow("static", "Race", WS_CHILD | WS_VISIBLE, 264, 8, 64, 16, hWnd, 0, hThisInstance, NULL);

	// Make it visible
	ShowWindow(hWnd, SW_SHOW); 

	// While its not done or hasn't received a quit message, keep processing windows messages
	while (!bDone)
	{
		// Check which message we recieved without removing it from the queue.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If it was a quit message, we are done; end loop.
			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				// Otherwise, carry out the message.
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		};
	}

	// Run our tests.
	Tester tester;
	tester.runTests();

	return 0;
};