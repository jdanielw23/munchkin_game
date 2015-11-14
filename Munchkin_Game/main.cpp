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
#include "Deck.h"
#include "Tester.h"

using namespace std;

const char lpszClassName[] = "MunchkinGame";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)                  
	{
	case WM_DESTROY:
		PostQuitMessage(0);		
		break;
	default:        
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	};

	return 0;
};


int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil)
{
	// TODO: Comment this code correctly
	WNDCLASSEX windowClass;
	HWND hWnd;
	bool bDone = false;
	MSG msg;

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
	windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	if (!RegisterClassEx(&windowClass))
		return false;

	hWnd = CreateWindowEx(0, lpszClassName, lpszClassName, WS_OVERLAPPED|WS_SYSMENU, CW_USEDEFAULT,                  
						  CW_USEDEFAULT, 500, 500, HWND_DESKTOP, NULL, hThisInstance, NULL);

	ShowWindow(hWnd, SW_SHOW); 

	while (!bDone)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		};
	}

	Tester tester;
	tester.runTests();

	return 0;
};