#include "main_window.h"
#include "../lv2_host.h"
#include "plugin_list.h"
#include <commctrl.h>
#include <iostream>

const wchar_t* MainWindow::CLASS_NAME = L"LunaLV2Host";

MainWindow::MainWindow(HINSTANCE hInstance) 
    : hInstance(hInstance), hWnd(nullptr), hAudioToggle(nullptr), hStatusBar(nullptr) {
    
    lv2Host = std::make_unique<LV2Host>();
}

MainWindow::~MainWindow() {
    if (lv2Host) {
        lv2Host->Shutdown();
    }
}

bool MainWindow::Create() {
    std::cout << "Registering window class..." << std::endl;
    
    // Validate hInstance
    if (!hInstance) {
        std::cout << "Error: Invalid hInstance" << std::endl;
        return false;
    }
    
    // Register window class using explicit Unicode version
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.style = CS_HREDRAW | CS_VREDRAW; // Add standard class styles
    
    if (!RegisterClassW(&wc)) {
        // Check if the class is already registered (this is okay)
        DWORD error = GetLastError();
        std::cout << "RegisterClassW failed with error: " << error << std::endl;
        if (error != ERROR_CLASS_ALREADY_EXISTS) {
            return false;
        }
        std::cout << "Window class already exists, continuing..." << std::endl;
    } else {
        std::cout << "Window class registered successfully" << std::endl;
    }
    
    // Verify the class exists by trying to get its info
    WNDCLASSW testClass = {};
    if (GetClassInfoW(hInstance, CLASS_NAME, &testClass)) {
        std::cout << "Window class verification successful" << std::endl;
    } else {
        std::cout << "Warning: Cannot verify window class registration" << std::endl;
    }
    
    // Create window using explicit Unicode version
    std::cout << "Creating window with CreateWindowExW..." << std::endl;
    std::cout << "Class name: LunaLV2Host" << std::endl;
    std::cout << "hInstance: " << hInstance << std::endl;
    
    // Clear any previous errors
    SetLastError(0);
    
    hWnd = CreateWindowExW(
        0,                          // dwExStyle
        CLASS_NAME,                 // lpClassName
        L"Luna LV2 Host",          // lpWindowName
        WS_OVERLAPPEDWINDOW,       // dwStyle
        CW_USEDEFAULT,             // X
        CW_USEDEFAULT,             // Y
        1024,                      // nWidth
        768,                       // nHeight
        nullptr,                   // hWndParent
        nullptr,                   // hMenu
        hInstance,                 // hInstance
        nullptr                    // lpParam - don't pass 'this' initially
    );
    
    if (!hWnd) {
        DWORD error = GetLastError();
        std::cout << "CreateWindowExW failed with error: " << error << std::endl;
        
        // Provide more specific error information
        switch (error) {
            case 1400: // ERROR_INVALID_WINDOW_HANDLE
                std::cout << "ERROR_INVALID_WINDOW_HANDLE - Invalid parameter or class not found" << std::endl;
                break;
            case 1407: // ERROR_CANNOT_FIND_WND_CLASS
                std::cout << "ERROR_CANNOT_FIND_WND_CLASS - Window class not registered" << std::endl;
                break;
            case 8: // ERROR_NOT_ENOUGH_MEMORY
                std::cout << "ERROR_NOT_ENOUGH_MEMORY - Insufficient memory" << std::endl;
                break;
            default:
                std::cout << "Unknown error code" << std::endl;
                break;
        }
        
        wchar_t errorMsg[512];
        swprintf(errorMsg, 512, L"CreateWindowExW failed. Error code: %lu\nCheck console for details.", error);
        MessageBoxW(nullptr, errorMsg, L"Window Creation Error", MB_OK | MB_ICONERROR);
        return false;
    }
    
    std::cout << "Window created successfully, handle: " << hWnd << std::endl;
    
    // Set up window user data manually since we didn't pass 'this' in CreateWindowEx
    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    
    // Initialize LV2 host (non-blocking - allow window to show even if this fails)
    bool lv2InitSuccess = lv2Host->Initialize();
    
    CreateControls();
    
    if (lv2InitSuccess) {
        UpdateStatus(L"Ready - LV2 Host initialized");
    } else {
        UpdateStatus(L"Warning: LV2 Host initialization failed");
        MessageBox(hWnd, L"LV2 Host initialization failed. Some features may not work.", L"Warning", MB_OK | MB_ICONWARNING);
    }
    
    return true;
}

void MainWindow::Show(int nCmdShow) {
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    if (pThis) {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            return 0;
            
        case WM_SIZE:
            OnSize(LOWORD(lParam), HIWORD(lParam));
            return 0;
            
        case WM_COMMAND:
            OnCommand(wParam, lParam);
            return 0;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void MainWindow::CreateControls() {
    // Initialize common controls
    INITCOMMONCONTROLSEX icc = {};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icc);
    
    // Create audio toggle button
    hAudioToggle = CreateWindow(
        L"BUTTON",
        L"Start Audio",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 100, 30,
        hWnd,
        reinterpret_cast<HMENU>(ID_AUDIO_TOGGLE),
        hInstance,
        nullptr
    );
    
    // Create status bar
    hStatusBar = CreateWindow(
        STATUSCLASSNAME,
        nullptr,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
        0, 0, 0, 0,
        hWnd,
        nullptr,
        hInstance,
        nullptr
    );
    
    // Create plugin list
    pluginList = std::make_unique<PluginList>(hWnd, hInstance);
    pluginList->Create(10, 50, 200, 500);
    
    // Refresh plugin list
    if (lv2Host && lv2Host->GetPluginManager()) {
        pluginList->RefreshList(lv2Host->GetPluginManager());
    }
}

void MainWindow::OnSize(int width, int height) {
    // Resize status bar
    if (hStatusBar) {
        SendMessage(hStatusBar, WM_SIZE, 0, 0);
    }
    
    // Resize plugin list
    if (pluginList) {
        pluginList->Resize(10, 50, 200, height - 100);
    }
}

void MainWindow::OnCommand(WPARAM wParam, LPARAM) {
    int id = LOWORD(wParam);
    
    switch (id) {
        case ID_AUDIO_TOGGLE:
            ToggleAudio();
            break;
    }
}

void MainWindow::ToggleAudio() {
    if (lv2Host->IsAudioRunning()) {
        lv2Host->StopAudio();
        UpdateStatus(L"Audio stopped");
    } else {
        if (lv2Host->StartAudio()) {
            UpdateStatus(L"Audio running");
        } else {
            UpdateStatus(L"Failed to start audio");
            MessageBox(hWnd, L"Failed to start audio engine", L"Error", MB_OK | MB_ICONWARNING);
        }
    }
    
    UpdateAudioButton();
}

void MainWindow::UpdateAudioButton() {
    if (hAudioToggle) {
        const wchar_t* text = lv2Host->IsAudioRunning() ? L"Stop Audio" : L"Start Audio";
        SetWindowText(hAudioToggle, text);
    }
}

void MainWindow::UpdateStatus(const wchar_t* message) {
    if (hStatusBar) {
        SendMessage(hStatusBar, SB_SETTEXT, 0, reinterpret_cast<LPARAM>(message));
    }
}