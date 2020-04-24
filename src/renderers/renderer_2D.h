#pragma once

#include <stdexcept>
#include "SDL.h"

class Renderer2D {
private:
    bool is_running_{false};

    SDL_Window *window_{nullptr};
    SDL_Renderer *renderer_{nullptr};
public:
    explicit Renderer2D(const char *title);

    ~Renderer2D();

    [[nodiscard]] inline bool is_running() const { return is_running_; }

    void handle_events();

    void render();
};
