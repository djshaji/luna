#ifndef PLUGIN_CHAIN_H
#define PLUGIN_CHAIN_H

#include <windows.h>
#include <vector>
#include <string>
#include "../plugin_manager.h"

class PluginChain {
public:
    PluginChain(HWND parent, HINSTANCE hInstance);
    ~PluginChain();
    
    bool Create(int x, int y, int width, int height);
    void Resize(int x, int y, int width, int height);
    void AddPlugin(const PluginInfo& plugin);
    void RemovePlugin(int index);
    void ClearChain();
    
    HWND GetHandle() const { return hListBox; }
    const std::vector<PluginInfo>& GetActivePlugins() const { return activePlugins; }
    
private:
    static LRESULT CALLBACK ListBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void RefreshDisplay();
    void OnDoubleClick();
    void OnRightClick(int x, int y);
    
    HWND hParent;
    HWND hListBox;
    HWND hTitleLabel;
    HINSTANCE hInstance;
    WNDPROC originalListBoxProc;
    
    std::vector<PluginInfo> activePlugins;
};

#endif // PLUGIN_CHAIN_H