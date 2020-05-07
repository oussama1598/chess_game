#include "renderer_3D.h"

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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window_.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 440");
}

void Renderer_3D::init_pices_meshes_() {
    std::array<std::string, 6> pieces_names{"pawn", "bishop", "king", "queen", "knight", "rook"};

    for (auto &piece_name: pieces_names)
        meshes_.insert(
                {
                        piece_name + "_mesh",
                        new Mesh(
                                "./assets/models/" +
                                piece_name + "_low.obj")
                }
        );
}


void Renderer_3D::init_() {
    // add shaders
    shaders_.insert({"main_shader", new Shader(
            "./assets/shaders/vertex_core.glsl",
            "./assets/shaders/fragment_core.glsl"
    )});

    shaders_.insert({"selection_shader", new Shader(
            "./assets/shaders/vertex_core.glsl",
            "./assets/shaders/fragment_selection.glsl"
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
                            glm::vec3(0.1f),
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

    materials_.insert(
            {
                    "selection_material",
                    new Material(
                            glm::vec3(0.2f, 0.f, 0.f),
                            glm::vec3(0.26f, 0.52f, 0.95f),
                            glm::vec3(1.f),
                            50.f,
                            0,
                            1
                    )
            }
    );

    materials_.insert(
            {
                    "history_material",
                    new Material(
                            glm::vec3(0.2f, 0.f, 0.f),
                            glm::vec3(246 / 255, 231 / 255, 116 / 255),
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
                            "./assets/sprites/table.png")
            }
    );

    textures_.insert(
            {
                    "main_specular_texture",
                    new Texture(
                            "./assets/sprites/table_specular.png")
            }
    );

    init_pices_meshes_();

    // table mesh
    meshes_.insert(
            {
                    "table_mesh",
                    new Mesh(
                            "./assets/models/table.obj")
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

    // lights
    main_scene_->add_light(
            new Directional_Light(
                    glm::vec3{-0.2f, -1.0f, -0.3f},
                    glm::vec3{0.05},
                    glm::vec3{0.8f},
                    glm::vec3{0.5f}
            )
    );

    main_scene_->add_light(
            new Point_Light(
                    glm::vec3{-2.45823, 3.11625, 2.82323},
                    glm::vec3{0.05},
                    glm::vec3{0.8f},
                    glm::vec3{1.f},
                    1.0f,
                    0.09f,
                    0.032f
            )
    );

    main_scene_->add_light(
            new Point_Light(
                    glm::vec3{2.45823, 3.11625, 2.82323},
                    glm::vec3{0.05},
                    glm::vec3{0.8f},
                    glm::vec3{1.f},
                    1.0f,
                    0.09f,
                    0.032f
            )
    );

    main_scene_->add_light(
            new Point_Light(
                    glm::vec3{2.45823, 3.11625, -2.82323},
                    glm::vec3{0.05},
                    glm::vec3{0.8f},
                    glm::vec3{1.f},
                    1.0f,
                    0.09f,
                    0.032f
            )
    );

    main_scene_->add_light(
            new Point_Light(
                    glm::vec3{-2.45823, 3.11625, -2.82323},
                    glm::vec3{0.05},
                    glm::vec3{0.8f},
                    glm::vec3{1.f},
                    1.0f,
                    0.09f,
                    0.032f
            )
    );

    // add skybox

    // TODO: delete this at destructuring
    SkyBox *sky_box = new SkyBox();
    sky_box->set_shader(shaders_.at("main_shader"));
    main_scene_->set_sky_box(sky_box);

    // setting selection/hover shader, and material
    main_scene_->set_selection_shader(shaders_.at("selection_shader"));
    main_scene_->set_selection_material(materials_.at("selection_material"));
    main_scene_->set_hover_material(materials_.at("selection_material"));


    // init uniforms
    shaders_.at("main_shader")->set_uniform_1_i("last_from_cell.i", -1);
    shaders_.at("main_shader")->set_uniform_1_i("last_from_cell.j", -1);

    shaders_.at("main_shader")->set_uniform_1_i("last_to_cell.i", -1);
    shaders_.at("main_shader")->set_uniform_1_i("last_to_cell.j", -1);

    shaders_.at("main_shader")->set_uniform_3_fv("history_color",
                                                 glm::vec3(246.f / 255.f, 231.f / 255.f,
                                                           116.f / 255.f));

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

    if (glfwGetMouseButton(window_.get_window(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        main_scene_->get_camera()->rotate_with_mouse(dt_, mouse_offset_x_, mouse_offset_y_, 5.f);
    }

    // mouse buttons
    if (glfwGetMouseButton(window_.get_window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
        !mouse_buttons_.at("left")) {
        process_object_selection_(mouse_x_, mouse_y_);

        mouse_buttons_.at("left") = true;
    }

    if (glfwGetMouseButton(window_.get_window(), GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        mouse_buttons_.at("left") = false;
    }

    // mouse hover
    process_object_hover_(mouse_x_, mouse_y_);
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

    if (glfwGetKey(window_.get_window(), GLFW_KEY_P) == GLFW_PRESS) {

        glm::vec3 pos = main_scene_->get_camera()->get_position();
        main_scene_->get_light(1)->set_position(main_scene_->get_camera()->get_position());

        std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
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

            Object *piece_obj = new Object();

            game_pieces_objects_.insert({piece, piece_obj});

            piece_obj->set_mesh(meshes_.at(pieces_meshes_.at(piece->get_symbol())));
            piece_obj->set_shader(shaders_.at("main_shader"));
            piece_obj->set_material(materials_.at(material));

            main_scene_->add_object(piece_obj);

            piece_obj->translate({-2.63 + (step * j), 0, 2.63 - (step * i)});
        }
    }
}

Piece::piece_coordinates Renderer_3D::get_object_coordinates_(Object *object) {
    Board::piecesType pieces = game_->get_board_pieces();

    for (int i = 0; i < (int) pieces.size(); ++i) {
        for (int j = 0; j < (int) pieces.at(i).size(); ++j) {
            Piece *piece = pieces[i][j];

            if (piece->get_player_id() == -1) {
                continue;
            }

            if (game_pieces_objects_.at(piece) == object) return {i, j};
        }
    }

    return {-1, -1};
}

void Renderer_3D::render_selection_() {
    glClearColor(0.f, 0.f, 0.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    main_scene_->render_for_selection();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
}

void Renderer_3D::process_object_selection_(double x, double y) {
    render_selection_();

    unsigned char color[4];
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    int r = color[0];
    int g = color[1];
    int b = color[2];

    if (b == 0) { // b == 0 piece selection
        if (r == 0) return;

        Object *object = main_scene_->get_object(r);

        if (object == nullptr) return;

        Piece::piece_coordinates object_coordinates = get_object_coordinates_(object);

        Piece *piece = game_->get_board().get_piece_at(object_coordinates.line,
                                                       object_coordinates.column);

        if (piece->get_player_id() != game_->get_current_player()->player_id) {
            if (main_scene_->get_selected_object() != nullptr) {
                Piece::piece_coordinates from_coordinates = get_object_coordinates_(
                        main_scene_->get_selected_object());
                std::string from = Piece::get_id_from_coordinates(from_coordinates);
                std::string to = Piece::get_id_from_coordinates(object_coordinates);

                handle_move_(from, to);
            }

            return;
        }

        if (main_scene_->get_selected_index() == r)
            main_scene_->set_selected_index(-1);
        else
            main_scene_->set_selected_index(r);
    } else {  // b == 1 cell selection
        Object *object = main_scene_->get_selected_object();

        if (object != nullptr) {
            Piece::piece_coordinates from_coordinates = get_object_coordinates_(object);
            std::string from = Piece::get_id_from_coordinates(from_coordinates);
            std::string to = Piece::get_id_from_coordinates({g, r});

            handle_move_(from, to);
        }
    }
}

void Renderer_3D::process_object_hover_(double x, double y) {
    render_selection_();

    unsigned char color[4];
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);

    int r = color[0];
    int b = color[2];

    if (b != 0 or (r > 32 or r <= 0)) {
        main_scene_->set_hover_index(-1);

        return;
    }

    Object *object = main_scene_->get_object(r);

    if (object == nullptr) return;

    Piece::piece_coordinates object_coordinates = get_object_coordinates_(object);
    Piece *piece = game_->get_board().get_piece_at(object_coordinates.line,
                                                   object_coordinates.column);

    if (piece->get_player_id() == game_->get_current_player()->player_id) {
        main_scene_->set_hover_index(r);
    }
}

bool Renderer_3D::piece_exists_(Piece *piece) {
    Board::piecesType pieces = game_->get_board_pieces();

    for (int i = 0; i < (int) pieces.size(); ++i) {
        for (int j = 0; j < (int) pieces.at(i).size(); ++j) {
            if (pieces[i][j] == piece) return true;
        }
    }

    return false;
}

void Renderer_3D::check_for_board_changes_() {
    Board::piecesType pieces = game_->get_board_pieces();

    float step = 0.753176;

    for (int i = 0; i < (int) pieces.size(); ++i) {
        for (int j = 0; j < (int) pieces.at(i).size(); ++j) {
            Piece *piece = pieces[i][j];

            if (piece->get_player_id() == -1) {
                continue;
            }

            Object *piece_obj = game_pieces_objects_.at(piece);
            piece_obj->move_to({-2.63 + (step * j), 0, 2.63 - (step * i)});
        }
    }

    for (auto &piece_object: game_pieces_objects_) {
        if (piece_exists_(piece_object.first)) continue;

        main_scene_->remove_object(piece_object.second);
    }
}

void Renderer_3D::handle_move_(std::string &from, std::string &to) {
    try {
        game_->make_move(from, to);

        check_for_board_changes_();

        main_scene_->set_selected_index(-1);
    } catch (std::exception &error) {
        std::cout << error.what() << std::endl;
    }
}

void Renderer_3D::render_imgui_() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin(
                "Render options");

        ImGui::Checkbox("Enable Skybox", &main_scene_->sky_box_enabled);
        ImGui::Checkbox("Enable Reflections", &main_scene_->reflection_enabled);
        ImGui::Checkbox("Enable Directional Lighting", &main_scene_->enabled_directional_lighting);
        ImGui::Checkbox("Enable Point Light #1", &main_scene_->enabled_points_lights[0]);
        ImGui::Checkbox("Enable Point Light #2", &main_scene_->enabled_points_lights[1]);
        ImGui::Checkbox("Enable Point Light #3", &main_scene_->enabled_points_lights[2]);
        ImGui::Checkbox("Enable Point Light #4", &main_scene_->enabled_points_lights[3]);
        ImGui::Checkbox("Selection Rendering", &selection_rendring_);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("OpenGL version supported by this platform: %s, %s",
                    glGetString(GL_VERSION), glGetString(GL_VENDOR));
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer_3D::render_last_move_() {
    std::pair<std::string, std::string> last_move = game_->get_latest_move();

    if (!last_move.first.empty() && !last_move.second.empty()) {
        Piece::piece_coordinates from = Piece::get_piece_coordinates_from_id(last_move.first);
        Piece::piece_coordinates to = Piece::get_piece_coordinates_from_id(last_move.second);

        shaders_.at("main_shader")->set_uniform_1_i("last_from_cell.i", from.line);
        shaders_.at("main_shader")->set_uniform_1_i("last_from_cell.j", from.column);

        shaders_.at("main_shader")->set_uniform_1_i("last_to_cell.i", to.line);
        shaders_.at("main_shader")->set_uniform_1_i("last_to_cell.j", to.column);
    }
}

void Renderer_3D::render() {
    animation_handler.update();

    if (game_->get_current_player()->player_id != last_player_id_) {
        // TODO: move the camera to a known location
        main_scene_->get_camera()->set_position(default_camera_postions.at(last_player_id_));
        main_scene_->get_camera()->rotate_around_origin({0, -90 + last_player_id_ * 180, 0}, true);

        animation_handler.add_animation(180.0 / 6.0, 1, [this](float step) {
            main_scene_->get_camera()->rotate_around_origin({0, -step * 6, 0});
            main_scene_->get_camera()->rotate(step * 6);
        });

        last_player_id_ = game_->get_current_player()->player_id;
    }

    update_dt_fps_();
    handle_inputs_();

    glfwPollEvents();
    glClearColor(0.f, 0.f, 0.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_last_move_();

    if (selection_rendring_)
        main_scene_->render_for_selection();
    else
        main_scene_->render();

    render_imgui_();

    glfwSwapBuffers(window_.get_window());
    glFlush();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);

    window_.set_title("Chess Game - FPS: " + std::to_string(fps_));
}

Renderer_3D::~Renderer_3D() {
    for (auto &mesh: meshes_) {
        delete mesh.second;
    }

    for (auto &shader: shaders_) {
        delete shader.second;
    }

    for (auto &material: materials_) {
        delete material.second;
    }

    for (auto &texture: textures_) {
        delete texture.second;
    }

    delete main_scene_;
}
