#pragma once

#include <array>
#include <map>

#include <imgui.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "engine/window.h"
#include "engine/scene.h"
#include "engine/sky_box.h"
#include "engine/camera.h"
#include "engine/directional_light.h"
#include "engine/point_light.h"
#include "engine/shader.h"
#include "engine/animation_handler.h"
#include "common/sound_manager.h"

#include "game/Game.h"

class Renderer_3D {
private:
    Window window_;
    Sound_Manager sound_manager_{};

    bool is_ai_{false};

    bool is_running_{true};

    Animation_Handler animation_handler_;

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
    double orbit_cursor_x_{0};
    double orbit_cursor_y_{0};
    bool first_move_{true};

    std::map<std::string, bool> mouse_buttons_{
            {"left",  false},
            {"right", false}
    };


    std::map<std::string, Mesh *> meshes_;
    std::map<std::string, Shader *> shaders_;
    std::map<std::string, Material *> materials_;
    std::map<std::string, Texture *> textures_;

    static constexpr int shadow_map_size_{3072};
    GLuint shadow_framebuffer_{0};
    GLuint shadow_depth_texture_{0};
    bool shadow_map_dirty_{true};
    glm::vec3 shadow_light_position_{-4.f, 8.f, 5.f};
    glm::mat4 light_space_matrix_{1.f};
    float environment_yaw_degrees_{0.f};

    static constexpr int msaa_samples_{4};
    GLuint hdr_framebuffer_{0};
    std::array<GLuint, 2> hdr_multisample_textures_{};
    GLuint hdr_depth_stencil_renderbuffer_{0};
    GLuint resolve_framebuffer_{0};
    std::array<GLuint, 2> resolved_textures_{};
    std::array<GLuint, 2> bloom_framebuffers_{};
    std::array<GLuint, 2> bloom_textures_{};
    GLuint fullscreen_vertex_array_{0};
    int final_bloom_texture_index_{0};

    Scene *main_scene_{nullptr};
    Object *board_tile_tops_{nullptr};

    bool onboarding_dismissed_{false};
    float onboarding_start_time_{0.f};

    Game *game_;
    std::array<glm::vec3, 2> default_camera_positions_{
            glm::vec3{.55f, 6.10f, 8.90f},
            glm::vec3{-.55f, 6.10f, -8.90f}
    };

    std::map<std::string, std::string> sounds_{
            {"move",    "./assets/sounds/move.wav"},
            {"capture", "./assets/sounds/capture.wav"},
            {"illegal", "./assets/sounds/illegal.wav"}
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

    struct flash {
        float start_time{0.0};
        float duration{1.0};
        unsigned int flashes_per_second{8};
        bool show{false};
        Piece::piece_coordinates position{-1, -1};
    };

    flash flash_message_;

    struct Landing_Feedback {
        bool active{false};
        float start_time{0.f};
        float duration{.26f};
        Piece::piece_coordinates cell{-1, -1};
    } landing_feedback_;

    bool pending_move_feedback_{false};
    bool pending_move_is_capture_{false};
    Piece::piece_coordinates pending_move_destination_{-1, -1};

    std::vector<std::string> guides_{};

    // options
    bool selection_rendering_{false};
    bool show_settings_{false};
    bool settings_key_pressed_{false};
    bool reset_key_pressed_{false};

    typedef std::function<void(int x, int y)> window_move_callback;
    typedef std::function<void(const std::string &from, const std::string &to)> move_callback;

    window_move_callback on_window_move_callback_;
    move_callback on_move_callback_;

private:
    void gl_setup_() const;

    void init_pices_meshes_();

    void load_sounds_();

    void init_();

    void init_game_scene_();

    void init_salon_scene_();

    void init_shadow_map_();

    void init_post_processing_();

    void destroy_post_processing_();

    void render_shadow_map_();

    void bind_shadow_map_();

    void render_selected_outline_();

    void render_late_board_feedback_();

    void resolve_hdr_framebuffer_();

    void render_bloom_();

    void composite_hdr_();

    void update_dt_fps_();

    void handle_mouse_input_();

    void handle_keyboard_input_();

    void handle_inputs_();

    Piece::piece_coordinates get_object_coordinates_(Object *object);

    Piece::piece_coordinates get_board_coordinates_at_(double x, double y) const;

    void process_object_selection_(double x, double y);

    void process_object_hover_(double x, double y);

    bool piece_exists_(Piece *piece);

    void render_selection_();

    void show_flash_message_(Piece::piece_coordinates coordinates);

    void handle_move_(std::string &from, std::string &to);

    void render_imgui_();

    void render_board_coordinates_();

    void render_move_guide_overlay_();

    bool project_world_to_screen_(const glm::vec3 &world_position,
                                  ImVec2 &screen_position) const;

    void render_flash_message_();

    void render_last_move_();

    void render_guides_();

    void render_landing_feedback_();

    void set_interaction_cell_(const std::string &uniform_name,
                               Piece::piece_coordinates coordinates);

    void reset_camera_();

    static void scroll_callback_(GLFWwindow *window, double offset_x, double offset_y);

public:

    Renderer_3D(Game *game, bool is_ai);

    ~Renderer_3D();

    inline bool is_running() { return is_running_; }

    void check_for_board_changes();

    void render();

    void on_window_move(window_move_callback callback);

    void on_move(move_callback callback);

    void play_move_sound(bool is_capture);

    void prepare_move_feedback(const std::string &destination, bool is_capture);

    inline bool is_animating() { return animation_handler_.is_animating(); }
};
