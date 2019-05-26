
#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>

#define MAX_LOADSTRING 100
#define TMR_1 1

HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];				
TCHAR szWindowClass[MAX_LOADSTRING];			

INT value_x;
INT value_y;
enum type { circle, rectangle, triangle };

struct cargo
{
	type figure;
	int X, Y;
	bool New;
};



struct crane
{
	int x;
	int y;
	bool grip;
	int grabed;
};



std::vector<cargo> cargos;
crane Crane;


void LoadScreen(HWND hWnd) {
	HDC hdc = GetDC(hWnd);
	HBRUSH brush = CreatePatternBrush((HBITMAP)LoadImage(NULL, L"file.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	HBRUSH hBrush = CreateSolidBrush(RGB(70, 70, 70));
	RECT crane = { 0, 0, 759, 517 };
	FillRect(hdc, &crane, brush);
	RECT ground = { 0, 445, 1000, 500 };
	FillRect(hdc, &ground, hBrush);
	DeleteObject(brush);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hdc);
}



HWND hwndButton;

int col = 0;
RECT drawArea1 = { 198, 84, 1000, 445 };
RECT drawArea2 = { 50, 400, 650, 422 };


ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void drawCargos(HDC hdc)
{
	for (int i = 0; i < cargos.size(); i++) {
		if (cargos[i].figure == circle)
		{
			Ellipse(hdc, cargos[i].X - 20, cargos[i].Y, cargos[i].X + 20, cargos[i].Y + 40);
		}
		if (cargos[i].figure == rectangle)
		{
			Rectangle(hdc, cargos[i].X - 20, cargos[i].Y, cargos[i].X + 20, cargos[i].Y + 40);
		}
		if (cargos[i].figure == triangle)
		{
			POINT Pt[3];
			Pt[0].x = cargos[i].X;
			Pt[0].y = cargos[i].Y;
			Pt[1].x = cargos[i].X - 20;
			Pt[1].y = cargos[i].Y + 40;
			Pt[2].x = cargos[i].X + 20;
			Pt[2].y = cargos[i].Y + 40;

			Polygon(hdc, Pt, 3);
		}
	}
}


void initCargos()
{
	cargo Cargo = { triangle, 350, 404, false };
	cargos.push_back(Cargo);
}


void MyOnPaint(HDC hdc, HWND hWnd)
{
	RECT rc;
	HDC hdcMem;
	HBITMAP hbmMem, hbmOld;
	HBRUSH hbrBkGnd;

	GetClientRect(hWnd, &rc);
	hdcMem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdcMem,
		rc.right - rc.left,
		rc.bottom - rc.top);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);


	hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	FillRect(hdcMem, &rc, hbrBkGnd);
	DeleteObject(hbrBkGnd);

	Graphics graphics(hdcMem);
	Pen pen(Color(255, 0, 0, 0), 2);
	Pen pen2(Color(255, 25 * col, 0, 255));
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	RECT part = { Crane.x - 10, 84, Crane.x + 10, 94 };
	FillRect(hdcMem, &part, hBrush);
	graphics.DrawLine(&pen, Point(Crane.x, 84), Point(Crane.x, Crane.y));
	drawCargos(hdcMem);


	BitBlt(hdc,
		rc.left, rc.top,
		rc.right - rc.left, rc.bottom - rc.top,
		hdcMem,
		0, 0,
		SRCCOPY);


	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
	DeleteObject(hBrush);
}


void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	LoadScreen(hWnd);

	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc, hWnd);
	EndPaint(hWnd, &ps);
}


int OnCreate(HWND window)
{
	SetTimer(window, TMR_1, 25, 0);
	Crane.x = 220;
	Crane.y = 130;
	Crane.grip = false;
	initCargos();
	return 0;
}


// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	
	MSG msg;
	HACCEL hAccelTable;

	value_x = 0;
	value_y = 50;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; 

					   // main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	                                                      

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}



int search_y(int i)
{
	for (int j = 0; j < cargos.size(); j++) {
		if ((cargos[i].Y == cargos[j].Y - 40) && (cargos[i].X <= cargos[j].X + 20) && (cargos[i].X >= cargos[j].X - 20) && (i != j))
			return 3;
		else if ((cargos[i].Y == cargos[j].Y - 40) && (cargos[i].X < cargos[j].X - 20) && (cargos[i].X > cargos[j].X - 40) && (i != j))
			return 1;
		else if ((cargos[i].Y == cargos[j].Y - 40) && (cargos[i].X > cargos[j].X + 20) && (cargos[i].X < cargos[j].X + 40) && (i != j))
			return 2;
	}
	return 0;
}



