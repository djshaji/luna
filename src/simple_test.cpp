#include <windows.h>
#include <iostream>

LRESULT CALLBACK SimpleWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            TextOut(hdc, 10, 10, L"Simple Test Window", 18);
            EndPaint(hWnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    // Allocate console for output
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
    
    std::cout << "Simple Window Test Starting..." << std::endl;
    std::cout << "hInstance: " << hInstance << std::endl;
    
    const wchar_t* CLASS_NAME = L"SimpleTestWindow";
    
    // Register window class
    WNDCLASSW wc = {};
    wc.lpfnWndProc = SimpleWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    
    std::cout << "Registering window class..." << std::endl;
    if (!RegisterClassW(&wc)) {
        DWORD error = GetLastError();
        std::cout << "RegisterClassW failed with error: " << error << std::endl;
        if (error != ERROR_CLASS_ALREADY_EXISTS) {
            std::cout << "Fatal error - cannot register window class" << std::endl;
            system("pause");
            return -1;
        }
        std::cout << "Class already exists, continuing..." << std::endl;
    } else {
        std::cout << "Window class registered successfully" << std::endl;
    }
    
    // Verify class registration
    WNDCLASSW testClass = {};
    if (GetClassInfoW(hInstance, CLASS_NAME, &testClass)) {
        std::cout << "Window class verification successful" << std::endl;
    } else {
        std::cout << "Warning: Cannot verify window class" << std::endl;
    }
    
    // Create window
    std::cout << "Creating simple test window..." << std::endl;
    SetLastError(0);
    
    HWND hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Simple Test Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400, 300,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
    
    if (!hWnd) {
        DWORD error = GetLastError();
        std::cout << "CreateWindowExW failed with error: " << error << std::endl;
        
        switch (error) {
            case 1400:
                std::cout << "ERROR_INVALID_WINDOW_HANDLE" << std::endl;
                break;
            case 1407:
                std::cout << "ERROR_CANNOT_FIND_WND_CLASS" << std::endl;
                break;
            case 8:
                std::cout << "ERROR_NOT_ENOUGH_MEMORY" << std::endl;
                break;
            default:
                std::cout << "Unknown error" << std::endl;
                break;
        }
        
        std::cout << "Press any key to exit..." << std::endl;
        system("pause");
        return -1;
    }
    
    std::cout << "Window created successfully!" << std::endl;
    std::cout << "Window handle: " << hWnd << std::endl;
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    std::cout << "Window shown, entering message loop..." << std::endl;
    
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    std::cout << "Application exiting..." << std::endl;
    return 0;
}