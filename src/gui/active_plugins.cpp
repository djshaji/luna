#include "active_plugins.h"
#include <commctrl.h>
#include <algorithm>
#include <iostream>

ActivePlugins::ActivePlugins(HWND parent, HINSTANCE hInstance) 
    : hParent(parent), hScrollArea(nullptr), hTitleLabel(nullptr), 
      hClearAllButton(nullptr), hInstance(hInstance), 
      containerX(0), containerY(0), containerWidth(0), containerHeight(0),
      originalScrollProc(nullptr) {
}

ActivePlugins::~ActivePlugins() {
    if (hScrollArea && originalScrollProc) {
        SetWindowLongPtr(hScrollArea, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalScrollProc));
    }
}

bool ActivePlugins::Create(int x, int y, int width, int height) {
    containerX = x;
    containerY = y;
    containerWidth = width;
    containerHeight = height;
    
    // Create title label
    hTitleLabel = CreateWindowW(
        L"STATIC",
        L"🎛️ Active Plugins",
        WS_VISIBLE | WS_CHILD | SS_LEFT | SS_CENTERIMAGE,
        x, y - 25, width - 120, 20,
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
    
    // Create "Clear All" button
    hClearAllButton = CreateWindowW(
        L"BUTTON",
        L"🗑️ Clear All",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        x + width - 100, y - 27, 100, 24,
        hParent,
        reinterpret_cast<HMENU>(3000), // Use the constant value directly
        hInstance,
        nullptr
    );    // Set modern font for button
    HFONT hButtonFont = CreateFontW(
        11, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
        L"Segoe UI"
    );
    SendMessage(hClearAllButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
    
    // Create scrollable area for plugin cards using proper container
    hScrollArea = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"STATIC",
        nullptr,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        x, y, width, height,
        hParent,
        nullptr,
        hInstance,
        nullptr
    );
    
    if (!hScrollArea) {
        return false;
    }
    
    // Subclass the scroll area to handle WM_COMMAND messages from buttons
    originalScrollProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hScrollArea, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ScrollProc)));
    SetWindowLongPtr(hScrollArea, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    
    RefreshDisplay();
    return true;
}

void ActivePlugins::Resize(int x, int y, int width, int height) {
    containerX = x;
    containerY = y;
    containerWidth = width;
    containerHeight = height;
    
    if (hTitleLabel) {
        SetWindowPos(hTitleLabel, nullptr, x, y - 25, width - 120, 20, SWP_NOZORDER);
    }
    
    if (hClearAllButton) {
        SetWindowPos(hClearAllButton, nullptr, x + width - 100, y - 27, 100, 24, SWP_NOZORDER);
    }
    
    if (hScrollArea) {
        SetWindowPos(hScrollArea, nullptr, x, y, width, height, SWP_NOZORDER);
        LayoutCards();
    }
}

void ActivePlugins::AddPlugin(const PluginInfo& plugin) {
    PluginCard card;
    card.plugin = plugin;
    card.bypassed = false;
    card.hCard = nullptr;
    
    activePlugins.push_back(card);
    
    // Create the visual card for the new plugin
    CreatePluginCard(activePlugins.back(), static_cast<int>(activePlugins.size() - 1));
    
    LayoutCards();
    
    std::cout << "Added plugin card: " << plugin.name << std::endl;
}

void ActivePlugins::RemovePlugin(int index) {
    if (index >= 0 && index < static_cast<int>(activePlugins.size())) {
        std::cout << "Removed plugin card: " << activePlugins[index].plugin.name << std::endl;
        
        // Destroy card controls
        PluginCard& card = activePlugins[index];
        if (card.hCard) DestroyWindow(card.hCard);
        if (card.hTitleLabel) DestroyWindow(card.hTitleLabel);
        if (card.hBypassButton) DestroyWindow(card.hBypassButton);
        if (card.hRemoveButton) DestroyWindow(card.hRemoveButton);
        
        for (HWND slider : card.parameterSliders) {
            if (slider) DestroyWindow(slider);
        }
        for (HWND label : card.parameterLabels) {
            if (label) DestroyWindow(label);
        }
        for (HWND value : card.parameterValues) {
            if (value) DestroyWindow(value);
        }
        
        activePlugins.erase(activePlugins.begin() + index);
        LayoutCards();
    }
}

