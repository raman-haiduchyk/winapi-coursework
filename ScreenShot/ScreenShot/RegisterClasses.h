#pragma once

#include "framework.h"
#include "resource.h"

ATOM		BackgroundClass(HINSTANCE hInst, WNDPROC BackWndProc);
ATOM		ToolClass(HINSTANCE hInst, WNDPROC ToolWndProc);
ATOM		CaptureClass(HINSTANCE hInst, WNDPROC CaptureWndProc);