#pragma once

#include "framework.h"

BOOL		CreateBackgroundWindow(HWND* hBackWnd, HINSTANCE hInst);
BOOL		CreateToolWindow(HWND* hToolWnd, HINSTANCE hInst, HWND hParent);
BOOL		CreateCaptureWindow(HWND* hCapWnd, HINSTANCE hInst, HWND hParent);