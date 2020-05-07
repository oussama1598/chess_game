#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include <map>

#include <imgui.h>

#include "vendor/imgui_impl_glfw.h"
#include "vendor/imgui_impl_opengl3.h"

#include "engine/window.h"
#include "engine/scene.h"
#include "engine/sky_box.h"
#include "engine/camera.h"
#include "engine/directional_light.h"
#include "engine/point_light.h"
#include "engine/shader.h"
#include "engine/animation_handler.h"

#include "game/Game.h"

class Renderer_3D {
private:
    Window window_;

    Animation_Handler animation_handler;

    float dt_{0};
    float last_time_{0};
    float current_time_;

    int fps_{0};
    int count_fps_{0};
    int last_fps_time_{0};

    double last_mouse_x_{0};
    double last_mouse_y_{0};
    double mouse_x_{0};
    double mouse_y_{0};
    double mouse_offset_x_{0};
    double mouse_offset_y_{0};
    bool first_move_{true};

    std::map<std::string, bool> mouse_buttons_{
            {"left",  false},
            {"right", false}
    };


    std::map<std::string, Mesh *> meshes_;
    std::map<std::string, Shader *> shaders_;
    std::map<std::string, Material *> materials_;
    std::map<std::string, Texture *> textures_;

    Scene *main_scene_;

    Game *game_;
    int last_player_id_{0};

    std::array<glm::vec3, 2> default_camera_postions{
            glm::vec3{0.135159, 3.11625, 6.1011},
            glm::vec3{-0.135159, 3.11625, -6.1011}
    };

    std::map<char, std::string> pieces_meshes_{
            {'P', "pawn_mesh"},    // pawn
            {'B', "bishop_mesh"},   // bishop
            {'K', "king_mesh"},     // king
            {'Q', "queen_mesh"},   // queen
            {'k', "knight_mesh"},   // knight
            {'R', "rook_mesh"}   // rook
    };

    std::map<Piece *, Object *> game_pieces_objects_;

    // options
    bool selection_rendring_{false};

private:
    void gl_setup_() const;

    void init_pices_meshes_();

    void init_();

    void init_game_scene_();

    void update_dt_fps_();

    void handle_mouse_input_();

    void handle_keyboard_input_();

    void handle_inputs_();

    Piece::piece_coordinates get_object_coordinates_(Object *object);

    void process_object_selection_(double x, double y);

    void process_object_hover_(double x, double y);

    bool piece_exists_(Piece *piece);

    void render_selection_();

    void check_for_board_changes_();

    void handle_move_(std::string &from, std::string &to);

    void render_last_move_();

public:

    Renderer_3D(Game *game);

    ~Renderer_3D();

    inline bool is_running() { return window_.is_running(); }

    void render_imgui_();

    void render();
};

#endif
