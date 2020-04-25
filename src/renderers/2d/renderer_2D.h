#pragma once

#include <stdexcept>
#include <vector>
#include <map>
#include "SDL.h"
#include "SDL_image.h"
#include "../../modules/Game.h"

class Renderer2D {
private:
    unsigned int last_time_{0};
    int current_frames_count_{0};
    int FPS_{0};

    int width_{800};
    int height_{800};

    int cols_{8};
    int rows_{8};

    int piece_width_{0};
    int piece_height_{0};

    bool is_running_{false};

    Game *game_;

    int mouse_x_{0};
    int mouse_y_{0};
    int mouse_i_{0};
    int mouse_j_{0};

    bool selected_{false};

    int selected_i_{0};
    int selected_j_{0};

    SDL_Window *window_{nullptr};
    SDL_Renderer *renderer_{nullptr};
    SDL_Texture *texture_{nullptr};

    std::map<char, std::map<std::string, SDL_Rect>> pieces_texture_rectangles_{
            {'K', {
                          {"light", (SDL_Rect) {2, 5, 95, 95}},
                          {"dark", (SDL_Rect) {2, 110, 95, 95}}
                  }
            },     // king
            {'Q', {
                          {"light", (SDL_Rect) {110, 5, 95, 95}},
                          {"dark", (SDL_Rect) {110, 110, 95, 95}}
                  }
            },   // queen
            {'B', {
                          {"light", (SDL_Rect) {215, 5, 95, 95}},
                          {"dark", (SDL_Rect) {215, 110, 95, 95}}
                  }
            },   // bishop
            {'k', {
                          {"light", (SDL_Rect) {322, 5, 95, 95}},
                          {"dark", (SDL_Rect) {322, 110, 95, 95}}
                  }
            },   // knight
            {'R', {
                          {"light", (SDL_Rect) {430, 5, 95, 95}},
                          {"dark", (SDL_Rect) {430, 110, 95, 95}}
                  }
            },   // rook
            {'P', {
                          {"light", (SDL_Rect) {535, 5, 95, 95}},
                          {"dark", (SDL_Rect) {535, 110, 95, 95}}
                  }
            }    // pawn
    };

    std::vector<SDL_Rect> dark_table_rectangles_{};
    std::vector<SDL_Rect> light_table_rectangles_{};

    void load_texture_();

    void init_table_();

    void
    handle_move_(Piece::piece_coordinates from_coordinates,
                 Piece::piece_coordinates to_coordinates);

    void handle_mouse_press_down_(SDL_MouseButtonEvent &event);

    void handle_mouse_press_up_(SDL_MouseButtonEvent event);

    void handle_events_();

    void render_table_();

    void render_game_();

    void render_cursor_();

public:

    explicit Renderer2D(const char *title);

    ~Renderer2D();

    [[nodiscard]] inline bool is_running() const { return is_running_; }

    inline void set_game(Game *game) { game_ = game; }

    void render();
};