int search_x(int i)
{
	for (int j = 0; j < cargos.size(); j++) {
		if ((cargos[i].X + 40 == cargos[j].X) && (cargos[i].Y - 40 < cargos[j].Y) && (i != j))
			return 1;
		else if ((cargos[i].X - 40 == cargos[j].X) && (cargos[i].Y - 40 < cargos[j].Y) && (i != j))
			return 2;
	}
	return 0;
}



bool check_h(int i)
{
	int height = 0;

	for (int k = 1; k < (444 - cargos[i].Y) / 2; k++)
		for (int j = 0; j < cargos.size(); j++) {
			if ((cargos[i].Y + k * 2 == cargos[j].Y) && (cargos[i].X <= cargos[j].X + 20) && (cargos[i].X >= cargos[j].X - 20))
				height++;
		}

	if (height == 3)
		return true;
	else
		return false;
}

bool check_w()
{
	for (int j = 0; j < cargos.size(); j++) {
		if (cargos[j].X >= 900)
			return true;

	}
	return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	if (Crane.grip)
	{
		cargos[Crane.grabed].X = Crane.x;
		cargos[Crane.grabed].Y = Crane.y;
	}
	int k = cargos.size();
	for (int i = 0; i < k; i++)
	{
		if ((cargos[i].X <= 220)) {
			cargos.erase(cargos.begin() + i);
			if (Crane.grabed > i)Crane.grabed--;
			i--;
			k--;

		}
	}

	for (int i = 0; i < cargos.size(); i++) {

		int position_x = search_x(i);

		switch (position_x) {
		case 1:
			cargos[i].X -= 2;
			break;
		case 2:
			cargos[i].X += 2;
			break;
		default:
			break;
		}

		if ((cargos[i].Y < 404) && (Crane.grabed != i)) {
			int position_y = search_y(i);

			switch (position_y) {
			case 0:
				cargos[i].Y += 2;
				break;
			case 1:
				cargos[i].Y -= 2;
				cargos[i].X -= 20;
				break;
			case 2:
				cargos[i].Y -= 2;
				cargos[i].X += 20;
				break;
			default:
				break;
			}
		}
		else if ((cargos[i].X == 660) && (cargos[i].New == true))
			cargos[i].New = false;
		else if (cargos[i].New == true)
			cargos[i].X -= 1;
		else if ((cargos[i].X < 220) || (cargos[i].X > 1020))
			cargos.erase(cargos.begin() + i);
	}

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);


		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
			break;
	case WM_ERASEBKGND:
		return (LRESULT)1;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
	
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	case WM_KEYDOWN:
		switch (wParam)
		{
		repaintWindow(hWnd, hdc, ps, &drawArea1);
		
		case VK_LEFT:
			if (Crane.x >= 224)Crane.x -= 2;
			break;
		case VK_RIGHT:
			if (Crane.x <= 676)Crane.x += 2;
			break;
		case VK_UP:
			if (Crane.y >= 115)Crane.y -= 2;
			break;
		case VK_DOWN:
			if (Crane.y <= 430 && Crane.grip && (cargos[Crane.grabed].Y < 404) && search_y(Crane.grabed) == 0)
			{
				Crane.y += 2;
				break;
			}
			else if (Crane.y <= 430 && !Crane.grip)
			{
				Crane.y += 2;
				break;
			}
		case VK_SPACE:
			if ((Crane.grip) && (check_h(Crane.grabed) == false))
			{
				Crane.grip = false;
				Crane.grabed = -1;
				break;
			}
			for (int i = 0; i < cargos.size(); i++) {
				if (!Crane.grip&&Crane.x == cargos[i].X&&Crane.y == cargos[i].Y)
				{
					Crane.grip = true;
					Crane.grabed = i;
				}
			}
			break;
		case 0x51:
		{
			if (check_w() == false) {
				cargo Cargo = { circle, 1020, 404, true };
				cargos.push_back(Cargo);
			}
			break;
		}
		case 0x57:
		{
			if (check_w() == false) {
				cargo Cargo = { rectangle, 1020, 404, true };
				cargos.push_back(Cargo);
			}
			break;
		}
		case 0x45:
		{
			if (check_w() == false) {
				cargo Cargo = { triangle, 1020, 404, true };
				cargos.push_back(Cargo);
			}
			break;
		}
		}
	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			//force window to repaint
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
