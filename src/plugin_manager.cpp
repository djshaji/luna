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

        char cwd[1024];
        GetCurrentDirectoryA(sizeof(cwd), cwd);
        std::string lv2Path = std::string(cwd) + "\\lv2";
        LilvNode* lv2_path = lilv_new_file_uri(world, NULL, lv2Path.c_str());
        lilv_world_set_option(world, LILV_OPTION_LV2_PATH, lv2_path);
        lilv_node_free(lv2_path);
        printf("LV2 Path set to: %s\n", lv2Path.c_str());

        // Set the LV2 path option
        // Set environment variable LV2_PATH
        _putenv_s("LV2_PATH", lv2Path.c_str());

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
        printf("Found plugin URI: %s\n", uri_str ? uri_str : "Unknown");
        
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
        
        // Add some default parameters for demo (simplified implementation)
        for (int i = 0; i < 4; ++i) {
            PluginParameter param;
            param.name = "Param " + std::to_string(i + 1);
            param.symbol = "param" + std::to_string(i + 1);
            param.min_value = 0.0f;
            param.max_value = 100.0f;
            param.default_value = 50.0f;
            param.current_value = 50.0f;
            param.is_input = true;
            param.is_control = true;
            info.parameters.push_back(param);
        }
        
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