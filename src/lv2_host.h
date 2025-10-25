#ifndef LV2_HOST_H
#define LV2_HOST_H

#include <memory>
#include <vector>
#include <string>

class PluginManager;
class AudioEngine;

class LV2Host {
public:
    LV2Host();
    ~LV2Host();
    
    bool Initialize();
    void Shutdown();
    
    bool StartAudio();
    void StopAudio();
    
    bool IsAudioRunning() const { return audioRunning; }
    
    PluginManager* GetPluginManager() const { return pluginManager.get(); }
    AudioEngine* GetAudioEngine() const { return audioEngine.get(); }
    
private:
    std::unique_ptr<PluginManager> pluginManager;
    std::unique_ptr<AudioEngine> audioEngine;
    bool initialized;
    bool audioRunning;
};

#endif // LV2_HOST_H