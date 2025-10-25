#include "controls.h"
#include <commctrl.h>

HWND Controls::CreateButton(HWND parent, const wchar_t* text, int x, int y, int width, int height, int id) {
    return CreateWindow(
        L"BUTTON",
        text,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x, y, width, height,
        parent,
        reinterpret_cast<HMENU>(id),
        GetModuleHandle(nullptr),
        nullptr
    );
}

HWND Controls::CreateLabel(HWND parent, const wchar_t* text, int x, int y, int width, int height) {
    return CreateWindow(
        L"STATIC",
        text,
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        x, y, width, height,
        parent,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
}

HWND Controls::CreateSlider(HWND parent, int x, int y, int width, int height, int id) {
    return CreateWindow(
        TRACKBAR_CLASS,
        nullptr,
        WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS,
        x, y, width, height,
        parent,
        reinterpret_cast<HMENU>(id),
        GetModuleHandle(nullptr),
        nullptr
    );
}