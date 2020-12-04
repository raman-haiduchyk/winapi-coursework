#include "InitToolControls.h"

COLORREF    acrCustClr[16];

VOID CreateToolWndControls(HWND hWnd, HINSTANCE hInst, HWND* hSaveBtn, HWND* hToolBox, OPENFILENAME* ofn, CHOOSECOLOR* cc)
{

#pragma region Create buttons

    //Creating "HELP" button
    HWND hHelpBtn = CreateWindow(
        L"Button",
        L"HELP",
        WS_CHILD | WS_VISIBLE | BS_FLAT,
        0,
        0,
        50,
        50,
        hWnd,
        (HMENU)IDC_HELP_BUTTON,
        hInst,
        NULL
    );

    *hSaveBtn = CreateWindow(
        L"Button",
        L"SAVE",
        WS_CHILD | WS_VISIBLE | BS_FLAT | WS_DISABLED,
        50,
        0,
        50,
        50,
        hWnd,
        (HMENU)IDC_SAVE_BUTTON,
        hInst,
        NULL
    );

    HWND hColorBtn = CreateWindow(
        L"Button",
        L"COLOR",
        WS_CHILD | WS_VISIBLE | BS_FLAT,
        300,
        0,
        70,
        50,
        hWnd,
        (HMENU)IDC_COLOR_BUTTON,
        hInst,
        NULL
    );


#pragma endregion

#pragma region Create comboboxes

    HWND hDelayComboBox = CreateWindow(
        WC_COMBOBOX,
        TEXT(""),
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
        100, 25, 100, 500,
        hWnd,
        (HMENU)IDC_TIMER_COMBOBOX,
        hInst,
        NULL
    );

    TCHAR DelayBoxText[6][10] =
    {
        TEXT("No Delay"), TEXT("1 second"), TEXT("2 seconds"), TEXT("3 second"), TEXT("4 seconds"), TEXT("5 seconds")
    };

    for (int k = 0; k < 6; k++)
    {
        // Add string to combobox.
        SendMessage(hDelayComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)DelayBoxText[k]);
    }
    // Send the CB_SETCURSEL message to display an initial item 
    //  in the selection field  
    SendMessage(hDelayComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);


    *hToolBox = CreateWindow(
        WC_COMBOBOX,
        TEXT(""),
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
        200, 25, 100, 500,
        hWnd,
        (HMENU)IDC_TOOL_COMBOBOX,
        hInst,
        NULL
    );

    TCHAR ToolBoxText[4][10] =
    {
        TEXT("Hand"), TEXT("Pen"), TEXT("Rect"), TEXT("Ellipse")
    };

    for (int k = 0; k < 4; k++)
    {
        // Add string to combobox.
        SendMessage(*hToolBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)ToolBoxText[k]);
    }
    // Send the CB_SETCURSEL message to display an initial item 
    //  in the selection field  
    SendMessage(*hToolBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

#pragma endregion

#pragma region Create static text

    HWND hDelayStaticText = CreateWindow(
        TEXT("STATIC"),
        NULL,
        WS_CHILD | WS_VISIBLE,
        100, 0, 100, 25,
        hWnd,
        NULL,
        hInst,
        NULL
    );

    SetWindowText(hDelayStaticText, TEXT("     Delay:"));

    HWND hToolStaticText = CreateWindow(
        TEXT("STATIC"),
        NULL,
        WS_CHILD | WS_VISIBLE,
        200, 0, 100, 25,
        hWnd,
        NULL,
        hInst,
        NULL
    );

    SetWindowText(hToolStaticText, TEXT("     Tool:"));

#pragma endregion

#pragma region Init OPENFILENAME

    TCHAR szFileName[MAX_PATH] = TEXT("");

    ZeroMemory(ofn, sizeof(OPENFILENAME));

    ofn->lStructSize = sizeof(OPENFILENAME);
    ofn->hwndOwner = hWnd;
    ofn->lpstrFilter = TEXT("Bitmap files(*.bmp)\0*.bmp\0JPEG files(*.jpg)\0*.jpg\0PNG files(*.png)\0*.png\0\0");
    ofn->lpstrFile = szFileName;
    ofn->nMaxFile = MAX_PATH;
    ofn->Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT;
    ofn->lpstrDefExt = TEXT("png");

#pragma endregion

#pragma region Init CHOOSECOLOR

    ZeroMemory(cc, sizeof(CHOOSECOLOR));

    cc->lStructSize = sizeof(CHOOSECOLOR);

    cc->hwndOwner = hWnd;
    cc->rgbResult = RGB(255, 0, 0);
    cc->lpCustColors = NULL;
    cc->Flags = CC_FULLOPEN | CC_RGBINIT;
    cc->lpCustColors = acrCustClr;

#pragma endregion

}