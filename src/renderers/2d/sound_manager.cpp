#include "sound_manager.h"

void Sound_Manager::add_sound(const std::string &sound_name,const std::string &file_path) {
    Sound sound;

    SDL_LoadWAV(file_path.c_str(), &sound.spec, &sound.buffer, &sound.length);

    sound.device_id = SDL_OpenAudioDevice(NULL, 0, &sound.spec, NULL, 0);

    sounds_.insert({sound_name, sound});
}

void Sound_Manager::play_sound(std::string &&sound_name) {
    Sound sound = sounds_.at(sound_name);

    SDL_QueueAudio(sound.device_id, sound.buffer, sound.length);
    SDL_PauseAudioDevice(sound.device_id, 0);
}
