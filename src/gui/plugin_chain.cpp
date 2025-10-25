#include "plugin_chain.h"
#include <algorithm>
#include <iostream>

PluginChain::PluginChain(HWND parent, HINSTANCE hInstance) 
    : hParent(parent), hListBox(nullptr), hTitleLabel(nullptr), hInstance(hInstance), originalListBoxProc(nullptr) {
}

PluginChain::~PluginChain() {
    if (hListBox && originalListBoxProc) {
        SetWindowLongPtr(hListBox, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalListBoxProc));
    }
}

bool PluginChain::Create(int x, int y, int width, int height) {
    // Create title label
    hTitleLabel = CreateWindowW(
        L"STATIC",
        L"🔗 Active Plugin Chain",
        WS_VISIBLE | WS_CHILD | SS_LEFT | SS_CENTERIMAGE,
        x, y, width, 25,
        hParent,
        nullptr,
        hInstance,
        nullptr
    );
    
    // Set modern font for title
    HFONT hTitleFont = CreateFontW(
        14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
        L"Segoe UI"
    );
    SendMessage(hTitleLabel, WM_SETFONT, (WPARAM)hTitleFont, TRUE);
    
    // Create listbox for active plugins
    hListBox = CreateWindowW(
        L"LISTBOX",
        nullptr,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_NOTIFY | LBS_HASSTRINGS,
        x, y + 30, width, height - 30,
        hParent,
        reinterpret_cast<HMENU>(2001), // Unique ID for plugin chain
        hInstance,
        nullptr
    );
    
    if (!hListBox) {
        return false;
    }
    
    // Set modern font for listbox
    HFONT hListFont = CreateFontW(
        12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
        L"Segoe UI"
    );
    SendMessage(hListBox, WM_SETFONT, (WPARAM)hListFont, TRUE);
    
    // Subclass the listbox to handle custom messages
    originalListBoxProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hListBox, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ListBoxProc)));
    SetWindowLongPtr(hListBox, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    
    RefreshDisplay();
    return true;
}

void PluginChain::Resize(int x, int y, int width, int height) {
    if (hTitleLabel) {
        SetWindowPos(hTitleLabel, nullptr, x, y, width, 25, SWP_NOZORDER);
    }
    
    if (hListBox) {
        SetWindowPos(hListBox, nullptr, x, y + 30, width, height - 30, SWP_NOZORDER);
    }
}

void PluginChain::AddPlugin(const PluginInfo& plugin) {
    activePlugins.push_back(plugin);
    RefreshDisplay();
    std::cout << "Added plugin to chain: " << plugin.name << std::endl;
}

void PluginChain::RemovePlugin(int index) {
    if (index >= 0 && index < static_cast<int>(activePlugins.size())) {
        std::cout << "Removed plugin from chain: " << activePlugins[index].name << std::endl;
        activePlugins.erase(activePlugins.begin() + index);
        RefreshDisplay();
    }
}

void PluginChain::ClearChain() {
    activePlugins.clear();
    RefreshDisplay();
    std::cout << "Cleared plugin chain" << std::endl;
}

void PluginChain::RefreshDisplay() {
    if (!hListBox) {
        return;
    }
    
    // Clear existing items
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
    
    if (activePlugins.empty()) {
        SendMessage(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"⚡ Drop plugins here or double-click from list"));
        SendMessage(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"📋 Plugin chain is empty"));
        return;
    }
    
    // Add active plugins with chain indicators
    for (size_t i = 0; i < activePlugins.size(); ++i) {
        std::wstring displayText;
        
        // Add chain position indicator
        if (i == 0) {
            displayText = L"🎤 ";
        } else {
            displayText = L"🔗 ";
        }
        
        // Convert plugin name to wide string
        const std::string& name = activePlugins[i].name;
        int len = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, nullptr, 0);
        if (len > 0) {
            std::vector<wchar_t> wname(len);
            MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, wname.data(), len);
            displayText += wname.data();
        }
        
        // Add output indicator for last plugin
        if (i == activePlugins.size() - 1) {
            displayText += L" 🔊";
        }
        
        SendMessage(hListBox, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(displayText.c_str()));
    }
}

LRESULT CALLBACK PluginChain::ListBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PluginChain* pThis = reinterpret_cast<PluginChain*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    switch (uMsg) {
        case WM_LBUTTONDBLCLK: {
            if (pThis) {
                pThis->OnDoubleClick();
            }
            return 0;
        }
        
        case WM_RBUTTONUP: {
            if (pThis) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                pThis->OnRightClick(x, y);
            }
            return 0;
        }
        
        case WM_KEYDOWN: {
            if (wParam == VK_DELETE && pThis) {
                int sel = static_cast<int>(SendMessage(hWnd, LB_GETCURSEL, 0, 0));
                if (sel != LB_ERR && sel < static_cast<int>(pThis->activePlugins.size())) {
                    pThis->RemovePlugin(sel);
                }
                return 0;
            }
            break;
        }
    }
    
    return CallWindowProc(pThis ? pThis->originalListBoxProc : DefWindowProc, hWnd, uMsg, wParam, lParam);
}

void PluginChain::OnDoubleClick() {
    int sel = static_cast<int>(SendMessage(hListBox, LB_GETCURSEL, 0, 0));
    if (sel != LB_ERR && sel < static_cast<int>(activePlugins.size())) {
        // Double-click removes plugin from chain
        RemovePlugin(sel);
    }
}

void PluginChain::OnRightClick(int x, int y) {
    int sel = static_cast<int>(SendMessage(hListBox, LB_GETCURSEL, 0, 0));
    if (sel != LB_ERR && sel < static_cast<int>(activePlugins.size())) {
        // Create context menu
        HMENU hMenu = CreatePopupMenu();
        AppendMenuW(hMenu, MF_STRING, 3001, L"🗑️ Remove Plugin");
        AppendMenuW(hMenu, MF_STRING, 3002, L"⬆️ Move Up");
        AppendMenuW(hMenu, MF_STRING, 3003, L"⬇️ Move Down");
        AppendMenuW(hMenu, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(hMenu, MF_STRING, 3004, L"🗑️ Clear Chain");
        
        // Convert to screen coordinates
        POINT pt = {x, y};
        ClientToScreen(hListBox, &pt);
        
        // Show context menu (Note: This is simplified - in a real implementation,
        // you'd need to handle the menu messages in the parent window)
        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hParent, nullptr);
        DestroyMenu(hMenu);
    }
}