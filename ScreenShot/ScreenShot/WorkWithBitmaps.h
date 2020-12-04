#pragma once

#include "framework.h"
#include "Resource.h"

HBITMAP CopyDCToBitmap(HDC hScrDC, LPRECT lpRect);

VOID MakeScreenshot(HWND hWnd, HBITMAP *hBmp, POINT *maxFrameSize);

VOID DrawShape(HDC hdc, int toolId, HPEN hPen, HBRUSH hBrush, int xDrawing, int yDrawing, int* xDrawingOld, int* yDrawingOld);

BOOL SaveBmpAs(HBITMAP hBitmap, LPCTSTR FileName);