#include <windows.h>
#include <objbase.h>
#include <iostream>
#include "lv2_host.h"
#include "gui/main_window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    
    try {
        // Initialize COM
        CoInitialize(nullptr);
        
        // Create the main window
        MainWindow mainWindow(hInstance);
        if (!mainWindow.Create()) {
            MessageBox(nullptr, L"Failed to create main window", L"Error", MB_OK | MB_ICONERROR);
            CoUninitialize();
            return -1;
        }
        
        // Show the window
        mainWindow.Show(nCmdShow);
        
        // Message loop
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        CoUninitialize();
        return static_cast<int>(msg.wParam);
        
    } catch (const std::exception& e) {
        std::string error = "Application error: ";
        error += e.what();
        MessageBoxA(nullptr, error.c_str(), "Luna LV2 Host - Error", MB_OK | MB_ICONERROR);
        CoUninitialize();
        return -1;
    }
}