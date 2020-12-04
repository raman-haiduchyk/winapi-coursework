// ScreenShot.cpp : Defines the entry point for the application.
//
#include "WorkWithBitmaps.h"
#include "ScreenShot.h"
#include "InitToolControls.h"
#include "RegisterClasses.h"
#include "InitWindows.h"
#include "GlobalVars.h"


//Pen
HPEN        hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
HBRUSH      hBrush = (HBRUSH)(GetStockObject(NULL_BRUSH));


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    hInst = hInstance;
    maxFrameSize = { 0, 0 };

    //Register window classes
    BackgroundClass(hInst, BackWndProc);
    ToolClass(hInst, ToolWndProc);
    CaptureClass(hInst, CaptureWndProc);

    if (!CreateBackgroundWindow(&hBackWnd, hInst)) return FALSE;
    if (!(CreateToolWindow(&hToolWnd, hInst, hBackWnd) && CreateCaptureWindow(&hCapWnd, hInst, hBackWnd))) return FALSE;
    
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENSHOT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


#pragma region WndProc

LRESULT CALLBACK BackWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        RegisterHotKey(hWnd, HOTKEY_ID, MOD_CONTROL, VK_F7);            // Register Ctrl+F7 hotkey
        RegisterHotKey(hWnd, COPY_TO_CLIPBOARD_ID, MOD_CONTROL, 'C');   // Register Ctrl+С hotkey
        SetLayeredWindowAttributes(hWnd, NULL, 1, LWA_ALPHA);           // Setting transparent of main window
    }
    break;
    case WM_HOTKEY:
    {
        // Show and hide hotkey
        switch (wParam)
        {
        case HOTKEY_ID:
        {
            if (isShown)
            {
                ShowWindow(hWnd, SW_HIDE);
                ShowWindow(hToolWnd, SW_HIDE);
                ShowWindow(hCapWnd, SW_HIDE);
            }
            else
            {
                ShowWindow(hWnd, SW_SHOW);
                ShowWindow(hToolWnd, SW_SHOW);
                ShowWindow(hCapWnd, SW_SHOW);
            }
            isShown = !isShown;
        }
        break;
        case COPY_TO_CLIPBOARD_ID:
        {
            //ctr + c (copy to clipboard)
            if (hBmp != NULL)
            {
                RECT clientRect;
                GetClientRect(hCapWnd, &clientRect);

                int height = clientRect.bottom, width = clientRect.right;

                HBITMAP hBmpCopy = CreateBitmap(width, height, 1, 32, NULL);

                OpenClipboard(hWnd);
                EmptyClipboard();
                
                HDC srcDC = CreateCompatibleDC(GetDC(NULL));
                HDC newDC = CreateCompatibleDC(GetDC(NULL));

                HBITMAP srcBitmap = (HBITMAP)SelectObject(srcDC, hBmp);
                HBITMAP newBitmap = (HBITMAP)SelectObject(newDC, hBmpCopy);

                BitBlt(newDC, 0, 0, width, height, srcDC, 0, 0, SRCCOPY);

                SelectObject(srcDC, srcBitmap);
                SelectObject(newDC, newBitmap);

                DeleteDC(srcDC);
                DeleteDC(newDC);

                SetClipboardData(CF_BITMAP, hBmpCopy);

                CloseClipboard();

                DeleteObject(hBmpCopy);
            }
        }
        break;
        }
    }
    break;
    case WM_LBUTTONDOWN:
    {
        //Drop timer if exists
        KillTimer(hCapWnd, IDT_SCREENSHOT);

        //Disable acces to capture window
        if (GetWindowLong(hCapWnd, GWL_STYLE) & WS_DISABLED)
            SetWindowLong(hCapWnd, GWL_STYLE, GetWindowLong(hCapWnd, GWL_STYLE) ^ WS_DISABLED);

        ShowWindow(hToolWnd, SW_HIDE);

        // "remember" start point of capture window
        xLeftPos = GET_X_LPARAM(lParam);
        yTopPos = GET_Y_LPARAM(lParam);

        // capture window state flags
        isDrawingCapWnd = true;
        isCreatedCapWnd = false;

        // "reset" capture window
        SetLayeredWindowAttributes(hCapWnd, NULL, 60, LWA_ALPHA);
        SetWindowLong(hCapWnd, GWL_EXSTYLE, GetWindowLong(hCapWnd, GWL_EXSTYLE) ^ WS_EX_TRANSPARENT);

        // "reset" tool window
        SetWindowLong(hToolBox, GWL_STYLE, GetWindowLong(hToolBox, GWL_STYLE) | WS_DISABLED);
        SetWindowLong(hSaveBtn, GWL_STYLE, GetWindowLong(hSaveBtn, GWL_STYLE) | WS_DISABLED);
        SendMessage(hToolBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
        toolId = ID_HAND;

        // drop bmp
        maxFrameSize = { 0, 0 };
        if (hBmp) DeleteObject(hBmp);
        hBmp = NULL;

        InvalidateRect(hCapWnd, NULL, true);
    }
    break;
    case WM_LBUTTONUP:
    {
        ShowWindow(hToolWnd, SW_SHOWDEFAULT);

        // remove transparent option from capture window
        SetWindowLong(hCapWnd, GWL_EXSTYLE, GetWindowLong(hCapWnd, GWL_EXSTYLE) ^ WS_EX_TRANSPARENT);

        // finish "drawing" capture window
        isDrawingCapWnd = false;
        isCreatedCapWnd = true;
    }
    break;
    case WM_MOUSEMOVE:
    {
        if (isDrawingCapWnd)
        {
            // resizing capture window
            xRightPos = GET_X_LPARAM(lParam);
            yBottomPos = GET_Y_LPARAM(lParam);
            if (yTopPos == yBottomPos && xLeftPos == xRightPos) return 0;
            int frameWidth = abs(xRightPos - xLeftPos);
            int frameHeight = abs(yBottomPos - yTopPos);
            
            INT left = xLeftPos < xRightPos ? xLeftPos : xRightPos + 1;
            INT top = yTopPos < yBottomPos ? yTopPos : yBottomPos + 1;
            MoveWindow(hCapWnd, left, top, frameWidth, frameHeight, FALSE);
        }
    }
    break;
    case WM_DESTROY:
    {
        UnregisterHotKey(hWnd, HOTKEY_ID);
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK CaptureWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        RegisterHotKey(hWnd, SCREEN1_HOTKEY_ID, MOD_CONTROL, VK_SPACE);    // register ctrl+SPACE hotkey
    }
    break;
    case WM_HOTKEY:
    {
        if (wParam == SCREEN1_HOTKEY_ID)
        {
            //capture screen area under capture window
            if (isCreatedCapWnd)
            { 
                isCreatedCapWnd = false;
                if (timer == 0)
                {
                    MakeScreenshot(hWnd, &hBmp, &maxFrameSize);

                    SetWindowLong(hToolBox, GWL_STYLE, GetWindowLong(hToolBox, GWL_STYLE) ^ WS_DISABLED);
                    SetWindowLong(hSaveBtn, GWL_STYLE, GetWindowLong(hSaveBtn, GWL_STYLE) ^ WS_DISABLED);
                    InvalidateRect(hToolWnd, NULL, false);
                }
                else
                {
                    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) ^ WS_DISABLED);
                    SetTimer(hWnd, IDT_SCREENSHOT, timer * 1000, (TIMERPROC)TimerProc);
                }
            }
        }
    }
    break;
    case WM_LBUTTONDOWN:
    {
        if (toolId != ID_HAND)
        {
            // "remember" drawing start point 
            xDrawingOld = GET_X_LPARAM(lParam);
            yDrawingOld = GET_Y_LPARAM(lParam);
            xDrawing = -1;
            yDrawing = -1;
            isDrawing = true;
        }
    }
    break;
    case WM_MOUSEMOVE:
    {
        if (isDrawing)
        {
            xDrawing = GET_X_LPARAM(lParam);
            yDrawing = GET_Y_LPARAM(lParam);
            InvalidateRect(hWnd, NULL, false);
        }
    }
    break;
    case WM_LBUTTONUP:
    {
        // draw rect or ellipse
        if (toolId > ID_PEN && isDrawing)
        {
            PAINTSTRUCT     ps;
            HDC             hdc;
            hdc = BeginPaint(hWnd, &ps);
            BITMAP          bitmap;
            HDC             hdcMem;
            HGDIOBJ         oldBitmap;

            hdcMem = CreateCompatibleDC(hdc);
            oldBitmap = SelectObject(hdcMem, hBmp);

            GetObject(hBmp, sizeof(bitmap), &bitmap);

            DrawShape(hdcMem, toolId, hPen, hBrush, xDrawing, yDrawing, &xDrawingOld, &yDrawingOld);

            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);
        }
        isDrawing = false;
    }
    break;
    case WM_GETMINMAXINFO:
    {
        // checking max size
        if (maxFrameSize.y != 0 && maxFrameSize.x != 0)
        {
            MINMAXINFO* minmaxPtr;
            minmaxPtr = (MINMAXINFO*)lParam;
            minmaxPtr->ptMaxTrackSize = maxFrameSize;
        }
    }
    break;
    case WM_NCHITTEST:
    {
        // drag-n-drop
        LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
        if (hit == HTCLIENT && !(toolId != ID_HAND)) hit = HTCAPTION;
        return hit;
    }
    break;
    case WM_PAINT:
    {
        // initialize painting
        PAINTSTRUCT     ps;
        HDC             hdc;
        hdc = BeginPaint(hWnd, &ps);

        if (hBmp)
        {
            // drawing bitmap in capture window

            BITMAP          bitmap;
            HDC             hdcMem;
            HGDIOBJ         oldBitmap;

            hdcMem = CreateCompatibleDC(hdc);
            oldBitmap = SelectObject(hdcMem, hBmp);

            GetObject(hBmp, sizeof(bitmap), &bitmap);
            
            // drawing with pen
            if (isDrawing && toolId == ID_PEN)
            {
                DrawShape(hdcMem, toolId, hPen, hBrush, xDrawing, yDrawing, &xDrawingOld, &yDrawingOld);
            }

            BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

            //drawing with shapes
            if (isDrawing && toolId > ID_PEN)
            {
                DrawShape(hdc, toolId, hPen, hBrush, xDrawing, yDrawing, &xDrawingOld, &yDrawingOld);
            }
            

            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);
        }
        else
        {
            // "reset" capture window
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            Rectangle(hdc, 0, 0, clientRect.right, clientRect.left);
        }

        EndPaint(hWnd, &ps);
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    
}

