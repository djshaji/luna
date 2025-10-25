#include "plugin_manager.h"
#include <iostream>
#include <cstring>

PluginManager::PluginManager() : initialized(false), world(nullptr), plugins(nullptr) {
}

PluginManager::~PluginManager() {
    Shutdown();
}

bool PluginManager::Initialize() {
    if (initialized) {
        return true;
    }
    
    try {
        // Create LV2 world
        world = lilv_world_new();
        if (!world) {
            return false;
        }
        
        // Load all installed LV2 plugins
        lilv_world_load_all(world);
        
        // Get all plugins
        plugins = lilv_world_get_all_plugins(world);
        
        // Scan and cache plugin information
        ScanPlugins();
        
        initialized = true;
        return true;
        
    } catch (...) {
        return false;
    }
}

void PluginManager::Shutdown() {
    if (!initialized) {
        return;
    }
    
    pluginList.clear();
    
    if (world) {
        lilv_world_free(world);
        world = nullptr;
    }
    
    plugins = nullptr;
    initialized = false;
}

void PluginManager::ScanPlugins() {
    if (!world || !plugins) {
        return;
    }
    
    pluginList.clear();
    
    LILV_FOREACH(plugins, iter, plugins) {
        const LilvPlugin* plugin = lilv_plugins_get(plugins, iter);
        
        // Get plugin URI
        const LilvNode* uri_node = lilv_plugin_get_uri(plugin);
        const char* uri_str = lilv_node_as_uri(uri_node);
        
        // Get plugin name
        LilvNode* name_node = lilv_plugin_get_name(plugin);
        const char* name_str = name_node ? lilv_node_as_string(name_node) : "Unknown";
        
        // Get bundle path
        const LilvNode* bundle_node = lilv_plugin_get_bundle_uri(plugin);
        const char* bundle_str = lilv_node_as_uri(bundle_node);
        
        // Create plugin info
        PluginInfo info;
        info.uri = uri_str ? uri_str : "";
        info.name = name_str ? name_str : "Unknown Plugin";
        info.bundle_path = bundle_str ? bundle_str : "";
        
        pluginList.push_back(info);
        
        // Free the name node
        if (name_node) {
            lilv_node_free(name_node);
        }
    }
}

std::vector<PluginInfo> PluginManager::GetPluginList() const {
    return pluginList;
}