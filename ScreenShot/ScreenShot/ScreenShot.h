#pragma once

#include "resource.h"
#include "framework.h"

//Forward declarations of functions included in this code module:

VOID				CALLBACK TimerProc (HWND hWnd,        // handle to window for timer messages 
										UINT message,     // WM_TIMER message 
										UINT idTimer,     // timer identifier 
										DWORD dwTime);     // current system time  

LRESULT CALLBACK    BackWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    CaptureWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ToolWndProc(HWND, UINT, WPARAM, LPARAM);