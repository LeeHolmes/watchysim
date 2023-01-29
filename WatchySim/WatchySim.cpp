#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <Shlwapi.h>
#include <time.h>

#include "Resource.h"

#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib, "Shlwapi.lib")

#include "Watchy.h"

//#include "WatchFaces/7_SEG/Watchy_7_SEG.h"
//#include "WatchFaces/AnalogGabel/Watchy_AnalogGabel.h"
//#include "WatchFaces/DOS/Watchy_DOS.h"
//#include "WatchFaces/MacPaint/Watchy_MacPaint.h"
//#include "WatchFaces/Mario/Watchy_Mario.h"
//#include "WatchFaces/Pokemon/Watchy_Pokemon.h"
//#include "WatchFaces/PowerShell/Watchy_PowerShell.h"
//#include "WatchFaces/Tetris/Watchy_Tetris.h"

//#include "WatchFaces/DrawTest/Watchy_Draw_Test.h"
//#include "WatchFaces/Niobe/niobe.h"
//#include "WatchFaces/Scene/Watchy_scene.h"


Watchy watchy = Watchy();  // Basic
//Watchy7SEG watchy = Watchy7SEG();
//WatchyAnalogGabel watchy = WatchyAnalogGabel();
//WatchyDOS watchy = WatchyDOS();
//WatchyMacPaint watchy = WatchyMacPaint();
//WatchyMario watchy = WatchyMario();
//WatchyPowerShell watchy = WatchyPowerShell();
//WatchyPokemon watchy = WatchyPokemon();
//WatchyTetris watchy = WatchyTetris();

//WatchyDrawTest watchy = WatchyDrawTest();
//Niobe watchy = Niobe();
//Scene watchy = Scene();


HRSRC myResource;
HGLOBAL myResourceData;
unsigned int myResourceSize;

LPSTREAM pStream;
Image *image;

BOOL WINAPI SaveBitmap(HWND hWnd);

wchar_t szUserInput[80];
BOOL CALLBACK InputBoxProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
int getDayOfWeek(int y, int m, int d);

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
        watchy.setTime(tm_local);

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
            return 0;


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
            return 0;

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
            return 0;

        case ID_TIME_CUSTOM:
            wcscpy_s(szUserInput, 80, L"03/14/2021 16:20");
            if (DialogBox(NULL,
                MAKEINTRESOURCE(IDD_INPUT_BOX),
                hWnd,
                (DLGPROC)InputBoxProc) == IDOK)
            {
                curr_time = time(NULL);
                localtime_s(&tm_local, &curr_time);

                int parsed = swscanf_s(szUserInput, L"%2d/%2d/%4d %2d:%2d",
                    &tm_local.tm_mon, &tm_local.tm_mday, &tm_local.tm_year, &tm_local.tm_hour, &tm_local.tm_min);
                if (parsed == 5)
                {
                    tm_local.tm_wday = getDayOfWeek(tm_local.tm_year, tm_local.tm_mon, tm_local.tm_mday);
                    tm_local.tm_year -= 1900;
                    tm_local.tm_mon++;

                    watchy.setTime(tm_local);

                    InvalidateRect(hWnd, NULL, false);
                    PostMessage(hWnd, WM_PAINT, 0, 0);
                }
                else
                {
                    MessageBox(hWnd, L"Please enter a date in the form of 'MM/DD/YYYY HH:MM'", L"Could not understand date", NULL);
                }
            }
            return 0;

        case ID_BATTERY_DEAD:
            watchy.setBatteryVoltage(0.0f);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_BATTERY_LOW:
            watchy.setBatteryVoltage(3.7f);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_BATTERY_MEDIUM:
            watchy.setBatteryVoltage(3.81f);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_BATTERY_HIGH:
            watchy.setBatteryVoltage(3.96f);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_BATTERY_MAX:
            watchy.setBatteryVoltage(4.2f);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_BLUETOOTH_ON:
            watchy.setBluetooth(true);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_BLUETOOTH_OFF:
            watchy.setBluetooth(false);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WIFI_ON:
            watchy.setWifi(true);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WIFI_OFF:
            watchy.setWifi(false);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_STEPS_NONE:
            watchy.setSteps(0);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_STEPS_LAZY:
            watchy.setSteps(12);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_STEPS_REGULAR:
            watchy.setSteps(5280);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_STEPS_ATHLETE:
            watchy.setSteps(52769);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WEATHER_CLOUDY:
            watchy.setWeatherCode(802);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WEATHER_FEWCLOUDS:
            watchy.setWeatherCode(801);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WEATHER_CLEAR:
            watchy.setWeatherCode(800);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WEATHER_ATMOSPHERE:
            watchy.setWeatherCode(750);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WEATHER_SNOW:
            watchy.setWeatherCode(650);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WEATHER_RAIN:
            watchy.setWeatherCode(550);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WEATHER_DRIZZLE:
            watchy.setWeatherCode(350);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WEATHER_THUNDERSTORM:
            watchy.setWeatherCode(250);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_WEATHER_OTHER:
            watchy.setWeatherCode(150);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_TEMPERATURE_CELSIUS:
            watchy.setTemperatureUnitMetric(true);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_TEMPERATURE_FAHRENHEIT:
            watchy.setTemperatureUnitMetric(false);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_TEMPERATURE_CANADA:
            watchy.setTemperature(-45);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_TEMPERATURE_CHILLY:
            watchy.setTemperature(7);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_TEMPERATURE_WARM:
            watchy.setTemperature(15);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

        case ID_TEMPERATURE_INFERNO:
            watchy.setTemperature(40);
            InvalidateRect(hWnd, NULL, false);
            PostMessage(hWnd, WM_PAINT, 0, 0);

            return 0;

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

BOOL CALLBACK InputBoxProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        SetFocus(GetDlgItem(hwndDlg, IDC_EDIT_USERTEXT));
        SetDlgItemText(hwndDlg, IDC_EDIT_USERTEXT, szUserInput);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            if (!GetDlgItemText(hwndDlg, IDC_EDIT_USERTEXT, szUserInput, 80))
            {
                *szUserInput = 0;
            }

            // Fall through. 

        case IDCANCEL:
            EndDialog(hwndDlg, wParam);
            return TRUE;
        }
    }
    return FALSE;
}
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

int getDayOfWeek(int y, int m, int d)
{
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    if (m < 3)
    {
        y -= 1;
    }
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}
