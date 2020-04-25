#pragma once

#include <map>
#include "SDL.h"

class Sound_Manager {
private:
    struct Sound {
        SDL_AudioSpec spec;
        Uint32 length;
        Uint8 *buffer;
        SDL_AudioDeviceID device_id;
    };

    std::map<std::string, Sound> sounds_;

public:
    void add_sound(const std::string &sound_name, const std::string &file_path);

    void play_sound(std::string &&sound_name);
};