LRESULT CALLBACK ToolWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            CreateToolWndControls(hWnd, hInst, &hSaveBtn, &hToolBox, &ofn, &cc);
            SetWindowLong(hToolBox, GWL_STYLE, GetWindowLong(hToolBox, GWL_STYLE) | WS_DISABLED);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDC_TIMER_COMBOBOX:
            {
               timer = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

            }
            break;
            case IDC_TOOL_COMBOBOX:
            {
                toolId = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
            }
            break;
            case IDC_SAVE_BUTTON:
            {
                if (GetSaveFileName(&ofn))
                {
                    TCHAR path[MAX_PATH];
                    _tcscpy_s(path, ofn.lpstrFile);

                    RECT clientRect;
                    GetClientRect(hWnd, &clientRect);

                    int height = clientRect.bottom, width = clientRect.right;

                    HBITMAP hBmpCopy = CreateBitmap(width, height, 1, 32, NULL);

                    HDC srcDC = CreateCompatibleDC(GetDC(NULL));
                    HDC newDC = CreateCompatibleDC(GetDC(NULL));

                    HBITMAP srcBitmap = (HBITMAP)SelectObject(srcDC, hBmp);
                    HBITMAP newBitmap = (HBITMAP)SelectObject(newDC, hBmpCopy);

                    BitBlt(newDC, 0, 0, width, height, srcDC, 0, 0, SRCCOPY);

                    SelectObject(srcDC, srcBitmap);
                    SelectObject(newDC, newBitmap);

                    DeleteDC(srcDC);
                    DeleteDC(newDC);

                    SaveBmpAs(hBmpCopy, path);
                    DeleteObject(hBmpCopy);
                }
            }
            break;
            case IDC_COLOR_BUTTON:
            {
                if (ChooseColor(&cc))
                {
                    DeleteObject(hPen);
                    hPen = CreatePen(PS_SOLID, 2, cc.rgbResult);
                }
            }
            break;
            case IDC_HELP_BUTTON:
            {
                MessageBox(hWnd, lpText, TEXT("HELP"), MB_OK);
            }
            break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CLOSE:
        {
            DestroyWindow(hWnd);
            DestroyWindow(hCapWnd);
            DestroyWindow(hBackWnd);
            return 0;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    
}

#pragma endregion


VOID CALLBACK TimerProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
    MakeScreenshot(hWnd, &hBmp, &maxFrameSize);
    SetWindowLong(hToolBox, GWL_STYLE, GetWindowLong(hToolBox, GWL_STYLE) ^ WS_DISABLED);
    SetWindowLong(hSaveBtn, GWL_STYLE, GetWindowLong(hSaveBtn, GWL_STYLE) ^ WS_DISABLED);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) ^ WS_DISABLED);
    InvalidateRect(hToolWnd, NULL, false);
    KillTimer(hWnd, IDT_SCREENSHOT);
}
