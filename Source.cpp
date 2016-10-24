#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define _USE_MATH_DEFINES

#include <windows.h>
#include <math.h>

TCHAR szClassName[] = TEXT("Window");

void DrawTree(HDC hdc, LPPOINT a, LPPOINT b, int n)
{
	const double STEM_RATIO = 0.25;
	const double BRANCH_RATIO = 0.6;

	const int xx = b->x - a->x;
	const int yy = -(b->y - a->y);

	const double angle1 = atan((double)yy / xx) + M_PI / 6;
	const double angle2 = atan((double)yy / xx) - M_PI / 6;

	const double center_length = sqrt(xx*xx + yy*yy)*(1 - STEM_RATIO);
	const double branch_length = BRANCH_RATIO*center_length;

	const int sign = (xx >= 0) ? 1 : -1;

	POINT c = { (int)(a->x + STEM_RATIO*xx),(int)(a->y - STEM_RATIO*yy) };
	POINT d = { c.x + sign*(int)(branch_length*cos(angle1)),c.y - sign*(int)(branch_length*sin(angle1)) };
	POINT e = { c.x + sign*(int)(branch_length*cos(angle2)),c.y - sign*(int)(branch_length*sin(angle2)) };

	MoveToEx(hdc, a->x, a->y, 0);
	LineTo(hdc, c.x, c.y);

	if (n <= 0)
	{
		MoveToEx(hdc, c.x, c.y, 0);
		LineTo(hdc, b->x, b->y);
		MoveToEx(hdc, c.x, c.y, 0);
		LineTo(hdc, d.x, d.y);
		MoveToEx(hdc, c.x, c.y, 0);
		LineTo(hdc, e.x, e.y);
	}
	else
	{
		DrawTree(hdc, &c, b, n - 1);
		DrawTree(hdc, &c, &d, n - 1);
		DrawTree(hdc, &c, &e, n - 1);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			POINT P = { 100,400 };
			POINT Q = { 100,100 };
			POINT R = { 250,400 };
			POINT S = { 250,100 };
			POINT T = { 400,400 };
			POINT U = { 400,100 };
			DrawTree(hdc, &P, &Q, 3);
			DrawTree(hdc, &R, &S, 4);
			DrawTree(hdc, &T, &U, 5);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Tree"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
