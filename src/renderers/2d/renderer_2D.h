#pragma once

#include <stdexcept>
#include <vector>
#include <map>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "sound_manager.h"
#include "game/Game.h"

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

    Game *game_{};

    int mouse_x_{0};
    int mouse_y_{0};
    int mouse_i_{0};
    int mouse_j_{0};

    bool selected_{false};

    int selected_i_{0};
    int selected_j_{0};

    struct flash {
        unsigned int start_time{0};
        unsigned int duration{1000};
        unsigned int flashes_per_second{8};
        bool show{false};
        Piece::piece_coordinates position{0, 0};
        SDL_Color color{255, 0, 0, 255};
    };

    flash flash_message_;

    std::vector<std::string> guides_{};

    SDL_Window *window_{nullptr};

    SDL_Renderer *renderer_{nullptr};
    SDL_Texture *table_texture_{nullptr};
    SDL_Texture *guide_circle_texture_{nullptr};
    SDL_Texture *texture_{nullptr};
    TTF_Font *font_{nullptr};

    Sound_Manager sound_manager_{};

    std::map<std::string, std::string> sounds_{
            {"move",    "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sounds/move.wav"},
            {"capture", "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sounds/move.wav"},
            {"illegal", "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sounds/illegal.wav"}
    };

    SDL_Cursor *arrow_cursor_;
    SDL_Cursor *hand_cursor_;

    std::map<char, std::map<std::string, SDL_Rect>> pieces_texture_rectangles_{
            {'P', {
                          {"light", (SDL_Rect) {0, 0, 100, 100}},
                          {"dark", (SDL_Rect) {0, 100, 100, 100}}
                  }
            },    // pawn
            {
             'B', {
                          {"light", (SDL_Rect) {100, 0, 100, 100}},
                          {"dark", (SDL_Rect) {100, 100, 100, 100}}
                  }
            },   // bishop
            {
             'K', {
                          {"light", (SDL_Rect) {200, 0, 100, 100}},
                          {"dark", (SDL_Rect) {200, 100, 100, 100}}
                  }
            },     // king
            {
             'Q', {
                          {"light", (SDL_Rect) {300, 0, 100, 100}},
                          {"dark", (SDL_Rect) {300, 100, 100, 100}}
                  }
            },   // queen
            {
             'k', {
                          {"light", (SDL_Rect) {400, 0, 100, 100}},
                          {"dark", (SDL_Rect) {400, 100, 100, 100}}
                  }
            },   // knight
            {
             'R', {
                          {"light", (SDL_Rect) {500, 0, 100, 100}},
                          {"dark", (SDL_Rect) {500, 100, 100, 100}}
                  }
            }   // rook
    };

    void load_fonts_();

    void load_texture_();

    void load_sounds_();

    void init_table_();

    void
    handle_move_(Piece::piece_coordinates from_coordinates,
                 Piece::piece_coordinates to_coordinates);

    void handle_mouse_press_down_(SDL_MouseButtonEvent &event);

    void handle_mouse_press_up_(SDL_MouseButtonEvent event);

    void handle_events_();

    void show_flash_message_(Piece::piece_coordinates position,
                             SDL_Color color = {255, 0, 0, 255}, unsigned int duration = 1000);

    void render_fps_();

    void render_table_();

    void render_cursor_();

    void render_flash_message_();

    void render_guides_();

    void render_game_();

public:

    explicit Renderer2D(const char *title);

    ~Renderer2D();

    [[nodiscard]] inline bool is_running() const { return is_running_; }

    inline void set_game(Game *game) { game_ = game; }

    void render();
};
