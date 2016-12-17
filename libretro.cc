// Copyright 2016 oneup

#include "libretro.h"

#include <iostream>
#include <memory>

#include "Machine.hpp"

static constexpr const char* LIBNAME = "oneup-gb-lr";
#define LOG_CALL do { std::cout << LIBNAME << " " << __func__ << std::endl; } while (0)

std::unique_ptr<gblr::Machine> g_machine;

RETRO_API unsigned retro_api_version(void) { return RETRO_API_VERSION; }

struct {
	retro_environment_t 		environment;
	retro_video_refresh_t 		video_refresh;
	retro_audio_sample_t 		audio_sample;
	retro_audio_sample_batch_t 	audio_sample_batch;
	retro_input_poll_t 			input_poll;
	retro_input_state_t			input_state;
} callbacks;

bool g_polled = true;

static gblr::Button do_button_poll() {
	callbacks.input_poll();

	gblr::u8 btn = 0;
	if (callbacks.input_state(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)) 		{ btn |= gblr::BTN_DOWN; }
	if (callbacks.input_state(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)) 		{ btn |= gblr::BTN_UP; }
	if (callbacks.input_state(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)) 		{ btn |= gblr::BTN_LEFT; }
	if (callbacks.input_state(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT)) 	{ btn |= gblr::BTN_RIGHT; }
	if (callbacks.input_state(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START)) 	{ btn |= gblr::BTN_START; }
	if (callbacks.input_state(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT)) 	{ btn |= gblr::BTN_SELECT; }
	if (callbacks.input_state(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B)) 		{ btn |= gblr::BTN_B; }
	if (callbacks.input_state(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)) 		{ btn |= gblr::BTN_A; }

	g_polled = true;

	std::cerr << "do_button_poll: return $" << std::hex << unsigned(btn) << std::dec << std::endl;
	return gblr::Button(btn);
}

unsigned long long g_ticks = 0,
				   g_audio_frames = 0;
std::array<int16_t, 32 * 1024 * 1024> g_samples;

RETRO_API void retro_init(void) {
	g_machine = std::make_unique<gblr::Machine>();
	g_machine->btn_poller = do_button_poll;

	g_samples.fill(0);
}
RETRO_API void retro_deinit(void) { g_machine.release(); }

RETRO_API void retro_run(void) {
	g_machine->ResetFrame();

	while (!g_machine->FrameReady()) {
		g_machine->Tick();
		++g_ticks;
	}

	unsigned long long audio_frames = g_ticks / 48000;
	while (g_audio_frames < audio_frames) {
		unsigned n_frames = audio_frames - g_audio_frames;
		if (n_frames > g_samples.size() / 2) { n_frames = g_samples.size() / 2; }

		callbacks.audio_sample_batch(g_samples.data(), n_frames);

		g_audio_frames += n_frames;
	}

	callbacks.video_refresh(g_machine->GetFrame(), 160, 144, 160*4);
}

RETRO_API void retro_set_environment(retro_environment_t environment) { callbacks.environment = environment; }
RETRO_API void retro_set_video_refresh(retro_video_refresh_t video_refresh) { callbacks.video_refresh = video_refresh; }
RETRO_API void retro_set_audio_sample(retro_audio_sample_t audio_sample) { callbacks.audio_sample = audio_sample; }
RETRO_API void retro_set_audio_sample_batch(retro_audio_sample_batch_t audio_sample_batch) { callbacks.audio_sample_batch = audio_sample_batch; }
RETRO_API void retro_set_input_poll(retro_input_poll_t input_poll) { callbacks.input_poll = input_poll; }
RETRO_API void retro_set_input_state(retro_input_state_t input_state) { callbacks.input_state = input_state; }
RETRO_API void retro_set_controller_port_device(unsigned, unsigned) { LOG_CALL; }

RETRO_API bool retro_load_game(const struct retro_game_info *game) {
	return g_machine->LoadGame(game);
}
RETRO_API void retro_unload_game(void) { g_machine->UnloadGame(); }


static constexpr const retro_system_info system_info = {
	"Oneup DMG core",
	"Beta",
	"gb",
	false,
	false
};
RETRO_API void retro_get_system_info(struct retro_system_info *info) {
	*info = system_info;
}


static constexpr const retro_system_av_info system_av_info = {
	{160, 144, 160, 144, 0.0},
	{60.0, 48000.0}
};
RETRO_API void retro_get_system_av_info(struct retro_system_av_info *info) {
	enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
	callbacks.environment(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);

	*info = system_av_info;
}

RETRO_API size_t retro_serialize_size(void) {
	LOG_CALL;
	return 0;
}

RETRO_API bool retro_serialize(void*, size_t) {
	LOG_CALL;
	return true;
}

RETRO_API bool retro_unserialize(const void*, size_t) {
	LOG_CALL;
	return true;
}

RETRO_API void retro_reset(void) { LOG_CALL; }

RETRO_API void retro_cheat_reset(void) { LOG_CALL; }
RETRO_API void retro_cheat_set(unsigned, bool, const char*) { LOG_CALL; }

RETRO_API bool retro_load_game_special(unsigned, const struct retro_game_info*, size_t) {
	LOG_CALL;
	return false;
}

RETRO_API unsigned retro_get_region(void) {
	LOG_CALL;
	return 0;
}

RETRO_API void *retro_get_memory_data(unsigned id) {
	switch (id) {
		case RETRO_MEMORY_SYSTEM_RAM:
			// TODO: hram
			return g_machine->wram.data();
		case RETRO_MEMORY_SAVE_RAM:		// TODO
		case RETRO_MEMORY_RTC:			// TODO
		case RETRO_MEMORY_VIDEO_RAM:	// TODO
		default:
			return nullptr;
	}
}

RETRO_API size_t retro_get_memory_size(unsigned id) {
	switch (id) {
		case RETRO_MEMORY_SYSTEM_RAM:
			// TODO: hram
			return g_machine->wram.size();
		case RETRO_MEMORY_SAVE_RAM:		// TODO
		case RETRO_MEMORY_RTC:			// TODO
		case RETRO_MEMORY_VIDEO_RAM:	// TODO
		default:
			return 0;
	}
}
