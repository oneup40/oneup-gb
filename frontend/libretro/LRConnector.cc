// Copyright 2016-2017 oneup

#include "frontend/libretro/LRConnector.hpp"

#include <chrono>
#include <fstream>
#include <sstream>

#include <cstring>

#include "core/Serializer.hpp"

namespace gb1 {
namespace lr {

LRConnector::LRConnector()
    : m_(*this),
      environment_(nullptr),
      video_refresh_(nullptr),
      audio_sample_(nullptr), audio_sample_batch_(nullptr),
      input_poll_(nullptr), input_state_(nullptr),
      frame_ready_(false)
{}

unsigned LRConnector::ApiVersion() { return RETRO_API_VERSION; }

void LRConnector::Run() {
    input_poll_();
    queued_samples_.clear();
    frame_ready_ = false;

    while (!frame_ready_) {
		// clock is 4194304 Hz
		// output is 60 Hz
		// apx. 69905 ticks per frame
		for (auto i=0; i < 10000; ++i) {
			m_.Tick();
		}
    }

    audio_sample_batch_(queued_samples_.data(), queued_samples_.size() / 2);
}

bool LRConnector::LoadGame(const struct retro_game_info *game) {
	environment_(RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf);

	const u8 *ptr = static_cast<const u8*>(game->data);
	std::vector<u8> data(ptr, ptr + game->size);
    return m_.LoadGame(std::move(data));
}

void LRConnector::UnloadGame() {
    m_.UnloadGame();
}

void LRConnector::GetSystemInfo(retro_system_info *info) {
    info->library_name = name_;
    info->library_version = version_;
    info->valid_extensions = extensions_;
    info->need_fullpath = false;
    info->block_extract = false;
}

void LRConnector::GetSystemAVInfo(retro_system_av_info *info) {
    info->geometry.base_width = width_;
    info->geometry.base_height = height_;
    info->geometry.max_width = width_;
    info->geometry.max_height = height_;
    info->geometry.aspect_ratio = 0.0;

    info->timing.fps = fps_;
    info->timing.sample_rate = sample_rate_;

    SetPixelFormat(RETRO_PIXEL_FORMAT_XRGB8888);
}

MemDescriptor LRConnector::GetSystemRAM() {
    return MemDescriptor{m_.wram.data(), m_.wram.size()};
}

MemDescriptor LRConnector::GetSaveRAM() {
    return MemDescriptor{m_.mapper.ram.data(), m_.mapper.ram.size()};
}

MemDescriptor LRConnector::GetRTC() {
    return MemDescriptor{nullptr, 0};
}

MemDescriptor LRConnector::GetVRAM() {
    return MemDescriptor{m_.lcd.vram.data(), m_.lcd.vram.size()};
}

MemDescriptor LRConnector::GetMemory(unsigned id) {
    switch (id) {
        case RETRO_MEMORY_SYSTEM_RAM:   return GetSystemRAM();
        case RETRO_MEMORY_SAVE_RAM:     return GetSaveRAM();
        case RETRO_MEMORY_RTC:          return GetRTC();
        case RETRO_MEMORY_VIDEO_RAM:    return GetVRAM();
        default:                        return MemDescriptor();
    }
}

void* LRConnector::GetMemoryData(unsigned id) {
    return GetMemory(id).data;
}
size_t LRConnector::GetMemorySize(unsigned id) {
    return GetMemory(id).size;
}

size_t LRConnector::GetSerializeSize() {
    std::ostringstream oss(std::ios_base::out | std::ios_base::binary);

    Serializer s(&oss);
    s << m_;

    return static_cast<bool>(s) ? oss.str().length() : 0;
}

bool LRConnector::Serialize(void *data, size_t len) {
    std::ostringstream oss;

    Serializer s(&oss);
    s << m_;

    auto str = oss.str();
    if (!s || str.length() != len) { return false; }

    char *buf = static_cast<char*>(data);
	memcpy(buf, str.data(), str.length());

    return true;
}

bool LRConnector::Unserialize(const void *data, size_t len) {
    const char *buf = static_cast<const char*>(data);
    std::string str(buf, buf+len);
    std::istringstream iss(str, std::ios_base::in | std::ios_base::binary);

    Deserializer d(&iss);
    d >> m_;

    return static_cast<bool>(d);
}

bool LRConnector::SetPixelFormat(enum retro_pixel_format fmt) {
    return environment_(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);
}

bool LRConnector::ShowMessage(const char *msg, unsigned frames) {
    struct retro_message rmsg{msg, frames};
    return environment_(RETRO_ENVIRONMENT_SET_MESSAGE, &rmsg);
}

Button LRConnector::InputJoypad() {
    u8 btn = 0;
    if (input_state_(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))   { btn |= BTN_DOWN; }
    if (input_state_(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))     { btn |= BTN_UP; }
    if (input_state_(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))   { btn |= BTN_LEFT; }
    if (input_state_(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))  { btn |= BTN_RIGHT; }
    if (input_state_(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START))  { btn |= BTN_START; }
    if (input_state_(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT)) { btn |= BTN_SELECT; }
    if (input_state_(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B))      { btn |= BTN_B; }
    if (input_state_(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A))      { btn |= BTN_A; }

    return Button(btn);
}

void LRConnector::OutputAudioFrame(int16_t left, int16_t right) {
    queued_samples_.emplace_back(left);
    queued_samples_.emplace_back(right);

    if (queued_samples_.size() >= 100) {
        audio_sample_batch_(queued_samples_.data(), queued_samples_.size() / 2);
		queued_samples_.clear();
    }
}

void LRConnector::OutputVideoFrame(std::array<std::array<u8, 40>, 144>&& frame) {
    std::array<std::array<u32, 160>, 144> lr_frame;

    size_t lr_row = 0, lr_col = 0;
    for (auto &row : frame) {
        lr_col = 0;

        for (auto cols : row) {
            for (unsigned i=0; i < 4; ++i) {
                u32 color = 0;
                switch (cols & 3) {
                    case 0: color = 0x00aae618; break;
                    case 1: color = 0x0080ad12; break;
                    case 2: color = 0x0054710c; break;
                    case 3: color = 0x00334507; break;

                    default: break;
                }
                cols >>= 2;

                lr_frame[lr_row][lr_col] = color;
                ++lr_col;
            }
        }

        ++lr_row;
    }

    video_refresh_(lr_frame.data(), width_, height_, width_ * sizeof(u32));
    frame_ready_ = true;
}

}   // namespace lr
}   // namespace gb1
