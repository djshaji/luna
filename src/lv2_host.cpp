#include "lv2_host.h"
#include "plugin_manager.h"
#include "audio_engine.h"

LV2Host::LV2Host() : initialized(false), audioRunning(false) {
}

LV2Host::~LV2Host() {
    Shutdown();
}

bool LV2Host::Initialize() {
    if (initialized) {
        return true;
    }
    
    try {
        // Initialize plugin manager
        pluginManager = std::make_unique<PluginManager>();
        if (!pluginManager->Initialize()) {
            return false;
        }
        
        // Initialize audio engine
        audioEngine = std::make_unique<AudioEngine>();
        if (!audioEngine->Initialize()) {
            return false;
        }
        
        initialized = true;
        return true;
        
    } catch (...) {
        return false;
    }
}

void LV2Host::Shutdown() {
    if (!initialized) {
        return;
    }
    
    StopAudio();
    
    if (audioEngine) {
        audioEngine->Shutdown();
        audioEngine.reset();
    }
    
    if (pluginManager) {
        pluginManager->Shutdown();
        pluginManager.reset();
    }
    
    initialized = false;
}

bool LV2Host::StartAudio() {
    if (!initialized || !audioEngine) {
        return false;
    }
    
    if (audioRunning) {
        return true;
    }
    
    if (audioEngine->Start()) {
        audioRunning = true;
        return true;
    }
    
    return false;
}

void LV2Host::StopAudio() {
    if (!audioRunning || !audioEngine) {
        return;
    }
    
    audioEngine->Stop();
    audioRunning = false;
}