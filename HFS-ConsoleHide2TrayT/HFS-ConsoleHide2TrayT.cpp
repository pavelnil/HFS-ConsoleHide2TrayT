#include <windows.h>
#include <cstring>
#include "resource.h"

#pragma comment(lib, "user32.lib")

HWND g_hwnd = nullptr;
HMENU g_hMenu = nullptr;
NOTIFYICONDATA g_trayData = {};
HICON g_hIcon = nullptr;

HWND FindHfsConsoleWindow() {
    const char* prefix = "HFS_CONSOLE_";
    HWND hwnd = nullptr;
    
    while ((hwnd = FindWindowExA(nullptr, hwnd, nullptr, nullptr)) != nullptr) {
        char title[256];
        if (GetWindowTextA(hwnd, title, sizeof(title)) > 0) {
            if (strstr(title, prefix) != nullptr) {
                return hwnd;
            }
        }
    }
    return nullptr;
}

void ToggleVisibility() {
    HWND consoleHwnd = FindHfsConsoleWindow();
    if (!consoleHwnd) return;
    
    bool isVisible = IsWindowVisible(consoleHwnd);
    
    if (isVisible) {
        ShowWindow(consoleHwnd, SW_HIDE);
        ModifyMenu(g_hMenu, 1, MF_BYCOMMAND | MF_STRING, 1, "Show HFS console");
    } else {
        ShowWindow(consoleHwnd, SW_SHOW);
        SetForegroundWindow(consoleHwnd);
        ModifyMenu(g_hMenu, 1, MF_BYCOMMAND | MF_STRING, 1, "Hide HFS console");
    }
    DrawMenuBar(g_hwnd);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_USER + 1:
            if (lParam == WM_RBUTTONUP) {
                POINT pt;
                GetCursorPos(&pt);
                SetForegroundWindow(hwnd);
                TrackPopupMenu(g_hMenu, TPM_BOTTOMALIGN | TPM_RIGHTBUTTON, 
                              pt.x, pt.y, 0, hwnd, nullptr);
            }
            else if (lParam == WM_LBUTTONDBLCLK) {
                ToggleVisibility();
            }
            break;
        
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                ToggleVisibility();
            }
            else if (LOWORD(wParam) == 2) {
                DestroyWindow(hwnd);
            }
            break;
            
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &g_trayData);
            if (g_hIcon) DestroyIcon(g_hIcon);
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

HICON CreateTrayIcon() {
    HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_TRAY_ICON));
    if (!hIcon) {
        hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    }
    return hIcon;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "HFS_ConsoleHide2TrayT";
    if (!RegisterClass(&wc)) {
        return 1;
    }
    
    g_hwnd = CreateWindow(wc.lpszClassName, "HFS-ConsoleHide2TrayT", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);
    if (!g_hwnd) {
        return 1;
    }
    
    ShowWindow(g_hwnd, SW_HIDE);
    
    g_hMenu = CreatePopupMenu();
    AppendMenu(g_hMenu, MF_STRING, 1, "Hide HFS console");
    AppendMenu(g_hMenu, MF_STRING, 2, "Exit");
    
    g_hIcon = CreateTrayIcon();
    if (!g_hIcon) {
        return 1;
    }
    
    ZeroMemory(&g_trayData, sizeof(g_trayData));
    g_trayData.cbSize = sizeof(NOTIFYICONDATA);
    g_trayData.hWnd = g_hwnd;
    g_trayData.uID = 1;
    g_trayData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    g_trayData.uCallbackMessage = WM_USER + 1;
    g_trayData.hIcon = g_hIcon;
    strncpy_s(g_trayData.szTip, "HFS-ConsoleHide2TrayT", sizeof(g_trayData.szTip));
    
    if (!Shell_NotifyIcon(NIM_ADD, &g_trayData)) {
        return 1;
    }
    
    HWND consoleHwnd = FindHfsConsoleWindow();
    if (consoleHwnd) {
        if (IsWindowVisible(consoleHwnd)) {
            ShowWindow(consoleHwnd, SW_HIDE);
            ModifyMenu(g_hMenu, 1, MF_BYCOMMAND | MF_STRING, 1, "Show HFS console");
        } else {
            ModifyMenu(g_hMenu, 1, MF_BYCOMMAND | MF_STRING, 1, "Show HFS console");
        }
        DrawMenuBar(g_hwnd);
    }
    
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}