#include "text_renderer.h"

#include <stdexcept>

Text_Renderer::~Text_Renderer() {
    clear();
}

void
Text_Renderer::add_font(const std::string &font_name, int font_size, const std::string &font_path) {
    TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
    if (font == nullptr) {
        throw std::runtime_error(TTF_GetError());
    }
    fonts_.insert({font_name, font});
}

void
Text_Renderer::render_text(const std::basic_string<char> &text, SDL_Rect rect, SDL_Renderer *renderer_,
                          const std::string &font_name,
                          SDL_Color color) {
    SDL_Surface *text_surface = TTF_RenderText_Blended(
            fonts_.at(font_name),
            text.c_str(),
            color);

    if (text_surface == nullptr) {
        throw std::runtime_error(TTF_GetError());
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_, text_surface);

    if (texture == nullptr) {
        SDL_FreeSurface(text_surface);
        throw std::runtime_error(SDL_GetError());
    }

    // get the text's width and height
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);

    // place the text to right
    rect.x -= rect.w;

    SDL_RenderCopy(renderer_, texture, nullptr, &rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(texture);
}

void Text_Renderer::clear() noexcept {
    for (auto &entry: fonts_) {
        TTF_CloseFont(entry.second);
    }
    fonts_.clear();
}
