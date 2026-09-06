#pragma once

#include <map>
#include <string>

#include <SDL.h>

class Sound_Manager {
private:
    struct Sound {
        Uint32 length{0};
        Uint8 *buffer{nullptr};
    };

    std::map<std::string, Sound> sounds_;
    SDL_AudioSpec audio_spec_{};
    SDL_AudioDeviceID device_id_{0};
    bool audio_initialized_{false};

public:
    Sound_Manager();
    ~Sound_Manager();

    Sound_Manager(const Sound_Manager &) = delete;
    Sound_Manager &operator=(const Sound_Manager &) = delete;

    void add_sound(const std::string &sound_name, const std::string &file_path);
    void play_sound(const std::string &sound_name);
    void clear() noexcept;
};
