#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <windows.h>
#include <mmeapi.h>
#include <vector>
#include <memory>

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine();
    
    bool Initialize();
    void Shutdown();
    
    bool Start();
    void Stop();
    
    bool IsRunning() const { return running; }
    
private:
    static void CALLBACK WaveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    void ProcessAudio(WAVEHDR* header);
    void PrepareBuffers();
    void CleanupBuffers();
    
    HWAVEOUT hWaveOut;
    bool initialized;
    bool running;
    
    std::vector<std::unique_ptr<WAVEHDR>> waveHeaders;
    std::vector<std::unique_ptr<short[]>> audioBuffers;
    
    static const int SAMPLE_RATE = 44100;
    static const int CHANNELS = 2;
    static const int BITS_PER_SAMPLE = 16;
    static const int BUFFER_SIZE = 1024;
    static const int NUM_BUFFERS = 4;
};

#endif // AUDIO_ENGINE_H