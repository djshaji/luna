#include <windows.h>
#include <objbase.h>
#include <iostream>
#include "lv2_host.h"
#include "gui/main_window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    
    try {
        // Allocate console for debugging (will be ignored if not running from console)
        AllocConsole();
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
        
        std::cout << "Luna LV2 Host starting..." << std::endl;
        
        // Initialize COM
        CoInitialize(nullptr);
        std::cout << "COM initialized" << std::endl;
        
        // Create the main window
        std::cout << "Creating main window..." << std::endl;
        MainWindow mainWindow(hInstance);
        if (!mainWindow.Create()) {
            DWORD error = GetLastError();
            std::cout << "Failed to create main window. Error code: " << error << std::endl;
            wchar_t errorMsg[512];
            swprintf(errorMsg, 512, L"Failed to create main window. Error code: %lu", error);
            MessageBox(nullptr, errorMsg, L"Error", MB_OK | MB_ICONERROR);
            CoUninitialize();
            return -1;
        }
        
        std::cout << "Main window created successfully" << std::endl;
        
        // Show the window
        std::cout << "Showing window..." << std::endl;
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