// Copyright 2016 oneup

#include "libretro.h"

#include <iostream>
#include <memory>

#include <LRConnector.hpp>

static constexpr const char* LIBNAME = "oneup-gb-lr";
#define LOG_CALL do { std::cout << LIBNAME << " " << __func__ << std::endl; } while (0)

gblr::LRConnector g_core;

RETRO_API unsigned retro_api_version(void) {
	return g_core.ApiVersion();
}

RETRO_API void retro_init(void) {
	g_core.Init();
}

RETRO_API void retro_deinit(void) {
	g_core.DeInit();
}

RETRO_API void retro_run(void) {
	g_core.Run();
}

RETRO_API void retro_set_environment(retro_environment_t callback) {
	g_core.SetEnvironment(callback);
}

RETRO_API void retro_set_video_refresh(retro_video_refresh_t callback) {
	g_core.SetVideoRefresh(callback);
}

RETRO_API void retro_set_audio_sample(retro_audio_sample_t callback) {
	g_core.SetAudioSample(callback);
}

RETRO_API void retro_set_audio_sample_batch(retro_audio_sample_batch_t callback) {
	g_core.SetAudioSampleBatch(callback);
}

RETRO_API void retro_set_input_poll(retro_input_poll_t callback) {
	g_core.SetInputPoll(callback);
}

RETRO_API void retro_set_input_state(retro_input_state_t callback) {
	g_core.SetInputState(callback);
}

RETRO_API void retro_set_controller_port_device(unsigned, unsigned) { LOG_CALL; }

RETRO_API bool retro_load_game(const struct retro_game_info *game) {
	return g_core.LoadGame(game);
}

RETRO_API void retro_unload_game(void) {
	g_core.UnloadGame();
}

RETRO_API void retro_get_system_info(struct retro_system_info *info) {
	g_core.GetSystemInfo(info);
}

RETRO_API void retro_get_system_av_info(struct retro_system_av_info *info) {
	g_core.GetSystemAVInfo(info);
}

RETRO_API void *retro_get_memory_data(unsigned id) {
	return g_core.GetMemoryData(id);
}

RETRO_API size_t retro_get_memory_size(unsigned id) {
	return g_core.GetMemorySize(id);
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

