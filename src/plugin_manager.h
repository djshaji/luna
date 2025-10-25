#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <vector>
#include <string>
#include <memory>
#include <lilv/lilv.h>

struct PluginInfo {
    std::string uri;
    std::string name;
    std::string bundle_path;
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