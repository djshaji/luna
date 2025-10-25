#ifndef ACTIVE_PLUGINS_H
#define ACTIVE_PLUGINS_H

#include <windows.h>
#include <vector>
#include <memory>
#include "../plugin_manager.h"

struct PluginCard {
    PluginInfo plugin;
    HWND hCard;
    HWND hTitleLabel;
    HWND hBypassButton;
    HWND hRemoveButton;
    std::vector<HWND> parameterSliders;
    std::vector<HWND> parameterLabels;
    std::vector<HWND> parameterValues;
    bool bypassed;
};

class ActivePlugins {
public:
    ActivePlugins(HWND parent, HINSTANCE hInstance);
    ~ActivePlugins();
    
    bool Create(int x, int y, int width, int height);
    void Resize(int x, int y, int width, int height);
    
    void AddPlugin(const PluginInfo& plugin);
    void RemovePlugin(int index);
    void BypassPlugin(int index, bool bypass);
    void ClearAll();
    
    void UpdateParameterValue(int pluginIndex, int paramIndex, float value);
    float GetParameterValue(int pluginIndex, int paramIndex) const;
    
    size_t GetPluginCount() const { return activePlugins.size(); }
    const std::vector<PluginCard>& GetPlugins() const { return activePlugins; }
    
private:
    void RefreshDisplay();
    void CreatePluginCard(PluginCard& card, int cardIndex);
    void LayoutCards();
    
    static LRESULT CALLBACK ScrollProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    HWND hParent;
    HWND hScrollArea;
    HWND hTitleLabel;
    HWND hClearAllButton;
    HINSTANCE hInstance;
    
    std::vector<PluginCard> activePlugins;
    int containerX, containerY, containerWidth, containerHeight;
    
    WNDPROC originalScrollProc;
    static const int CARD_HEIGHT = 200;
    static const int CARD_SPACING = 10;
    static const int CARD_MARGIN = 10;
};

#endif // ACTIVE_PLUGINS_H