void ActivePlugins::BypassPlugin(int index, bool bypass) {
    if (index >= 0 && index < static_cast<int>(activePlugins.size())) {
        activePlugins[index].bypassed = bypass;
        
        // Update bypass button text and state
        if (activePlugins[index].hBypassButton) {
            const wchar_t* text = bypass ? L"🔇 Bypassed" : L"🔊 Active";
            SetWindowText(activePlugins[index].hBypassButton, text);
            // Set the check state of the toggle button
            SendMessage(activePlugins[index].hBypassButton, BM_SETCHECK, bypass ? BST_CHECKED : BST_UNCHECKED, 0);
        }
        
        std::cout << (bypass ? "Bypassed" : "Activated") << " plugin: " 
                  << activePlugins[index].plugin.name << std::endl;
    }
}

void ActivePlugins::ClearAll() {
    // Destroy all card controls
    for (auto& card : activePlugins) {
        if (card.hCard) DestroyWindow(card.hCard);
        if (card.hTitleLabel) DestroyWindow(card.hTitleLabel);
        if (card.hBypassButton) DestroyWindow(card.hBypassButton);
        if (card.hRemoveButton) DestroyWindow(card.hRemoveButton);
        
        for (HWND slider : card.parameterSliders) {
            if (slider) DestroyWindow(slider);
        }
        for (HWND label : card.parameterLabels) {
            if (label) DestroyWindow(label);
        }
        for (HWND value : card.parameterValues) {
            if (value) DestroyWindow(value);
        }
    }
    
    activePlugins.clear();
    RefreshDisplay();
    std::cout << "Cleared all plugin cards" << std::endl;
}

void ActivePlugins::UpdateParameterValue(int pluginIndex, int paramIndex, float value) {
    if (pluginIndex >= 0 && pluginIndex < static_cast<int>(activePlugins.size())) {
        PluginCard& card = activePlugins[pluginIndex];
        if (paramIndex >= 0 && paramIndex < static_cast<int>(card.plugin.parameters.size())) {
            card.plugin.parameters[paramIndex].current_value = value;
            
            // Update value display
            if (paramIndex < static_cast<int>(card.parameterValues.size()) && card.parameterValues[paramIndex]) {
                wchar_t valueText[32];
                swprintf(valueText, 32, L"%.2f", value);
                SetWindowText(card.parameterValues[paramIndex], valueText);
            }
        }
    }
}

float ActivePlugins::GetParameterValue(int pluginIndex, int paramIndex) const {
    if (pluginIndex >= 0 && pluginIndex < static_cast<int>(activePlugins.size())) {
        const PluginCard& card = activePlugins[pluginIndex];
        if (paramIndex >= 0 && paramIndex < static_cast<int>(card.plugin.parameters.size())) {
            return card.plugin.parameters[paramIndex].current_value;
        }
    }
    return 0.0f;
}

void ActivePlugins::RefreshDisplay() {
    if (activePlugins.empty()) {
        // Show empty state message in scroll area
        InvalidateRect(hScrollArea, nullptr, TRUE);
        UpdateWindow(hScrollArea);
    } else {
        LayoutCards();
    }
    
    // Ensure the parent window redraws this area
    if (hParent) {
        RECT rect = {containerX, containerY, containerX + containerWidth, containerY + containerHeight};
        InvalidateRect(hParent, &rect, FALSE);
    }
}

