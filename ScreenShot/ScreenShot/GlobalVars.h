#pragma once

#define GLOBAL_VARS_H

#include "framework.h"

HINSTANCE   hInst;                          // current instance

//windows handles
HWND        hBackWnd;						// background window
HWND        hCapWnd;						// capture window
HWND        hToolWnd;						// tool popup window

HWND		hToolBox;						//
HWND		hSaveBtn;

OPENFILENAME ofn;
CHOOSECOLOR	cc;

HBITMAP     hBmp = NULL;					// bmp in capture window

// flags
BOOL        isShown = false;                // is shown app 
BOOL        isDrawingCapWnd = false;
BOOL        isCreatedCapWnd = false;
BOOL        isDrawing = false;

// coords of capture window and its max size
INT         xLeftPos;
INT         yTopPos;
POINT       maxFrameSize;

// coords for drawing on capture window
INT         xDrawing;
INT         yDrawing;
INT         xDrawingOld;
INT         yDrawingOld;

INT			timer = 0;

INT			toolId = 0;

LPCTSTR lpText = TEXT("Use Ctrl + F7 to show/hide this app.\nUse mouse to select screen area you want to capture.\nUse Ctrl + Space to make screenshot of selected area.\nHand-tool is used for changing capture area position. Also you can change size of this area. Other tools are used for drawing.\nYou can choose color of pen/rect/ellipse by clicking 'COLOR'\nYou can set delay time for your screenshot.\nSave your screenshot by clicking 'SAVE' button\nYou can also use clipboard with Ctrl + C.");