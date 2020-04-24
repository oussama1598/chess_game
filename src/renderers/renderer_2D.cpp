#include "renderer_2D.h"

Renderer2D::Renderer2D([[maybe_unused]] const char *title) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        throw std::runtime_error(SDL_GetError());

    window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800,
                               SDL_WINDOW_ALLOW_HIGHDPI);

    if (window_ == nullptr)
        throw std::runtime_error("Could not create a window_");

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_PRESENTVSYNC);

    if (renderer_ == nullptr)
        throw std::runtime_error("Could not create a renderer");

    is_running_ = true;

    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
}

Renderer2D::~Renderer2D() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Renderer2D::handle_events() {
    SDL_Event event;

    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running_ = false;
            break;
    }
}

void Renderer2D::render() {
    // TODO: this has to change
    handle_events();

    SDL_RenderClear(renderer_);

    SDL_RenderPresent(renderer_);
}
