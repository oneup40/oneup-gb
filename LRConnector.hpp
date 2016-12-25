#pragma once

// Copyright 2016 oneup

#include "Machine.hpp"

#include "libretro.h"

namespace gblr {

struct MemDescriptor {
    void *data;
    size_t size;

	MemDescriptor(void *data = nullptr, size_t size = 0) : data(data), size(size) {}
};

class LRConnector {
    Machine m_;

    retro_environment_t         environment_;
    retro_video_refresh_t       video_refresh_;
    retro_audio_sample_t        audio_sample_;
    retro_audio_sample_batch_t  audio_sample_batch_;
    retro_input_poll_t          input_poll_;
    retro_input_state_t         input_state_;

    static constexpr const char *name_ = "oneup-dmg",
                                *version_ = GIT_BRANCH,
                                *extensions_ = "gb";

    static constexpr const unsigned width_ = 160,
                                    height_ = 144;

    static constexpr const double fps_ = 60.0,
                                  sample_rate_ = 0.0;
public:
    LRConnector();
    LRConnector(const LRConnector&) = delete;
    LRConnector(LRConnector&&) = delete;
    LRConnector& operator=(const LRConnector&) = delete;
    LRConnector& operator=(LRConnector&&) = delete;

    // frontend interface
    unsigned ApiVersion();    // probably best to implement this outside of the header

    void Init() {}
    void DeInit() {}

    void Run();

    void SetEnvironment(retro_environment_t callback)               { environment_ = callback; }
    void SetVideoRefresh(retro_video_refresh_t callback)            { video_refresh_ = callback; }
    void SetAudioSample(retro_audio_sample_t callback)              { audio_sample_ = callback; }
    void SetAudioSampleBatch(retro_audio_sample_batch_t callback)   { audio_sample_batch_ = callback; }
    void SetInputPoll(retro_input_poll_t callback)                  { input_poll_ = callback; }
    void SetInputState(retro_input_state_t callback)                { input_state_ = callback; }

    bool LoadGame(const struct retro_game_info *game);
    void UnloadGame();

    void GetSystemInfo(retro_system_info *info);
    void GetSystemAVInfo(retro_system_av_info *info);

    MemDescriptor GetSystemRAM();
    MemDescriptor GetSaveRAM();
    MemDescriptor GetRTC();
    MemDescriptor GetVRAM();
    MemDescriptor GetMemory(unsigned id);

    void* GetMemoryData(unsigned id);
    size_t GetMemorySize(unsigned id);

    size_t GetSerializeSize();
    bool Serialize(void *data, size_t len);
    bool Unserialize(const void *data, size_t len);

    // machine interface
    bool SetPixelFormat(enum retro_pixel_format fmt);
    bool ShowMessage(const char *msg, unsigned frames);

    Button PollInput();
};

}    // namespace gblr
