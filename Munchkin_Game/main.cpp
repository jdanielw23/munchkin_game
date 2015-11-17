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
#include <cstdio>
#include <ctime>
#include "Deck.h"
#include "Tester.h"

using namespace std;

// Name of Window class
const char lpszClassName[] = "MunchkinGame";

const int MAX_PLAYERS = 3;
const int MAX_ATTRIBS = 5;

// This function carries out the messages that are sent to the window, i.e., click, dblclick, resize, etc...
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;				// handle to Device Context
	PAINTSTRUCT ps;			// Used to repaint window

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
	HWND lblPlayerTitle, lblLevelTitle, lblStrengthTitle, lblClassTitle, lblRaceTitle; // Titles for the player attributes
	HWND lblPlayerAttrib[MAX_PLAYERS][MAX_ATTRIBS];	// Labels for the player data; [player][attribute]
	HWND lblMunchkinStrengthTitle, lblMunchkinStrength; // Munchkin str labels
	HWND lblRectangle0, lblRectangle1; // Labels for rectangle objects
	HWND lblEnemyTitle, lblEnemyLevel, lblEnemyDescription, lblEnemyBadStuff, lblEnemyTreasures, lblEnemyNumLevels;

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
	hWnd = CreateWindowEx(0, lpszClassName, "Munchkin Game!", WS_OVERLAPPED|WS_SYSMENU, 
						  CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, HWND_DESKTOP, NULL, hThisInstance, NULL);

	// Start the randomizer
	srand(time(NULL));

	// Create Labels
	lblPlayerTitle = CreateWindow("static", "Player", WS_CHILD | WS_VISIBLE, 8, 8, 64, 16, hWnd, 0, hThisInstance, NULL);
	lblLevelTitle = CreateWindow("static", "Level", WS_CHILD | WS_VISIBLE, 72, 8, 64, 16, hWnd, 0, hThisInstance, NULL);
	lblStrengthTitle = CreateWindow("static", "Strength", WS_CHILD | WS_VISIBLE, 136, 8, 64, 16, hWnd, 0, hThisInstance, NULL);
	lblClassTitle = CreateWindow("static", "Class", WS_CHILD | WS_VISIBLE, 200, 8, 64, 16, hWnd, 0, hThisInstance, NULL);
	lblRaceTitle = CreateWindow("static", "Race", WS_CHILD | WS_VISIBLE, 264, 8, 64, 16, hWnd, 0, hThisInstance, NULL);

	// Temp code, delete this later.
	string name[4] = { "Bob", "Jake", "Chris", "CPU" };
	string classes[4] = { "-", "Warrior", "-", "-" };
	string races[4] = { "Elf", "-", "-", "-" };
	int levels[4] = { 3, 5, 4, 0 };
	int strength[4] = { 10, 9, 6, 0 };

	// TODO: Remove magic numbers
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// Set the labels and attributes
		lblPlayerAttrib[i][0] = CreateWindow("static", name[i].c_str(), WS_CHILD | WS_VISIBLE, 8, 8 + (16 * (i + 1)), 64, 16, hWnd, 0, hThisInstance, NULL);
		lblPlayerAttrib[i][1] = CreateWindow("static", to_string(levels[i]).c_str(), WS_CHILD | WS_VISIBLE, 72, 8 + (16 * (i + 1)), 64, 16, hWnd, 0, hThisInstance, NULL);
		lblPlayerAttrib[i][2] = CreateWindow("static", to_string(strength[i]).c_str(), WS_CHILD | WS_VISIBLE, 136, 8 + (16 * (i + 1)), 64, 16, hWnd, 0, hThisInstance, NULL);
		lblPlayerAttrib[i][3] = CreateWindow("static", classes[i].c_str(), WS_CHILD | WS_VISIBLE, 200, 8 + (16 * (i + 1)), 64, 16, hWnd, 0, hThisInstance, NULL);
		lblPlayerAttrib[i][4] = CreateWindow("static", races[i].c_str(), WS_CHILD | WS_VISIBLE, 264, 8 + (16 * (i + 1)), 64, 16, hWnd, 0, hThisInstance, NULL);
	}

	// Create the Strength labels
	lblMunchkinStrengthTitle = CreateWindow("static", "Munchkin Strength", WS_CHILD | WS_VISIBLE | SS_CENTER, 328, 8, 128, 16, hWnd, 0, hThisInstance, NULL);
	lblMunchkinStrength = CreateWindow("static", "9", WS_CHILD | WS_VISIBLE | SS_CENTER, 328, 24, 128, 16, hWnd, 0, hThisInstance, NULL);
	
	// Create enemy data labels.
	lblRectangle0 = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_CENTER | WS_BORDER, 456, 8, 128, 128, hWnd, 0, hThisInstance, NULL);
	lblEnemyTitle = CreateWindow("static", "Goblin", WS_CHILD | WS_VISIBLE | SS_CENTER, 456, 18, 128, 16, hWnd, 0, hThisInstance, NULL);

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