#include "audio_engine.h"
#include <iostream>
#include <cstring>

AudioEngine::AudioEngine() : hWaveOut(nullptr), initialized(false), running(false) {
}

AudioEngine::~AudioEngine() {
    Shutdown();
}

bool AudioEngine::Initialize() {
    if (initialized) {
        return true;
    }
    
    try {
        // Set up wave format
        WAVEFORMATEX wfx = {};
        wfx.wFormatTag = WAVE_FORMAT_PCM;
        wfx.nChannels = CHANNELS;
        wfx.nSamplesPerSec = SAMPLE_RATE;
        wfx.wBitsPerSample = BITS_PER_SAMPLE;
        wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
        wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
        
        // Open wave out device
        MMRESULT result = waveOutOpen(
            &hWaveOut,
            WAVE_MAPPER,
            &wfx,
            reinterpret_cast<DWORD_PTR>(WaveOutProc),
            reinterpret_cast<DWORD_PTR>(this),
            CALLBACK_FUNCTION
        );
        
        if (result != MMSYSERR_NOERROR) {
            return false;
        }
        
        // Prepare audio buffers
        PrepareBuffers();
        
        initialized = true;
        return true;
        
    } catch (...) {
        return false;
    }
}

void AudioEngine::Shutdown() {
    if (!initialized) {
        return;
    }
    
    Stop();
    CleanupBuffers();
    
    if (hWaveOut) {
        waveOutClose(hWaveOut);
        hWaveOut = nullptr;
    }
    
    initialized = false;
}

bool AudioEngine::Start() {
    if (!initialized || running) {
        return false;
    }
    
    // Queue initial buffers
    for (size_t i = 0; i < waveHeaders.size(); ++i) {
        ProcessAudio(waveHeaders[i].get());
        waveOutWrite(hWaveOut, waveHeaders[i].get(), sizeof(WAVEHDR));
    }
    
    running = true;
    return true;
}

void AudioEngine::Stop() {
    if (!running) {
        return;
    }
    
    running = false;
    
    if (hWaveOut) {
        waveOutReset(hWaveOut);
    }
}

void CALLBACK AudioEngine::WaveOutProc(HWAVEOUT, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR) {
    AudioEngine* engine = reinterpret_cast<AudioEngine*>(dwInstance);
    
    switch (uMsg) {
        case WOM_DONE: {
            WAVEHDR* header = reinterpret_cast<WAVEHDR*>(dwParam1);
            if (engine->running) {
                engine->ProcessAudio(header);
                waveOutWrite(engine->hWaveOut, header, sizeof(WAVEHDR));
            }
            break;
        }
    }
}

void AudioEngine::ProcessAudio(WAVEHDR* header) {
    if (!header || !header->lpData) {
        return;
    }
    
    // Generate silence for now (TODO: implement LV2 plugin processing)
    short* buffer = reinterpret_cast<short*>(header->lpData);
    std::memset(buffer, 0, header->dwBufferLength);
}

void AudioEngine::PrepareBuffers() {
    const size_t bufferSizeBytes = BUFFER_SIZE * CHANNELS * sizeof(short);
    
    for (int i = 0; i < NUM_BUFFERS; ++i) {
        // Allocate audio buffer
        auto audioBuffer = std::make_unique<short[]>(BUFFER_SIZE * CHANNELS);
        
        // Create wave header
        auto waveHeader = std::make_unique<WAVEHDR>();
        std::memset(waveHeader.get(), 0, sizeof(WAVEHDR));
        
        waveHeader->lpData = reinterpret_cast<LPSTR>(audioBuffer.get());
        waveHeader->dwBufferLength = static_cast<DWORD>(bufferSizeBytes);
        waveHeader->dwFlags = 0;
        
        // Prepare the header
        waveOutPrepareHeader(hWaveOut, waveHeader.get(), sizeof(WAVEHDR));
        
        // Store buffers
        audioBuffers.push_back(std::move(audioBuffer));
        waveHeaders.push_back(std::move(waveHeader));
    }
}

void AudioEngine::CleanupBuffers() {
    for (auto& header : waveHeaders) {
        if (header && hWaveOut) {
            waveOutUnprepareHeader(hWaveOut, header.get(), sizeof(WAVEHDR));
        }
    }
    
    waveHeaders.clear();
    audioBuffers.clear();
}