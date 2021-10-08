#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <Shlwapi.h>
#include <time.h>

#include "Resource.h"

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib, "Shlwapi.lib")

//#include "Watchy_DOS.h"
//#include "Watchy_Tetris.h"
//#include "Watchy_MacPaint.h"
//#include "Watchy_Pokemon.h"
//#include "Watchy_AnalogGabel.h"
#include "Watchy.h"
#include "Watchy_PowerShell.h"

//WatchyDOS watchy = WatchyDOS();
//WatchyTetris watchy = WatchyTetris();
//WatchyMacPaint watchy = WatchyMacPaint();
//WatchyPokemon watchy = WatchyPokemon();
//WatchyAnalogGabel watchy = WatchyAnalogGabel();
WatchyPowerShell watchy = WatchyPowerShell();

HRSRC myResource;
HGLOBAL myResourceData;
unsigned int myResourceSize;

LPSTREAM pStream;
Image *image;

BOOL WINAPI SaveBitmap(HWND hWnd);

VOID OnPaint(HDC hdc)
{
    if (image == NULL)
    {
        myResource = ::FindResource(NULL, MAKEINTRESOURCE(IDR_BACKGROUND), RT_RCDATA);
        myResourceData = ::LoadResource(NULL, myResource);
        myResourceSize = ::SizeofResource(NULL, myResource);

        pStream = SHCreateMemStream((LPBYTE)LockResource(myResourceData), myResourceSize);
        image = new Image(pStream);
    }

    Graphics graphics(hdc);
    graphics.DrawImage(image, 0, 0);

    watchy.showWatchFace(&graphics, &hdc);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WATCHYSIM));

    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = MAKEINTRESOURCE(IDC_WATCHYSIM);
    wndClass.lpszClassName = TEXT("WatchySim");

    RegisterClass(&wndClass);

    RECT clientRect;
    clientRect.left = clientRect.top = 0;
    clientRect.right = 676;
    clientRect.bottom = 676;
    
    AdjustWindowRect(&clientRect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false);

    hWnd = CreateWindow(
        TEXT("WatchySim"),
        TEXT("WatchySim"),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WATCHYSIM));
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
   
    // Set the watch face timer for every second, but only
    // repaint every minute.
    SetTimer(hWnd, 1, 1000, NULL);

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return (int) msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_TIMER:
    {
        time_t curr_time = time(NULL);
        struct tm tm_local;
        localtime_s(&tm_local, &curr_time);
        if (tm_local.tm_sec != 0)
        {
            return 0;
        }
        else
        {
            InvalidateRect(hWnd, NULL, false);
            // Intentional fall-through to WM_PAINT
        }
    }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_TIME_CURRENT:
            watchy.resetTime();

            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);
            return 0;;


        case ID_TIME_SHORT:
            time_t curr_time;
            curr_time = time(NULL);

            struct tm tm_local;
            localtime_s(&tm_local, &curr_time);
            tm_local.tm_hour = 01;
            tm_local.tm_mday = 01;
            tm_local.tm_min = 01;
            tm_local.tm_mon = 04;
            tm_local.tm_sec = 01;
            tm_local.tm_wday = 0;
            tm_local.tm_year = 2011 - 1900;

            watchy.setTime(tm_local);

            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);
            return 0;;

        case ID_TIME_LONG:
            curr_time = time(NULL);
            localtime_s(&tm_local, &curr_time);
            tm_local.tm_hour = 18;
            tm_local.tm_mday = 31;
            tm_local.tm_min = 33;
            tm_local.tm_mon = 8;
            tm_local.tm_sec = 01;
            tm_local.tm_wday = 3;
            tm_local.tm_year = 2099 - 1900;

            watchy.setTime(tm_local);

            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);
            return 0;;

        case ID_TOOLS_SCREENSHOT:

            SaveBitmap(hWnd);

            return 0;;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc

BOOL WINAPI SaveBitmap(HWND hWnd)
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    wchar_t filename[MAX_PATH] = { 0 };

    ofn.lStructSize = sizeof(ofn);
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.hwndOwner = hWnd;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFile = filename;
    ofn.nFilterIndex = 1;
    ofn.lpstrFilter = L"GIF Files (*.gif)\0*.gif\0\0";
    ofn.lpstrDefExt = L"gif";
    GetSaveFileName(&ofn);

    if (wcsnlen_s(filename, MAX_PATH) > 0)
    {
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        int x1, y1, x2, y2;

        // get screen dimensions
        x1 = clientRect.left + DISPLAY_OFFSET_X;
        y1 = clientRect.top + DISPLAY_OFFSET_Y;
        x2 = clientRect.left + DISPLAY_WIDTH;
        y2 = clientRect.bottom + DISPLAY_HEIGHT;

        // copy screen to bitmap
        HDC     hScreen = GetDC(hWnd);
        HDC     hDC = CreateCompatibleDC(hScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
        HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
        BOOL    bRet = BitBlt(hDC, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, hScreen, x1, y1, SRCCOPY);

        CLSID gifClsid;
        CLSIDFromString(L"{557cf402-1a04-11d3-9a73-0000f81ef32e}", &gifClsid);

        Bitmap newBitmap(hBitmap, NULL);
        newBitmap.Save(filename, &gifClsid, NULL);

        // clean up
        SelectObject(hDC, old_obj);
        DeleteDC(hDC);
        ReleaseDC(NULL, hScreen);
        DeleteObject(hBitmap);
    }

    return TRUE;
}