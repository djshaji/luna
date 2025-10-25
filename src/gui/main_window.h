#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <windows.h>
#include <memory>

class LV2Host;
class PluginList;

class MainWindow {
public:
    MainWindow(HINSTANCE hInstance);
    ~MainWindow();
    
    bool Create();
    void Show(int nCmdShow);
    HWND GetHandle() const { return hWnd; }
    
private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    void CreateControls();
    void OnSize(int width, int height);
    void OnCommand(WPARAM wParam, LPARAM lParam);
    void ToggleAudio();
    void UpdateAudioButton();
    void UpdateStatus(const wchar_t* message);
    
    HINSTANCE hInstance;
    HWND hWnd;
    HWND hAudioToggle;
    HWND hStatusBar;
    
    std::unique_ptr<LV2Host> lv2Host;
    std::unique_ptr<PluginList> pluginList;
    
    static const wchar_t* CLASS_NAME;
    static const int ID_AUDIO_TOGGLE = 1001;
};

#endif // MAIN_WINDOW_H