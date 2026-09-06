#include "sound_manager.h"

#include <iostream>

Sound_Manager::Sound_Manager() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) == 0) {
        audio_initialized_ = true;
    } else {
        std::cerr << "Audio disabled: " << SDL_GetError() << '\n';
    }
}

Sound_Manager::~Sound_Manager() {
    clear();
}

void Sound_Manager::add_sound(const std::string &sound_name, const std::string &file_path) {
    if (!audio_initialized_) {
        return;
    }

    Sound sound{};
    SDL_AudioSpec sound_spec{};

    if (SDL_LoadWAV(file_path.c_str(), &sound_spec, &sound.buffer, &sound.length) == nullptr) {
        std::cerr << "Could not load sound '" << file_path << "': " << SDL_GetError() << '\n';
        return;
    }

    if (device_id_ == 0) {
        device_id_ = SDL_OpenAudioDevice(nullptr, 0, &sound_spec, &audio_spec_, 0);

        if (device_id_ == 0) {
            std::cerr << "Audio output disabled: " << SDL_GetError() << '\n';
            SDL_FreeWAV(sound.buffer);
            return;
        }
    } else if (sound_spec.freq != audio_spec_.freq ||
               sound_spec.format != audio_spec_.format ||
               sound_spec.channels != audio_spec_.channels) {
        std::cerr << "Ignoring sound with an incompatible audio format: " << file_path << '\n';
        SDL_FreeWAV(sound.buffer);
        return;
    }

    const auto inserted = sounds_.insert({sound_name, sound});
    if (!inserted.second) {
        SDL_FreeWAV(sound.buffer);
    }
}

void Sound_Manager::play_sound(const std::string &sound_name) {
    const auto sound = sounds_.find(sound_name);
    if (device_id_ == 0 || sound == sounds_.end()) {
        return;
    }

    SDL_ClearQueuedAudio(device_id_);
    if (SDL_QueueAudio(device_id_, sound->second.buffer, sound->second.length) == 0) {
        SDL_PauseAudioDevice(device_id_, 0);
    }
}

void Sound_Manager::clear() noexcept {
    if (device_id_ != 0) {
        SDL_ClearQueuedAudio(device_id_);
        SDL_CloseAudioDevice(device_id_);
        device_id_ = 0;
    }

    for (auto &entry: sounds_) {
        SDL_FreeWAV(entry.second.buffer);
    }
    sounds_.clear();

    if (audio_initialized_) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        audio_initialized_ = false;
    }
}
