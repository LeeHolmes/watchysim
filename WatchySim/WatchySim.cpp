#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <Shlwapi.h>

#include "Resource.h"

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib, "Shlwapi.lib")

//#include "Watchy_DOS.h"
//#include "Watchy_Tetris.h"
//#include "Watchy_MacPaint.h"
//#include "Watchy_Pokemon.h"
//#include "Watchy_AnalogGabel.h"
#include "Watchy_PowerShell.h"

//WatchyDOS watchy = WatchyDOS();
//WatchyTetris watchy = WatchyTetris();
//WatchyMacPaint watchy = WatchyMacPaint();
//WatchyPokemon watchy = WatchyPokemon();
//WatchyAnalogGabel watchy = WatchyAnalogGabel();
WatchyPowerShell watchy = WatchyPowerShell();

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);

    HRSRC myResource = ::FindResource(NULL, MAKEINTRESOURCE(IDR_BACKGROUND), RT_RCDATA);
    HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
    unsigned int myResourceSize = ::SizeofResource(NULL, myResource);

    LPSTREAM pStream = SHCreateMemStream((LPBYTE)LockResource(myResourceData), myResourceSize);
    Image image(pStream);
    graphics.DrawImage(&image, 0, 0);

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
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("WatchySim");

    RegisterClass(&wndClass);

    RECT clientRect;
    clientRect.left = clientRect.top = 0;
    clientRect.right = 659;
    clientRect.bottom = 437;
    
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

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
} // WndProc