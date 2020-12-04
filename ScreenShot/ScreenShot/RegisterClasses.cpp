#include "RegisterClasses.h"

ATOM ToolClass(HINSTANCE hInst, WNDPROC ToolWndProc)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 0);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hIconSm = NULL;
    wcex.hInstance = hInst;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpszClassName = TEXT("ToolWnd");
    wcex.lpszMenuName = NULL;
    wcex.lpfnWndProc = (WNDPROC)ToolWndProc;

    return RegisterClassEx(&wcex);
}

ATOM BackgroundClass(HINSTANCE hInst, WNDPROC BackWndProc)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wcex.hCursor = LoadCursor(NULL, IDC_CROSS);
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hIconSm = NULL;
    wcex.hInstance = hInst;
    wcex.style = CS_VREDRAW | CS_HREDRAW;
    wcex.lpszClassName = TEXT("BackgroundWnd");
    wcex.lpszMenuName = NULL;
    wcex.lpfnWndProc = (WNDPROC)BackWndProc;

    return RegisterClassEx(&wcex);
}

ATOM CaptureClass(HINSTANCE hInst, WNDPROC CaptureWndProc)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 150, 150));
    wcex.hCursor = LoadCursor(NULL, IDC_HAND);
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hIconSm = NULL;
    wcex.hInstance = hInst;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpszClassName = TEXT("CaptureWnd");
    wcex.lpszMenuName = NULL;
    wcex.lpfnWndProc = (WNDPROC)CaptureWndProc;

    return RegisterClassEx(&wcex);
}