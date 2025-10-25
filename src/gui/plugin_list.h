#ifndef PLUGIN_LIST_H
#define PLUGIN_LIST_H

#include <windows.h>
#include <vector>
#include <string>

class PluginManager;

class PluginList {
public:
    PluginList(HWND parent, HINSTANCE hInstance);
    ~PluginList();
    
    bool Create(int x, int y, int width, int height);
    void Resize(int x, int y, int width, int height);
    void RefreshList(PluginManager* pluginManager);
    
    HWND GetHandle() const { return hListBox; }
    
private:
    HWND hParent;
    HWND hListBox;
    HINSTANCE hInstance;
};

#endif // PLUGIN_LIST_H