// Copyright 2016 oneup

#include "libretro.h"

#include <iostream>
#include <memory>

#include "Machine.hpp"

static constexpr const char* LIBNAME = "oneup-gb-lr";
#define LOG_CALL do { std::cout << LIBNAME << " " << __func__ << std::endl; } while (0)

std::unique_ptr<gblr::Machine> g_machine;

RETRO_API unsigned retro_api_version(void) { return RETRO_API_VERSION; }

RETRO_API void retro_init(void) { g_machine = std::make_unique<gblr::Machine>(); }
RETRO_API void retro_deinit(void) { g_machine.release(); }
RETRO_API void retro_run(void) { g_machine->Tick(); }

RETRO_API bool retro_load_game(const struct retro_game_info *game) { return g_machine->LoadGame(game); }
RETRO_API void retro_unload_game(void) { g_machine->UnloadGame(); }

struct {
	retro_environment_t 		environment;
	retro_video_refresh_t 		video_refresh;
	retro_audio_sample_t 		audio_sample;
	retro_audio_sample_batch_t 	audio_sample_batch;
	retro_input_poll_t 			input_poll;
	retro_input_state_t			input_state;
} callbacks;

RETRO_API void retro_set_environment(retro_environment_t environment) {
	LOG_CALL;
	callbacks.environment = environment;
}
RETRO_API void retro_set_video_refresh(retro_video_refresh_t video_refresh) {
	LOG_CALL;
	callbacks.video_refresh = video_refresh;
}
RETRO_API void retro_set_audio_sample(retro_audio_sample_t audio_sample) {
	LOG_CALL;
	callbacks.audio_sample = audio_sample;
}
RETRO_API void retro_set_audio_sample_batch(retro_audio_sample_batch_t audio_sample_batch) {
	LOG_CALL;
	callbacks.audio_sample_batch = audio_sample_batch;
}
RETRO_API void retro_set_input_poll(retro_input_poll_t input_poll) {
	LOG_CALL;
	callbacks.input_poll = input_poll;
}
RETRO_API void retro_set_input_state(retro_input_state_t input_state) {
	LOG_CALL;
	callbacks.input_state = input_state;
}
RETRO_API void retro_set_controller_port_device(unsigned, unsigned) { LOG_CALL; }

static constexpr const retro_system_info system_info = {
	"Oneup DMG core",
	"Beta",
	"gb",
	false,
	false
};
RETRO_API void retro_get_system_info(struct retro_system_info *info) {
	LOG_CALL;
	*info = system_info;
}


static constexpr const retro_system_av_info system_av_info = {
	{160, 144, 160, 144, 0.0},
	{60.0, 0.0}
};
RETRO_API void retro_get_system_av_info(struct retro_system_av_info *info) {
	LOG_CALL;
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
	return true;
}

RETRO_API unsigned retro_get_region(void) {
	LOG_CALL;
	return 0;
}

RETRO_API void *retro_get_memory_data(unsigned) {
	LOG_CALL;
	return nullptr;
}

RETRO_API size_t retro_get_memory_size(unsigned) {
	LOG_CALL;
	return 0;
}
