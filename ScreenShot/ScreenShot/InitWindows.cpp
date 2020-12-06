#include "InitWindows.h"



BOOL CreateBackgroundWindow(HWND* hBackWnd, HINSTANCE hInst)
{
    int initX = 0;
    int initY = 0;
    int initWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int initHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    *hBackWnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        TEXT("BackgroundWnd"),
        TEXT("ScreenShot"),
        WS_POPUP,
        initX, initY, initWidth, initHeight,
        NULL,
        NULL,
        hInst,
        NULL
    );

    if (!hBackWnd)
    {
        return FALSE;
    }

    ShowWindow(*hBackWnd, SW_HIDE);
    UpdateWindow(*hBackWnd);

    return TRUE;
}

BOOL CreateToolWindow(HWND* hToolWnd, HINSTANCE hInst, HWND hParent)
{
    int initY = 0;
    int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);;
    int initHeight = 100;
    int initWidth = 390;

    *hToolWnd = CreateWindowEx(
        WS_EX_TOPMOST,
        TEXT("ToolWnd"),
        NULL,
        WS_POPUPWINDOW | WS_CAPTION,
        (screenWidth - initWidth) / 2, initY, initWidth, initHeight,
        hParent,
        NULL,
        hInst,
        NULL
    );

    if (!hToolWnd)
    {
        DWORD err = GetLastError();
        return FALSE;
    }

    ShowWindow(*hToolWnd, SW_HIDE);
    UpdateWindow(*hToolWnd);

    return TRUE;
}

BOOL CreateCaptureWindow(HWND* hCapWnd, HINSTANCE hInst, HWND hParent)
{
    *hCapWnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        TEXT("CaptureWnd"),
        NULL,
        WS_POPUP | WS_THICKFRAME,
        -100, 0, 0, 0,
        hParent,
        NULL,
        hInst,
        NULL
    );

    if (!hCapWnd)
    {
        return FALSE;
    }

    ShowWindow(*hCapWnd, SW_HIDE);
    UpdateWindow(*hCapWnd);

    return TRUE;
}