static LRESULT CALLBACK CardProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_COMMAND) {
        // Forward command messages to the scroll area, which will forward to main window
        HWND hParent = GetParent(hWnd);
        if (hParent) {
            std::cout << "CardProc: Forwarding WM_COMMAND ID " << LOWORD(wParam) << " to scroll area" << std::endl;
            return SendMessage(hParent, WM_COMMAND, wParam, lParam);
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void ActivePlugins::CreatePluginCard(PluginCard& card, int cardIndex) {
    if (!hScrollArea) return;
    
    int cardY = cardIndex * (CARD_HEIGHT + CARD_SPACING) + CARD_MARGIN;
    int cardWidth = containerWidth - (2 * CARD_MARGIN) - 20; // Account for scrollbar
    
    // Create card background with proper styles
    card.hCard = CreateWindowExW(
        0,
        L"STATIC",
        nullptr,
        WS_CHILD | WS_VISIBLE | SS_NOTIFY | WS_CLIPCHILDREN,
        CARD_MARGIN, cardY, cardWidth, CARD_HEIGHT,
        hScrollArea,
        reinterpret_cast<HMENU>(3100 + cardIndex), // Unique ID for card
        hInstance,
        nullptr
    );
    
    // Set white background for the card
    SetClassLongPtr(card.hCard, GCLP_HBRBACKGROUND, 
                   reinterpret_cast<LONG_PTR>(CreateSolidBrush(RGB(255, 255, 255))));
    
    // Subclass the card to handle WM_COMMAND messages from child buttons
    SetWindowLongPtr(card.hCard, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(CardProc));
    
    // Create plugin title
    std::wstring title = L"🎚️ ";
    int len = MultiByteToWideChar(CP_UTF8, 0, card.plugin.name.c_str(), -1, nullptr, 0);
    if (len > 0) {
        std::vector<wchar_t> wname(len);
        MultiByteToWideChar(CP_UTF8, 0, card.plugin.name.c_str(), -1, wname.data(), len);
        title += wname.data();
    }
    
    card.hTitleLabel = CreateWindowW(
        L"STATIC",
        title.c_str(),
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        10, 10, cardWidth - 20, 25,
        card.hCard,
        nullptr,
        hInstance,
        nullptr
    );
    
    // Set title font
    HFONT hTitleFont = CreateFontW(
        13, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
        L"Segoe UI"
    );
    SendMessage(card.hTitleLabel, WM_SETFONT, (WPARAM)hTitleFont, TRUE);
    
    // Create bypass button as toggle
    card.hBypassButton = CreateWindowW(
        L"BUTTON",
        L"🔊 Active",
        WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        cardWidth - 180, 8, 80, 24,
        card.hCard,
        reinterpret_cast<HMENU>(4000 + cardIndex * 100 + 1), // Unique ID
        hInstance,
        nullptr
    );
    
    // Create remove button
    card.hRemoveButton = CreateWindowW(
        L"BUTTON",
        L"🗑️ Remove",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        cardWidth - 90, 8, 80, 24,
        card.hCard,
        reinterpret_cast<HMENU>(4000 + cardIndex * 100 + 2), // Unique ID
        hInstance,
        nullptr
    );
    
    // Set button fonts
    HFONT hButtonFont = CreateFontW(
        10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
        L"Segoe UI"
    );
    SendMessage(card.hBypassButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
    SendMessage(card.hRemoveButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
    
    // Create parameter controls based on plugin parameters
    card.parameterSliders.clear();
    card.parameterLabels.clear();
    card.parameterValues.clear();
    
    // Use actual plugin parameters (up to 4 for UI space)
    int paramCount = std::min(4, static_cast<int>(card.plugin.parameters.size()));
    for (int i = 0; i < paramCount; ++i) {
        const PluginParameter& param = card.plugin.parameters[i];
        int paramY = 45 + i * 35;
        
        // Parameter label with real parameter name
        std::wstring paramLabel = L"";
        int len = MultiByteToWideChar(CP_UTF8, 0, param.name.c_str(), -1, nullptr, 0);
        if (len > 0) {
            std::vector<wchar_t> wname(len);
            MultiByteToWideChar(CP_UTF8, 0, param.name.c_str(), -1, wname.data(), len);
            paramLabel = wname.data();
        }
        paramLabel += L":";
        
        HWND hLabel = CreateWindowW(
            L"STATIC",
            paramLabel.c_str(),
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            10, paramY, 60, 20,
            card.hCard,
            nullptr,
            hInstance,
            nullptr
        );
        card.parameterLabels.push_back(hLabel);
        
        // Parameter slider with real parameter range
        HWND hSlider = CreateWindowW(
            TRACKBAR_CLASS,
            nullptr,
            WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS,
            75, paramY, cardWidth - 180, 20,
            card.hCard,
            reinterpret_cast<HMENU>(5000 + cardIndex * 100 + i), // Unique ID
            hInstance,
            nullptr
        );
        
        // Set slider range based on parameter min/max (scaled to 0-100 for UI)
        SendMessage(hSlider, TBM_SETRANGE, TRUE, MAKELPARAM(0, 100));
        
        // Calculate initial position based on current value
        float normalized = (param.current_value - param.min_value) / (param.max_value - param.min_value);
        int pos = static_cast<int>(normalized * 100.0f);
        SendMessage(hSlider, TBM_SETPOS, TRUE, pos);
        
        card.parameterSliders.push_back(hSlider);
        
        // Parameter value display with actual current value
        wchar_t valueText[32];
        swprintf(valueText, 32, L"%.2f", param.current_value);
        HWND hValue = CreateWindowW(
            L"STATIC",
            valueText,
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            cardWidth - 95, paramY, 40, 20,
            card.hCard,
            nullptr,
            hInstance,
            nullptr
        );
        card.parameterValues.push_back(hValue);
        
        // Set fonts for parameter controls
        HFONT hParamFont = CreateFontW(
            9, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
            L"Segoe UI"
        );
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hParamFont, TRUE);
        SendMessage(hValue, WM_SETFONT, (WPARAM)hParamFont, TRUE);
    }
}

void ActivePlugins::LayoutCards() {
    if (!hScrollArea || activePlugins.empty()) {
        return;
    }
    
    // Calculate total content height
    int totalHeight = static_cast<int>(activePlugins.size()) * (CARD_HEIGHT + CARD_SPACING) + CARD_MARGIN;
    
    // Set scroll range
    SCROLLINFO si = {};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = totalHeight;
    si.nPage = containerHeight;
    SetScrollInfo(hScrollArea, SB_VERT, &si, TRUE);
    
    // Position all cards and ensure proper redraw
    for (size_t i = 0; i < activePlugins.size(); ++i) {
        PluginCard& card = activePlugins[i];
        if (card.hCard) {
            int cardY = static_cast<int>(i) * (CARD_HEIGHT + CARD_SPACING) + CARD_MARGIN;
            int cardWidth = containerWidth - (2 * CARD_MARGIN) - 20;
            
            SetWindowPos(card.hCard, nullptr, CARD_MARGIN, cardY, cardWidth, CARD_HEIGHT, 
                        SWP_NOZORDER | SWP_NOACTIVATE);
            
            // Force redraw of the card to prevent artifacts
            InvalidateRect(card.hCard, nullptr, TRUE);
            UpdateWindow(card.hCard);
        }
    }
    
    // Force redraw of the scroll area
    InvalidateRect(hScrollArea, nullptr, TRUE);
}

LRESULT CALLBACK ActivePlugins::ScrollProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    ActivePlugins* pThis = reinterpret_cast<ActivePlugins*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    
    switch (uMsg) {
        case WM_COMMAND: {
            // Forward button commands to the main window
            if (pThis) {
                int controlId = LOWORD(wParam);
                int notification = HIWORD(wParam);
                std::cout << "ScrollProc: Received WM_COMMAND ID " << controlId << ", notification " << notification << std::endl;
                
                // Forward to parent window (main window)
                LRESULT result = SendMessage(pThis->hParent, WM_COMMAND, wParam, lParam);
                std::cout << "ScrollProc: Forwarded to parent, result: " << result << std::endl;
                return result;
            }
            break;
        }
        case WM_VSCROLL: {
            SCROLLINFO si = {};
            si.cbSize = sizeof(SCROLLINFO);
            si.fMask = SIF_ALL;
            GetScrollInfo(hWnd, SB_VERT, &si);
            
            int yPos = si.nPos;
            
            switch (LOWORD(wParam)) {
                case SB_LINEUP:
                    yPos -= 20;
                    break;
                case SB_LINEDOWN:
                    yPos += 20;
                    break;
                case SB_PAGEUP:
                    yPos -= si.nPage;
                    break;
                case SB_PAGEDOWN:
                    yPos += si.nPage;
                    break;
                case SB_THUMBTRACK:
                    yPos = HIWORD(wParam);
                    break;
            }
            
            yPos = std::max(0, std::min(yPos, si.nMax - static_cast<int>(si.nPage)));
            
            if (yPos != si.nPos) {
                SetScrollPos(hWnd, SB_VERT, yPos, TRUE);
                ScrollWindow(hWnd, 0, si.nPos - yPos, nullptr, nullptr);
            }
            
            return 0;
        }
        
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORSTATIC: {
            // Ensure proper colors for child controls
            HDC hdc = (HDC)wParam;
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)GetStockObject(WHITE_BRUSH);
        }
        
        case WM_PAINT: {
            if (pThis && pThis->activePlugins.empty()) {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                
                // Draw empty state message
                RECT rect;
                GetClientRect(hWnd, &rect);
                
                // Fill background
                FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
                
                SetTextColor(hdc, RGB(128, 128, 128));
                SetBkMode(hdc, TRANSPARENT);
                
                DrawText(hdc, L"🎛️ No plugins in chain\n\nDouble-click plugins from the list\nto add them as cards with controls", 
                        -1, &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
                
                EndPaint(hWnd, &ps);
                return 0;
            }
            break;
        }
    }
    
    return CallWindowProc(pThis ? pThis->originalScrollProc : DefWindowProc, hWnd, uMsg, wParam, lParam);
}