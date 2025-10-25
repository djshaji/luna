#include "plugin_list.h"
#include "../plugin_manager.h"
#include <algorithm>

PluginList::PluginList(HWND parent, HINSTANCE hInstance) 
    : hParent(parent), hListBox(nullptr), hInstance(hInstance) {
}

PluginList::~PluginList() {
}

bool PluginList::Create(int x, int y, int width, int height) {
    hListBox = CreateWindow(
        L"LISTBOX",
        nullptr,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_NOTIFY,
        x, y, width, height,
        hParent,
        nullptr,
        hInstance,
        nullptr
    );
    
    if (!hListBox) {
        return false;
    }
    
    return true;
}

void PluginList::Resize(int x, int y, int width, int height) {
    if (hListBox) {
        SetWindowPos(hListBox, nullptr, x, y, width, height, SWP_NOZORDER);
    }
}

void PluginList::RefreshList(PluginManager* pluginManager) {
    if (!hListBox) {
        return;
    }
    
    // Clear existing items
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
    
    if (!pluginManager) {
        // Add placeholder if no plugin manager
        SendMessage(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"No plugins available"));
        return;
    }
    
    // Get plugin list from manager
    auto plugins = pluginManager->GetPluginList();
    
    if (plugins.empty()) {
        SendMessage(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"No LV2 plugins found"));
        return;
    }
    
    // Add actual plugins
    for (const auto& plugin : plugins) {
        // Convert std::string to wide string
        int len = MultiByteToWideChar(CP_UTF8, 0, plugin.name.c_str(), -1, nullptr, 0);
        if (len > 0) {
            std::vector<wchar_t> wname(len);
            MultiByteToWideChar(CP_UTF8, 0, plugin.name.c_str(), -1, wname.data(), len);
            SendMessage(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(wname.data()));
        }
    }
}