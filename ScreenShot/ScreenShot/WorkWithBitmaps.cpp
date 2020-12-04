#include "WorkWithBitmaps.h"

HBITMAP CopyDCToBitmap(HDC hScrDC, LPRECT lpRect)
{
    HDC hMemDC;
    HBITMAP hBitmap, hOldBitmap;
    int nX, nY, nX2, nY2;
    int nWidth, nHeight;
    if (IsRectEmpty(lpRect))
        return NULL;

    nX = lpRect->left;
    nY = lpRect->top;
    nX2 = lpRect->right;
    nY2 = lpRect->bottom;
    nWidth = nX2 - nX;
    nHeight = nY2 - nY;
    hMemDC = CreateCompatibleDC(hScrDC);
    hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    StretchBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, nWidth, nHeight, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

    DeleteDC(hMemDC);
    DeleteObject(hOldBitmap);
    return hBitmap;
}

BOOL SaveBmpAs(HBITMAP hBitmap, LPCTSTR FileName)
{
    CImage img;
    img.Attach(hBitmap);
    HRESULT Save = img.Save(FileName);
    img.Detach();
    return true;
}

VOID MakeScreenshot(HWND hWnd, HBITMAP *hBmp, POINT *maxFrameSize)
{
    // make capture window full transparent
    SetLayeredWindowAttributes(hWnd, NULL, 0, LWA_ALPHA);

    RECT captureCoord, clientRect;
    GetClientRect(hWnd, &clientRect);
    GetWindowRect(hWnd, &captureCoord);

    // set max size of capture window
    *maxFrameSize = { captureCoord.right - captureCoord.left, captureCoord.bottom - captureCoord.top };

    // border size correction
    int borders = (captureCoord.right - captureCoord.left - clientRect.right) / 2;
    captureCoord.left += borders;
    captureCoord.right -= borders;
    captureCoord.top += borders;
    captureCoord.bottom -= borders;

    if (!&hBmp) DeleteObject(&hBmp);

    // capture bitmap
    *hBmp = CopyDCToBitmap(GetDC(GetDesktopWindow()), &captureCoord);

    // make capture window non-transparent
    SetLayeredWindowAttributes(hWnd, NULL, 255, LWA_ALPHA);

    // update capture window to show captured bitmap
    InvalidateRect(hWnd, NULL, true);
}

VOID DrawShape(HDC hdc, int toolId, HPEN hPen, HBRUSH hBrush, int xDrawing, int yDrawing, int* xDrawingOld, int* yDrawingOld)
{
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    switch (toolId)
    {
    case ID_PEN:
    {
        MoveToEx(hdc, *xDrawingOld, *yDrawingOld, NULL);
        LineTo(hdc, xDrawing, yDrawing);
        *xDrawingOld = xDrawing;
        *yDrawingOld = yDrawing;
    }
    break;
    case ID_RECT:
    {
        Rectangle(hdc, *xDrawingOld, *yDrawingOld, xDrawing, yDrawing);
    }
    break;
    case ID_ELLIPSE:
    {
        Ellipse(hdc, *xDrawingOld, *yDrawingOld, xDrawing, yDrawing);
    }
    break;
    }

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
}