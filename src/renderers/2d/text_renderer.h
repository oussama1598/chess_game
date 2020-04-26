#pragma once

#include <map>
#include <string>
#include "SDL_ttf.h"

class Text_Renderer {
private:
    std::map<std::string, TTF_Font *> fonts_;
public:
    void add_font(const std::string &font_name, int font_size, const std::string &font_path);

    void
    render_text(const std::basic_string<char> &text, SDL_Rect rect, SDL_Renderer *renderer_,
                const std::string &font_name, SDL_Color color);
};
