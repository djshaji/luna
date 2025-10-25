#ifndef CONTROLS_H
#define CONTROLS_H

#include <windows.h>

class Controls {
public:
    static HWND CreateButton(HWND parent, const wchar_t* text, int x, int y, int width, int height, int id);
    static HWND CreateLabel(HWND parent, const wchar_t* text, int x, int y, int width, int height);
    static HWND CreateSlider(HWND parent, int x, int y, int width, int height, int id);
    
private:
    Controls() = delete;
};

#endif // CONTROLS_H