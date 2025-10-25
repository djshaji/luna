#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <windows.h>
#include <vector>
#include <string>
#include <memory>
#include <lilv/lilv.h>

struct PluginParameter {
    std::string name;
    std::string symbol;
    float min_value;
    float max_value;
    float default_value;
    float current_value;
    bool is_input;
    bool is_control;
};

struct PluginInfo {
    std::string uri;
    std::string name;
    std::string bundle_path;
    std::vector<PluginParameter> parameters;
};

class PluginManager {
public:
    PluginManager();
    ~PluginManager();
    
    bool Initialize();
    void Shutdown();
    
    void ScanPlugins();
    std::vector<PluginInfo> GetPluginList() const;
    
private:
    bool initialized;
    LilvWorld* world;
    const LilvPlugins* plugins;
    std::vector<PluginInfo> pluginList;
};

#endif // PLUGIN_MANAGER_H