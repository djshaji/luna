#include "main_window.h"
#include "../lv2_host.h"
#include "plugin_list.h"
#include <commctrl.h>

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
    // Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    
    if (!RegisterClass(&wc)) {
        return false;
    }
    
    // Create window
    hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Luna LV2 Host",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1024, 768,
        nullptr,
        nullptr,
        hInstance,
        this
    );
    
    if (!hWnd) {
        return false;
    }
    
    // Initialize LV2 host
    if (!lv2Host->Initialize()) {
        MessageBox(hWnd, L"Failed to initialize LV2 host", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }
    
    CreateControls();
    UpdateStatus(L"Ready - LV2 Host initialized");
    return true;
}

void MainWindow::Show(int nCmdShow) {
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    MainWindow* pThis = nullptr;
    
    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    } else {
        pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }
    
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