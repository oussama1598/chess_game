#include "renderer_3d.h"

Renderer_3D::Renderer_3D(Game *game) {
    game_ = game;

    gl_setup_();
    init_();
}

void Renderer_3D::gl_setup_() const {
    // enable z axe
    glEnable(GL_DEPTH_TEST);

   // glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // fill shape with color, GL_LINES will only draw the wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // for the alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer_3D::init_pices_meshes_() {
    std::array<std::string, 6> pieces_names{"pawn", "bishop", "king", "queen", "knight", "rook"};

    for (auto &piece_name: pieces_names)
        meshes_.insert(
                {
                        piece_name + "_mesh",
                        new Mesh(
                                "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/models/" +
                                piece_name + "_low.obj")
                }
        );
}


void Renderer_3D::init_() {
    // add shaders
    shaders_.insert({"main_shader", new Shader(
            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/src/renderers/3d/shaders/vertex_core.glsl",
            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/src/renderers/3d/shaders/fragment_core.glsl"
    )});

    // add materials
    materials_.insert(
            {
                    "main_material",
                    new Material(
                            glm::vec3(.2f),
                            glm::vec3(1.0f),
                            glm::vec3(1.f),
                            10.f,
                            0,
                            1
                    )
            }
    );
    materials_.insert(
            {
                    "dark_material",
                    new Material(
                            glm::vec3(0.f),
                            glm::vec3(0.01f),
                            glm::vec3(1.f),
                            50.f,
                            0,
                            1
                    )
            }
    );

    materials_.insert(
            {
                    "light_material",
                    new Material(
                            glm::vec3(0.2f),
                            glm::vec3(1.f),
                            glm::vec3(1.f),
                            50.f,
                            0,
                            1
                    )
            }
    );

    // add textures
    textures_.insert(
            {
                    "main_diffuse_texture",
                    new Texture(
                            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/table.png")
            }
    );

    textures_.insert(
            {
                    "main_specular_texture",
                    new Texture(
                            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/table_specular.png")
            }
    );

    init_pices_meshes_();

    // table mesh
    meshes_.insert(
            {
                    "table_mesh",
                    new Mesh(
                            "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/models/table.obj")
            }
    );

    // init the main scene
    main_scene_ = new Scene();

    main_scene_->add_camera(
            new Camera(
                    {0.135159, 3.11625, 6.1011},
                    {window_.get_width(), window_.get_height()}
            )
    );

    main_scene_->add_light(
            new Directional_Light(
                    {0.497147, 3.22812, 6.42233}
            )
    );

    main_scene_->add_light(
            new Point_Light(
                    glm::vec3{0.135159, 3.11625, 6.1011}
            )
    );

    // add skybox

    // TODO: delete this at destructuring
    SkyBox *sky_box = new SkyBox();
    sky_box->set_shader(shaders_.at("main_shader"));

    main_scene_->add_object(sky_box);

    // init game scene
    init_game_scene_();
}

void Renderer_3D::update_dt_fps_() {
    current_time_ = (float) glfwGetTime();
    dt_ = current_time_ - last_time_;
    last_time_ = current_time_;

    if (current_time_ - last_fps_time_ > 1) {
        fps_ = count_fps_;

        count_fps_ = 0;
        last_fps_time_ = current_time_;
    }

    ++count_fps_;
}

void Renderer_3D::handle_mouse_input_() {
    glfwGetCursorPos(window_.get_window(), &mouse_x_, &mouse_y_);

    if (first_move_) {
        last_mouse_x_ = mouse_x_;
        last_mouse_y_ = mouse_y_;
        first_move_ = false;
    }

    mouse_offset_x_ = mouse_x_ - last_mouse_x_;
    mouse_offset_y_ = mouse_y_ - last_mouse_y_;

    last_mouse_x_ = mouse_x_;
    last_mouse_y_ = mouse_y_;

    main_scene_->get_camera()->rotate(dt_, mouse_offset_x_, mouse_offset_y_, 5.f);
}

void Renderer_3D::handle_keyboard_input_() {
    if (glfwGetKey(window_.get_window(), GLFW_KEY_W) == GLFW_PRESS) {
        main_scene_->get_camera()->move(dt_, Camera::Movement_Direction::FORWARD, 1);
    }

    if (glfwGetKey(window_.get_window(), GLFW_KEY_S) == GLFW_PRESS) {
        main_scene_->get_camera()->move(dt_, Camera::Movement_Direction::BACKWARD, 1);
    }

    if (glfwGetKey(window_.get_window(), GLFW_KEY_D) == GLFW_PRESS) {
        main_scene_->get_camera()->move(dt_, Camera::Movement_Direction::RIGHT, 1);
    }

    if (glfwGetKey(window_.get_window(), GLFW_KEY_A) == GLFW_PRESS) {
        main_scene_->get_camera()->move(dt_, Camera::Movement_Direction::LEFT, 1);
    }

    if (glfwGetKey(window_.get_window(), GLFW_KEY_L) == GLFW_PRESS) {
        glm::vec3 pos = main_scene_->get_camera()->get_position();

        main_scene_->get_light(0)->set_direction(main_scene_->get_camera()->get_position());

        std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
    }

    if (glfwGetKey(window_.get_window(), GLFW_KEY_P) == GLFW_PRESS) {
        main_scene_->get_light(1)->set_position(main_scene_->get_camera()->get_position());
    }
}


void Renderer_3D::handle_inputs_() {
    handle_mouse_input_();
    handle_keyboard_input_();
}

void Renderer_3D::init_game_scene_() {
    Object *table = new Object();

    table->set_mesh(meshes_.at("table_mesh"));
    table->set_shader(shaders_.at("main_shader"));
    table->set_diffuse_texture(textures_.at("main_diffuse_texture"));
    table->set_specular_texture(textures_.at("main_specular_texture"));
    table->set_material(materials_.at("main_material"));

    main_scene_->add_object(table);

    std::vector<Player> players = game_->get_players();
    Board::piecesType pieces = game_->get_board_pieces();

    float step = 0.753176;

    for (int i = 0; i < (int) pieces.size(); ++i) {
        for (int j = 0; j < (int) pieces.at(i).size(); ++j) {
            Piece *piece = pieces[i][j];

            if (piece->get_player_id() == -1) {
                continue;
            }

            std::string material = players.at(piece->get_player_id()).is_dark ? "dark_material"
                                                                              : "light_material";

            Object *king = new Object();

            king->set_mesh(meshes_.at(pieces_meshes_.at(piece->get_symbol())));
            king->set_shader(shaders_.at("main_shader"));
            king->set_material(materials_.at(material));

            main_scene_->add_object(king);

            king->translate({-2.63 + (step * j), 0, 2.63 - (step * i)});

        }
    }
}

void Renderer_3D::render() {
    update_dt_fps_();
    handle_inputs_();

    glfwPollEvents();
    glClearColor(0.f, 0.f, 0.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    main_scene_->render();

    glfwSwapBuffers(window_.get_window());
    glFlush();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    window_.set_title("Chess Game - FPS: " + std::to_string(fps_));
}
