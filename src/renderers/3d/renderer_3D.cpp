#include "renderer_3D.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <stdexcept>

#include "engine/primitive_mesh.h"

Renderer_3D::Renderer_3D(Game *game, bool is_ai) : is_ai_{is_ai}, game_{game} {
    gl_setup_();
    load_sounds_();
    init_();

    glfwSetWindowUserPointer(window_.get_window(), this);
    glfwSetScrollCallback(window_.get_window(), scroll_callback_);
    onboarding_start_time_ = static_cast<float>(glfwGetTime());
}

void Renderer_3D::gl_setup_() const {
    // enable z axe
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

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
    io.Fonts->AddFontFromFileTTF("./assets/fonts/OpenSans-Regular.ttf", 16.f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 10.f;
    style.FrameRounding = 6.f;
    style.WindowPadding = ImVec2(14.f, 12.f);
    style.ItemSpacing = ImVec2(9.f, 8.f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.035f, 0.055f, 0.09f, 0.94f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.13f, 0.20f, 1.f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.13f, 0.20f, 0.30f, 1.f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.30f, 0.82f, 0.70f, 1.f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.10f, 0.16f, 1.f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.47f, 0.41f, 1.f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.60f, 0.51f, 1.f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.38f, 0.34f, 1.f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.13f, 0.47f, 0.41f, .72f);

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window_.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 440");
}

void Renderer_3D::load_sounds_() {
    for (const auto &sound: sounds_) {
        sound_manager_.add_sound(sound.first, sound.second);
    }
}

void Renderer_3D::init_pices_meshes_() {
    std::array<std::string, 6> pieces_names{"pawn", "bishop", "king", "queen", "knight", "rook"};

    for (auto &piece_name: pieces_names) {
        const std::string model_path = "./assets/models/" + piece_name + "_low.obj";
        Mesh *mesh = std::filesystem::exists(model_path)
                     ? new Mesh(model_path)
                     : new Mesh(Procedural_Mesh::make_chess_piece(piece_name), std::vector<GLuint>{});

        meshes_.insert(
                {
                        piece_name + "_mesh",
                        mesh
                }
        );
    }
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

    shaders_.insert({"depth_shader", new Shader(
            "./assets/shaders/depth_vertex.glsl",
            "./assets/shaders/depth_fragment.glsl"
    )});

    shaders_.insert({"outline_shader", new Shader(
            "./assets/shaders/outline_vertex.glsl",
            "./assets/shaders/outline_fragment.glsl"
    )});

    shaders_.insert({"board_feedback_shader", new Shader(
            "./assets/shaders/vertex_core.glsl",
            "./assets/shaders/board_feedback_fragment.glsl"
    )});

    shaders_.insert({"blur_shader", new Shader(
            "./assets/shaders/post_vertex.glsl",
            "./assets/shaders/blur_fragment.glsl"
    )});

    shaders_.insert({"composite_shader", new Shader(
            "./assets/shaders/post_vertex.glsl",
            "./assets/shaders/composite_fragment.glsl"
    )});

    init_shadow_map_();
    init_post_processing_();

    // add materials
    materials_.insert(
            {
                    "main_material",
                    new Material(
                            glm::vec3(1.f),
                            .34f,
                            0.f,
                            1.f,
                            .10f,
                            0,
                            1,
                            0,
                            0.f,
                            0.f,
                            0.f,
                            .60f,
                            .30f
                    )
            }
    );
    materials_.insert(
            {
                    "dark_material",
                    new Material(
                            glm::vec3(.040f, .021f, .010f),
                            .40f,
                            0.f,
                            1.f,
                            .16f,
                            0,
                            1,
                            2,
                            .075f,
                            .045f,
                            .013f
                    )
            }
    );

    materials_.insert(
            {
                    "light_material",
                    new Material(
                            glm::vec3(.54f, .44f, .31f),
                            .40f,
                            0.f,
                            1.f,
                            .12f,
                            0,
                            1,
                            1,
                            .022f,
                            .016f,
                            .0035f
                    )
            }
    );

    materials_.insert(
            {
                    "selection_material",
                    new Material(
                            glm::vec3(.18f, .76f, .66f),
                            .32f,
                            0.f,
                            1.f,
                            .2f,
                            0,
                            1
                    )
            }
    );

    materials_.insert(
            {
                    "hover_material",
                    new Material(
                            glm::vec3(.12f, .42f, .50f),
                            .38f,
                            0.f,
                            1.f,
                            .12f,
                            0,
                            1
                    )
            }
    );

    materials_.insert(
            {
                    "frame_material",
                    new Material(
                            glm::vec3(.12f, .040f, .012f),
                            .34f,
                            0.f,
                            1.f,
                            .24f,
                            0,
                            1
                    )
            }
    );

    materials_.insert(
            {
                    "light_tile_edge_material",
                    new Material(glm::vec3(.34f, .21f, .10f), .46f, 0.f, 1.f,
                                 .08f, 0, 1)
            }
    );
    materials_.insert(
            {
                    "dark_tile_edge_material",
                    new Material(glm::vec3(.055f, .022f, .008f), .50f, 0.f, 1.f,
                                 .05f, 0, 1)
            }
    );
    materials_.insert(
            {
                    "groove_material",
                    new Material(glm::vec3(.009f, .0045f, .0025f), .65f, 0.f, 1.f,
                                 0.f, 0, 1)
            }
    );
    materials_.insert(
            {
                    "brass_material",
                    new Material(glm::vec3(.42f, .23f, .055f), .34f, .92f, 1.f,
                                 .08f, 0, 1)
            }
    );

    materials_.insert(
            {
                    "tabletop_material",
                    new Material(
                            glm::vec3(1.f),
                            .48f,
                            0.f,
                            1.f,
                            .08f,
                            0,
                            1
                    )
            }
    );

    materials_.insert(
            {
                    "salon_wall_material",
                    new Material(glm::vec3(1.f, 1.20f, 1.80f), .82f, 0.f, 1.f,
                                 .02f, 0, 1)
            }
    );
    materials_.insert(
            {
                    "salon_floor_material",
                    new Material(glm::vec3(.34f, .32f, .31f), .72f, 0.f, 1.f,
                                 .04f, 0, 1)
            }
    );
    materials_.insert(
            {
                    "salon_molding_material",
                    new Material(glm::vec3(.20f, .085f, .025f), .48f, 0.f, 1.f,
                                 .05f, 0, 1)
            }
    );
    materials_.insert(
            {
                    "salon_chair_material",
                    new Material(glm::vec3(.72f, .66f, .58f), .56f, 0.f, 1.f,
                                 .06f, 0, 1)
            }
    );
    materials_.insert(
            {
                    "salon_frame_material",
                    new Material(glm::vec3(.86f, .74f, .52f), .42f, .20f, 1.f,
                                 .14f, 0, 1)
            }
    );
    materials_.insert(
            {
                    "salon_canvas_material",
                    new Material(glm::vec3(.78f, .73f, .66f), .74f, 0.f, 1.f,
                                 0.f, 0, 1)
            }
    );

    materials_.insert(
            {
                    "history_material",
                    new Material(
                            glm::vec3(246.f / 255.f, 231.f / 255.f, 116.f / 255.f),
                            .35f,
                            0.f,
                            1.f,
                            .1f,
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

    textures_.insert(
            {
                    "tabletop_diffuse_texture",
                    new Texture(
                            "./assets/textures/wood_table_001/wood_table_001_diff_cinematic_2k.jpg")
            }
    );

    textures_.insert(
            {
                    "tabletop_specular_texture",
                    new Texture(
                            "./assets/textures/wood_table_001/wood_table_001_spec_2k.jpg")
            }
    );

    textures_.insert({"salon_wall_diffuse_texture",
                      new Texture("./assets/textures/navy_linen/navy_linen_albedo.png")});
    textures_.insert({"salon_wall_specular_texture",
                      new Texture("./assets/textures/navy_linen/navy_linen_spec.png")});
    textures_.insert({"salon_floor_diffuse_texture",
                      new Texture("./assets/textures/salon/herringbone_diff_1k.jpg")});
    textures_.insert({"salon_floor_specular_texture",
                      new Texture("./assets/textures/salon/herringbone_spec_1k.jpg")});
    textures_.insert({"salon_chair_diffuse_texture",
                      new Texture("./assets/textures/salon/wooden_chair_diff_1k.jpg")});
    textures_.insert({"salon_chair_specular_texture",
                      new Texture("./assets/textures/salon/wooden_chair_spec_1k.jpg")});
    textures_.insert({"salon_frame_diffuse_texture",
                      new Texture("./assets/textures/salon/picture_frame_diff_1k.jpg")});
    textures_.insert({"salon_frame_specular_texture",
                      new Texture("./assets/textures/salon/picture_frame_spec_1k.jpg")});
    textures_.insert({"salon_canvas_diffuse_texture",
                      new Texture("./assets/textures/salon/picture_canvas_diff_1k.jpg")});
    textures_.insert({"salon_canvas_specular_texture",
                      new Texture("./assets/textures/salon/picture_canvas_spec_1k.jpg")});

    init_pices_meshes_();

    meshes_.insert({"board_tile_tops_mesh",
                    new Mesh(Procedural_Mesh::make_board_tile_tops(), std::vector<GLuint>{})});
    meshes_.insert({"light_tile_edges_mesh",
                    new Mesh(Procedural_Mesh::make_board_tile_edges(true), std::vector<GLuint>{})});
    meshes_.insert({"dark_tile_edges_mesh",
                    new Mesh(Procedural_Mesh::make_board_tile_edges(false), std::vector<GLuint>{})});
    meshes_.insert({"board_frame_ring_mesh",
                    new Mesh(Procedural_Mesh::make_board_frame_ring(), std::vector<GLuint>{})});
    meshes_.insert({"board_base_mesh",
                    new Mesh(Procedural_Mesh::make_box(), std::vector<GLuint>{})});
    meshes_.insert({"furniture_box_mesh",
                    new Mesh(Procedural_Mesh::make_beveled_box(), std::vector<GLuint>{})});
    meshes_.insert({"table_leg_mesh",
                    new Mesh(Procedural_Mesh::make_tapered_prism(), std::vector<GLuint>{})});
    meshes_.insert({"salon_wall_mesh",
                    new Mesh(Procedural_Mesh::make_cylinder_band(17.f, -3.25f, 17.f, 80, 10.f),
                             std::vector<GLuint>{})});
    meshes_.insert({"salon_wainscot_mesh",
                    new Mesh(Procedural_Mesh::make_cylinder_band(16.86f, -3.17f, -.30f, 80, 10.f),
                             std::vector<GLuint>{})});
    meshes_.insert({"salon_baseboard_mesh",
                    new Mesh(Procedural_Mesh::make_cylinder_band(16.70f, -3.08f, -2.74f, 80, 10.f),
                             std::vector<GLuint>{})});
    meshes_.insert({"salon_chair_rail_mesh",
                    new Mesh(Procedural_Mesh::make_cylinder_band(16.70f, -.44f, -.18f, 80, 10.f),
                             std::vector<GLuint>{})});
    meshes_.insert({"salon_cornice_mesh",
                    new Mesh(Procedural_Mesh::make_cylinder_band(16.70f, 10.f, 10.32f, 80, 10.f),
                             std::vector<GLuint>{})});
    meshes_.insert({"salon_panel_rail_mesh",
                    new Mesh(Procedural_Mesh::make_cylinder_arc_band(
                                     16.64f, -.045f, .045f, -28.f, 28.f, 28, 2.5f),
                             std::vector<GLuint>{})});
    meshes_.insert({"salon_chair_mesh",
                    new Mesh("./assets/models/salon/wooden_chair.obj")});
    meshes_.insert({"salon_frame_mesh",
                    new Mesh("./assets/models/salon/picture_frame.obj")});
    meshes_.insert({"salon_canvas_mesh",
                    new Mesh("./assets/models/salon/picture_canvas.obj")});

    // init the main scene
    main_scene_ = new Scene();

    main_scene_->add_camera(
            new Camera(
                    default_camera_positions_[0],
                    {window_.get_width(), window_.get_height()}
            )
    );

    // lights
    main_scene_->add_light(
            new Directional_Light(
                    glm::vec3{4.f, -8.f, -5.f},
                    glm::vec3{0.07f, 0.08f, 0.10f},
                    glm::vec3{2.80f, 2.10f, 1.55f},
                    glm::vec3{0.54f, 0.46f, 0.40f}
            )
    );

    main_scene_->add_light(
            new Point_Light(
                    glm::vec3{4.5f, 4.0f, 2.0f},
                    glm::vec3{0.f},
                    glm::vec3{0.36f, 0.48f, 0.72f},
                    glm::vec3{0.20f, 0.26f, 0.36f},
                    1.0f,
                    0.07f,
                    0.025f
            )
    );

    main_scene_->add_light(
            new Point_Light(
                    glm::vec3{3.8f, 4.6f, 2.0f},
                    glm::vec3{0.01f},
                    glm::vec3{0.44f, 0.60f, 0.82f},
                    glm::vec3{0.38f, 0.48f, 0.62f},
                    1.0f,
                    0.09f,
                    0.032f
            )
    );

    main_scene_->add_light(
            new Point_Light(
                    glm::vec3{3.2f, 3.8f, -3.4f},
                    glm::vec3{0.01f},
                    glm::vec3{0.28f, 0.38f, 0.58f},
                    glm::vec3{0.24f, 0.32f, 0.48f},
                    1.0f,
                    0.09f,
                    0.032f
            )
    );

    main_scene_->add_light(
            new Point_Light(
                    glm::vec3{-2.45f, 1.22f, -14.65f},
                    glm::vec3{0.012f, .008f, .004f},
                    glm::vec3{.85f, .48f, .28f},
                    glm::vec3{.48f, .28f, .18f},
                    1.0f,
                    0.045f,
                    0.012f
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
    main_scene_->set_hover_material(materials_.at("hover_material"));


    // init uniforms
    shaders_.at("main_shader")->set_uniform_1_i("last_from_cell.i", -1);
    shaders_.at("main_shader")->set_uniform_1_i("last_from_cell.j", -1);

    shaders_.at("main_shader")->set_uniform_1_i("last_to_cell.i", -1);
    shaders_.at("main_shader")->set_uniform_1_i("last_to_cell.j", -1);

    shaders_.at("main_shader")->set_uniform_3_fv("history_color",
                                                 glm::vec3(.92f, .63f, .18f));

    shaders_.at("main_shader")->set_uniform_3_fv("error_color",
                                                 glm::vec3(.96f, .20f, .26f));

    shaders_.at("main_shader")->set_uniform_1_i("guides_count", 0);
    shaders_.at("main_shader")->set_uniform_3_fv("guides_color",
                                                 glm::vec3(.18f, .78f, .66f));

    set_interaction_cell_("selected_cell", {-1, -1});
    set_interaction_cell_("hover_cell", {-1, -1});
    set_interaction_cell_("landing_cell", {-1, -1});
    shaders_.at("main_shader")->set_uniform_1_f("landing_progress", -1.f);
    shaders_.at("main_shader")->set_uniform_3_fv("selected_color",
                                                 glm::vec3(.08f, .72f, .52f));
    shaders_.at("main_shader")->set_uniform_3_fv("hover_color",
                                                 glm::vec3(.20f, .52f, .82f));
    shaders_.at("main_shader")->set_uniform_1_f("environment_yaw_degrees",
                                                 environment_yaw_degrees_);

    // init game scene
    init_game_scene_();
}

void Renderer_3D::init_shadow_map_() {
    const glm::mat4 light_projection = glm::ortho(
            -12.5f, 12.5f, -12.5f, 12.5f, 1.f, 30.f);
    const glm::mat4 light_view = glm::lookAt(
            shadow_light_position_, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    light_space_matrix_ = light_projection * light_view;

    glGenFramebuffers(1, &shadow_framebuffer_);
    glGenTextures(1, &shadow_depth_texture_);
    glBindTexture(GL_TEXTURE_2D, shadow_depth_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
                 shadow_map_size_, shadow_map_size_, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    const GLfloat border_color[]{1.f, 1.f, 1.f, 1.f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    glBindFramebuffer(GL_FRAMEBUFFER, shadow_framebuffer_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, shadow_depth_texture_, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        throw std::runtime_error("Could not create the directional shadow framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer_3D::init_post_processing_() {
    const int width = window_.get_width();
    const int height = window_.get_height();
    const GLenum color_attachments[]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

    glGenFramebuffers(1, &hdr_framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, hdr_framebuffer_);
    glGenTextures(static_cast<GLsizei>(hdr_multisample_textures_.size()),
                  hdr_multisample_textures_.data());
    for (size_t index = 0; index < hdr_multisample_textures_.size(); ++index) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, hdr_multisample_textures_[index]);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa_samples_, GL_RGBA16F,
                                width, height, GL_TRUE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, color_attachments[index],
                               GL_TEXTURE_2D_MULTISAMPLE,
                               hdr_multisample_textures_[index], 0);
    }
    glDrawBuffers(2, color_attachments);

    glGenRenderbuffers(1, &hdr_depth_stencil_renderbuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, hdr_depth_stencil_renderbuffer_);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa_samples_, GL_DEPTH24_STENCIL8,
                                     width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, hdr_depth_stencil_renderbuffer_);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Could not create the multisampled HDR framebuffer");
    }

    glGenFramebuffers(1, &resolve_framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, resolve_framebuffer_);
    glGenTextures(static_cast<GLsizei>(resolved_textures_.size()), resolved_textures_.data());
    for (size_t index = 0; index < resolved_textures_.size(); ++index) {
        glBindTexture(GL_TEXTURE_2D, resolved_textures_[index]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0,
                     GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, color_attachments[index], GL_TEXTURE_2D,
                               resolved_textures_[index], 0);
    }
    glDrawBuffers(2, color_attachments);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Could not create the HDR resolve framebuffer");
    }

    const int bloom_width = std::max(1, width / 2);
    const int bloom_height = std::max(1, height / 2);
    glGenFramebuffers(static_cast<GLsizei>(bloom_framebuffers_.size()),
                      bloom_framebuffers_.data());
    glGenTextures(static_cast<GLsizei>(bloom_textures_.size()), bloom_textures_.data());
    for (size_t index = 0; index < bloom_framebuffers_.size(); ++index) {
        glBindFramebuffer(GL_FRAMEBUFFER, bloom_framebuffers_[index]);
        glBindTexture(GL_TEXTURE_2D, bloom_textures_[index]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bloom_width, bloom_height, 0,
                     GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               bloom_textures_[index], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Could not create a bloom framebuffer");
        }
    }

    glGenVertexArrays(1, &fullscreen_vertex_array_);
    shaders_.at("blur_shader")->set_uniform_1_i("source_image", 7);
    shaders_.at("composite_shader")->set_uniform_1_i("scene_color", 7);
    shaders_.at("composite_shader")->set_uniform_1_i("bloom_color", 8);
    shaders_.at("composite_shader")->set_uniform_1_i("emissive_color", 9);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer_3D::destroy_post_processing_() {
    if (fullscreen_vertex_array_ != 0) glDeleteVertexArrays(1, &fullscreen_vertex_array_);
    glDeleteTextures(static_cast<GLsizei>(bloom_textures_.size()), bloom_textures_.data());
    glDeleteFramebuffers(static_cast<GLsizei>(bloom_framebuffers_.size()),
                         bloom_framebuffers_.data());
    glDeleteTextures(static_cast<GLsizei>(resolved_textures_.size()), resolved_textures_.data());
    if (resolve_framebuffer_ != 0) glDeleteFramebuffers(1, &resolve_framebuffer_);
    if (hdr_depth_stencil_renderbuffer_ != 0) {
        glDeleteRenderbuffers(1, &hdr_depth_stencil_renderbuffer_);
    }
    glDeleteTextures(static_cast<GLsizei>(hdr_multisample_textures_.size()),
                     hdr_multisample_textures_.data());
    if (hdr_framebuffer_ != 0) glDeleteFramebuffers(1, &hdr_framebuffer_);
}

void Renderer_3D::render_selected_outline_() {
    Object *selected = main_scene_->get_selected_object();
    if (selected == nullptr) return;

    Shader *outline_shader = shaders_.at("outline_shader");
    main_scene_->get_camera()->attach_to_shader(outline_shader);
    outline_shader->set_uniform_3_fv("outline_color", glm::vec3(.30f, 1.05f, .90f));

    const GLboolean culling_was_enabled = glIsEnabled(GL_CULL_FACE);
    GLint previous_cull_mode = GL_BACK;
    GLint previous_depth_function = GL_LESS;
    glGetIntegerv(GL_CULL_FACE_MODE, &previous_cull_mode);
    glGetIntegerv(GL_DEPTH_FUNC, &previous_depth_function);

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    outline_shader->set_uniform_1_f("outline_width", 0.f);
    Shader *piece_shader = selected->get_shader();
    selected->set_shader(outline_shader);
    selected->draw();

    glStencilMask(0x00);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glColorMaski(1, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    outline_shader->set_uniform_1_f("outline_width", .0195f);
    selected->draw();
    selected->set_shader(piece_shader);

    glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glColorMaski(1, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glDepthFunc(previous_depth_function);
    glStencilMask(0xFF);
    glDisable(GL_STENCIL_TEST);
    glCullFace(previous_cull_mode);
    if (!culling_was_enabled) glDisable(GL_CULL_FACE);
}

void Renderer_3D::render_late_board_feedback_() {
    if (board_tile_tops_ == nullptr || main_scene_->get_selected_index() == -1 ||
        guides_.empty()) {
        return;
    }

    Shader *feedback_shader = shaders_.at("board_feedback_shader");
    main_scene_->get_camera()->attach_to_shader(feedback_shader);
    feedback_shader->set_uniform_3_fv("guides_color", glm::vec3(.18f, .78f, .66f));
    feedback_shader->set_uniform_1_i("guides_count", static_cast<int>(guides_.size()));
    for (int index = 0; index < static_cast<int>(guides_.size()); ++index) {
        const Piece::piece_coordinates coordinates =
                Piece::get_piece_coordinates_from_id(guides_[index]);
        const std::string prefix = "guides[" + std::to_string(index) + "]";
        feedback_shader->set_uniform_1_i(prefix + ".i", coordinates.line);
        feedback_shader->set_uniform_1_i(prefix + ".j", coordinates.column);
        const Piece *target = game_->get_board().get_piece_at(
                coordinates.line, coordinates.column);
        feedback_shader->set_uniform_1_i(
                "guide_capture[" + std::to_string(index) + "]",
                target->get_player_id() == -1 ? 0 : 1);
    }

    const GLboolean depth_was_enabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean depth_write_was_enabled = GL_TRUE;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_write_was_enabled);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader *board_shader = board_tile_tops_->get_shader();
    board_tile_tops_->set_shader(feedback_shader);
    board_tile_tops_->draw();
    board_tile_tops_->set_shader(board_shader);

    glDepthMask(depth_write_was_enabled);
    if (depth_was_enabled) glEnable(GL_DEPTH_TEST);
}

void Renderer_3D::resolve_hdr_framebuffer_() {
    const int width = window_.get_width();
    const int height = window_.get_height();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, hdr_framebuffer_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolve_framebuffer_);
    for (int attachment = 0; attachment < 2; ++attachment) {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
        glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachment);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer_3D::render_bloom_() {
    const int bloom_width = std::max(1, window_.get_width() / 2);
    const int bloom_height = std::max(1, window_.get_height() / 2);
    Shader *blur_shader = shaders_.at("blur_shader");

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glViewport(0, 0, bloom_width, bloom_height);
    glBindVertexArray(fullscreen_vertex_array_);

    GLuint source_texture = resolved_textures_[1];
    for (int pass = 0; pass < 4; ++pass) {
        const int target = pass % 2;
        glBindFramebuffer(GL_FRAMEBUFFER, bloom_framebuffers_[target]);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        blur_shader->set_uniform_2_fv("blur_direction",
                                      pass % 2 == 0 ? glm::vec2(1.f, 0.f)
                                                    : glm::vec2(0.f, 1.f));
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, source_texture);
        blur_shader->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        blur_shader->unbind();
        source_texture = bloom_textures_[target];
        final_bloom_texture_index_ = target;
    }

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer_3D::composite_hdr_() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_.get_width(), window_.get_height());
    glClearColor(.018f, .030f, .052f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, resolved_textures_[0]);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, bloom_textures_[final_bloom_texture_index_]);
    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, resolved_textures_[1]);
    glBindVertexArray(fullscreen_vertex_array_);
    Shader *composite_shader = shaders_.at("composite_shader");
    composite_shader->bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    composite_shader->unbind();
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
}

void Renderer_3D::render_shadow_map_() {
    GLint previous_framebuffer = 0;
    GLint previous_viewport[4]{};
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_framebuffer);
    glGetIntegerv(GL_VIEWPORT, previous_viewport);
    const GLboolean polygon_offset_was_enabled = glIsEnabled(GL_POLYGON_OFFSET_FILL);

    glViewport(0, 0, shadow_map_size_, shadow_map_size_);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_framebuffer_);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.f, 4.f);

    Shader *depth_shader = shaders_.at("depth_shader");
    depth_shader->set_uniform_matrix_4_fv("light_space_matrix", light_space_matrix_);
    main_scene_->render_depth(depth_shader);

    if (!polygon_offset_was_enabled) glDisable(GL_POLYGON_OFFSET_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, previous_framebuffer);
    glViewport(previous_viewport[0], previous_viewport[1],
               previous_viewport[2], previous_viewport[3]);
}

void Renderer_3D::bind_shadow_map_() {
    Shader *shader = shaders_.at("main_shader");
    shader->set_uniform_matrix_4_fv("light_space_matrix", light_space_matrix_);
    shader->set_uniform_1_i("shadows_enabled", 1);
    shader->set_uniform_1_i("shadow_map", 6);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, shadow_depth_texture_);
    glActiveTexture(GL_TEXTURE0);
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

    const bool ui_owns_mouse = ImGui::GetIO().WantCaptureMouse;

    const bool right_pressed =
            glfwGetMouseButton(window_.get_window(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    // Right-drag is reserved for the camera. Do not let a stale ImGui capture
    // flag suppress it when focus moves between the Qt side panel and GLFW.
    if (right_pressed && !mouse_buttons_.at("right")) {
        onboarding_dismissed_ = true;
        mouse_buttons_.at("right") = true;
        orbit_cursor_x_ = mouse_x_;
        orbit_cursor_y_ = mouse_y_;
        mouse_offset_x_ = 0.0;
        mouse_offset_y_ = 0.0;
        glfwSetInputMode(window_.get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else if (right_pressed && mouse_buttons_.at("right")) {
        main_scene_->get_camera()->orbit(mouse_offset_x_, mouse_offset_y_);
    }

    if (!right_pressed && mouse_buttons_.at("right")) {
        mouse_buttons_.at("right") = false;
        glfwSetInputMode(window_.get_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPos(window_.get_window(), orbit_cursor_x_, orbit_cursor_y_);
        first_move_ = true;
    }

    // mouse buttons
    if (!ui_owns_mouse &&
        glfwGetMouseButton(window_.get_window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS &&
        !mouse_buttons_.at("left")) {
        onboarding_dismissed_ = true;
        process_object_selection_(mouse_x_, mouse_y_);

        mouse_buttons_.at("left") = true;
    }

    if (glfwGetMouseButton(window_.get_window(), GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        mouse_buttons_.at("left") = false;
    }

    if (ui_owns_mouse || right_pressed) {
        main_scene_->set_hover_index(-1);
        set_interaction_cell_("hover_cell", {-1, -1});
    } else {
        process_object_hover_(mouse_x_, mouse_y_);
    }
}

void Renderer_3D::handle_keyboard_input_() {
    if (glfwGetKey(window_.get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_.get_window(), GLFW_TRUE);
    }

    const bool settings_pressed =
            glfwGetKey(window_.get_window(), GLFW_KEY_F1) == GLFW_PRESS;
    if (settings_pressed && !settings_key_pressed_) {
        onboarding_dismissed_ = true;
        show_settings_ = !show_settings_;
    }
    settings_key_pressed_ = settings_pressed;

    const bool reset_pressed = glfwGetKey(window_.get_window(), GLFW_KEY_R) == GLFW_PRESS;
    if (reset_pressed && !reset_key_pressed_) {
        onboarding_dismissed_ = true;
        reset_camera_();
    }
    reset_key_pressed_ = reset_pressed;

    if (ImGui::GetIO().WantCaptureKeyboard) {
        return;
    }

    if (glfwGetKey(window_.get_window(), GLFW_KEY_W) == GLFW_PRESS) {
        onboarding_dismissed_ = true;
        main_scene_->get_camera()->move(dt_, Camera::Movement_Direction::FORWARD, 1);
    }

    if (glfwGetKey(window_.get_window(), GLFW_KEY_S) == GLFW_PRESS) {
        onboarding_dismissed_ = true;
        main_scene_->get_camera()->move(dt_, Camera::Movement_Direction::BACKWARD, 1);
    }

    if (glfwGetKey(window_.get_window(), GLFW_KEY_D) == GLFW_PRESS) {
        onboarding_dismissed_ = true;
        main_scene_->get_camera()->move(dt_, Camera::Movement_Direction::RIGHT, 1);
    }

    if (glfwGetKey(window_.get_window(), GLFW_KEY_A) == GLFW_PRESS) {
        onboarding_dismissed_ = true;
        main_scene_->get_camera()->move(dt_, Camera::Movement_Direction::LEFT, 1);
    }
}


void Renderer_3D::handle_inputs_() {
    handle_mouse_input_();
    handle_keyboard_input_();

    int x, y;
    glfwGetWindowPos(window_.get_window(), &x, &y);

    if (on_window_move_callback_) {
        on_window_move_callback_(x, y);
    }
}

void Renderer_3D::init_salon_scene_() {
    auto add_object = [this](const std::string &mesh_name,
                             const std::string &material_name,
                             const glm::vec3 &position,
                             const glm::vec3 &scale,
                             Texture *diffuse_texture,
                             Texture *specular_texture,
                             const glm::vec2 &texture_scale,
                             bool casts_shadow,
                             const glm::vec3 &rotation) {
        Object *object = new Object();
        object->set_mesh(meshes_.at(mesh_name));
        object->set_shader(shaders_.at("main_shader"));
        object->set_material(materials_.at(material_name));
        if (diffuse_texture != nullptr) object->set_diffuse_texture(diffuse_texture);
        if (specular_texture != nullptr) object->set_specular_texture(specular_texture);
        object->set_texture_scale(texture_scale);
        object->set_selectable(false);
        object->set_casts_shadow(casts_shadow);
        object->scale_ = scale;
        object->move_to(position);
        object->rotate(rotation);
        main_scene_->add_object(object);
        return object;
    };

    Texture *wood_diffuse = textures_.at("tabletop_diffuse_texture");
    Texture *wood_specular = textures_.at("tabletop_specular_texture");

    // The room shell replaces the floating skybox horizon with a grounded interior.
    add_object("board_base_mesh", "salon_floor_material",
               {0.f, -3.25f, 0.f}, {16.8f, .08f, 16.8f},
               textures_.at("salon_floor_diffuse_texture"),
               textures_.at("salon_floor_specular_texture"), {3.8f, 3.8f}, false, {});
    add_object("salon_wall_mesh", "salon_wall_material",
               {}, {1.f, 1.f, 1.f},
               textures_.at("salon_wall_diffuse_texture"),
               textures_.at("salon_wall_specular_texture"), {1.f, 1.5f}, false, {});
    add_object("salon_wainscot_mesh", "tabletop_material",
               {}, {1.f, 1.f, 1.f}, wood_diffuse, wood_specular,
               {.55f, 1.35f}, false, {});
    add_object("salon_baseboard_mesh", "salon_molding_material",
               {}, {1.f, 1.f, 1.f}, nullptr, nullptr, {1.f, 1.f}, false, {});
    add_object("salon_chair_rail_mesh", "salon_molding_material",
               {}, {1.f, 1.f, 1.f}, nullptr, nullptr, {1.f, 1.f}, false, {});
    add_object("salon_cornice_mesh", "salon_molding_material",
               {}, {1.f, 1.f, 1.f}, nullptr, nullptr, {1.f, 1.f}, false, {});

    // Restrained moulding gives the rear wall readable architectural scale.
    for (float x: {-7.1f, -2.45f, 2.20f, 6.85f}) {
        constexpr float panel_radius = 16.64f;
        constexpr float radians_to_degrees = 57.2957795f;
        const float wall_z = -std::sqrt(panel_radius * panel_radius - x * x) + .045f;
        const float tangent_yaw = -std::asin(x / panel_radius) * radians_to_degrees;
        add_object("furniture_box_mesh", "salon_molding_material",
                   {x, -1.64f, wall_z}, {.045f, 1.05f, .035f},
                   nullptr, nullptr, {1.f, 1.f}, false, {0.f, tangent_yaw, 0.f});
    }
    for (float y: {-2.62f, -.66f}) {
        add_object("salon_panel_rail_mesh", "salon_molding_material",
                   {0.f, y, 0.f}, {1.f, 1.f, 1.f},
                   nullptr, nullptr, {1.f, 1.f}, false, {});
    }

    // A compact, fully modelled table keeps its apron and legs visible at gameplay zoom.
    add_object("furniture_box_mesh", "tabletop_material",
               {0.f, -.34f, 0.f}, {4.70f, .14f, 4.50f},
               wood_diffuse, wood_specular, {2.f, 2.f}, true, {});
    add_object("furniture_box_mesh", "tabletop_material",
               {0.f, -.72f, 4.25f}, {4.32f, .25f, .10f},
               wood_diffuse, wood_specular, {2.f, 1.f}, true, {});
    add_object("furniture_box_mesh", "tabletop_material",
               {0.f, -.72f, -4.25f}, {4.32f, .25f, .10f},
               wood_diffuse, wood_specular, {2.f, 1.f}, true, {});
    add_object("furniture_box_mesh", "tabletop_material",
               {4.45f, -.72f, 0.f}, {.10f, .25f, 4.05f},
               wood_diffuse, wood_specular, {2.f, 1.f}, true, {});
    add_object("furniture_box_mesh", "tabletop_material",
               {-4.45f, -.72f, 0.f}, {.10f, .25f, 4.05f},
               wood_diffuse, wood_specular, {2.f, 1.f}, true, {});
    for (float x: {-4.25f, 4.25f}) {
        for (float z: {-4.05f, 4.05f}) {
            add_object("table_leg_mesh", "tabletop_material",
                       {x, -1.83f, z}, {.27f, 1.35f, .27f},
                       wood_diffuse, wood_specular, {1.f, 2.f}, true, {});
        }
    }

    // One asymmetric hero prop and one focal artwork stop the scene feeling procedural.
    add_object("salon_chair_mesh", "salon_chair_material",
               {-7.f, -3.17f, -8.8f}, {1.52f, 1.52f, 1.52f},
               textures_.at("salon_chair_diffuse_texture"),
               textures_.at("salon_chair_specular_texture"), {1.f, 1.f}, true,
               {0.f, 25.f, 0.f});
    add_object("salon_frame_mesh", "salon_frame_material",
               {2.20f, -.45f, -16.45f}, {4.20f, 4.20f, 4.20f},
               textures_.at("salon_frame_diffuse_texture"),
               textures_.at("salon_frame_specular_texture"), {1.f, 1.f}, false, {});
    add_object("salon_canvas_mesh", "salon_canvas_material",
               {2.20f, -.45f, -16.41f}, {4.20f, 4.20f, 4.20f},
               textures_.at("salon_canvas_diffuse_texture"),
               textures_.at("salon_canvas_specular_texture"), {1.f, 1.f}, false, {});

}

void Renderer_3D::init_game_scene_() {
    init_salon_scene_();

    Object *lower_plinth = new Object();
    lower_plinth->set_mesh(meshes_.at("board_base_mesh"));
    lower_plinth->set_shader(shaders_.at("main_shader"));
    lower_plinth->set_material(materials_.at("frame_material"));
    lower_plinth->set_selectable(false);
    lower_plinth->scale_ = {3.36f, .035f, 3.36f};
    lower_plinth->move_to({0.f, -.135f, 0.f});
    main_scene_->add_object(lower_plinth);

    Object *groove_underlay = new Object();
    groove_underlay->set_mesh(meshes_.at("board_base_mesh"));
    groove_underlay->set_shader(shaders_.at("main_shader"));
    groove_underlay->set_material(materials_.at("groove_material"));
    groove_underlay->set_selectable(false);
    groove_underlay->scale_ = {3.022f, .012f, 3.022f};
    groove_underlay->move_to({0.f, -.043f, 0.f});
    main_scene_->add_object(groove_underlay);

    Object *frame_ring = new Object();
    frame_ring->set_mesh(meshes_.at("board_frame_ring_mesh"));
    frame_ring->set_shader(shaders_.at("main_shader"));
    frame_ring->set_material(materials_.at("frame_material"));
    frame_ring->set_selectable(false);
    main_scene_->add_object(frame_ring);

    for (int rail = 0; rail < 4; ++rail) {
        Object *brass_inlay = new Object();
        brass_inlay->set_mesh(meshes_.at("board_base_mesh"));
        brass_inlay->set_shader(shaders_.at("main_shader"));
        brass_inlay->set_material(materials_.at("brass_material"));
        brass_inlay->set_selectable(false);
        if (rail < 2) {
            brass_inlay->scale_ = {3.075f, .005f, .011f};
            brass_inlay->move_to({0.f, .020f, rail == 0 ? 3.064f : -3.064f});
        } else {
            brass_inlay->scale_ = {.011f, .005f, 3.075f};
            brass_inlay->move_to({rail == 2 ? 3.064f : -3.064f, .020f, 0.f});
        }
        main_scene_->add_object(brass_inlay);
    }

    Object *light_tile_edges = new Object();
    light_tile_edges->set_mesh(meshes_.at("light_tile_edges_mesh"));
    light_tile_edges->set_shader(shaders_.at("main_shader"));
    light_tile_edges->set_material(materials_.at("light_tile_edge_material"));
    light_tile_edges->set_selectable(false);
    main_scene_->add_object(light_tile_edges);

    Object *dark_tile_edges = new Object();
    dark_tile_edges->set_mesh(meshes_.at("dark_tile_edges_mesh"));
    dark_tile_edges->set_shader(shaders_.at("main_shader"));
    dark_tile_edges->set_material(materials_.at("dark_tile_edge_material"));
    dark_tile_edges->set_selectable(false);
    main_scene_->add_object(dark_tile_edges);

    board_tile_tops_ = new Object();
    board_tile_tops_->set_mesh(meshes_.at("board_tile_tops_mesh"));
    board_tile_tops_->set_shader(shaders_.at("main_shader"));
    board_tile_tops_->set_diffuse_texture(textures_.at("main_diffuse_texture"));
    board_tile_tops_->set_specular_texture(textures_.at("main_specular_texture"));
    board_tile_tops_->set_material(materials_.at("main_material"));
    board_tile_tops_->set_chess_board(true);
    board_tile_tops_->set_selectable(false);
    main_scene_->add_object(board_tile_tops_);

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
            piece_obj->set_surface_seed(std::fmod(
                    static_cast<float>(i * 8 + j) * .6180339f, 1.f));

            piece_obj->rotate({0.f, -180.f * (float) (piece->get_player_id()), 0.f});

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

Piece::piece_coordinates Renderer_3D::get_board_coordinates_at_(double x, double y) const {
    if (x < 0 || y < 0 || x >= window_.get_width() || y >= window_.get_height()) {
        return {-1, -1};
    }

    const float normalized_x = 2.f * static_cast<float>(x) / window_.get_width() - 1.f;
    const float normalized_y = 1.f - 2.f * static_cast<float>(y) / window_.get_height();
    const Camera *camera = main_scene_->get_camera();
    const glm::mat4 inverse_view_projection = glm::inverse(
            camera->get_projection_matrix() * camera->get_view_matrix());

    glm::vec4 near_point = inverse_view_projection *
                           glm::vec4(normalized_x, normalized_y, -1.f, 1.f);
    glm::vec4 far_point = inverse_view_projection *
                          glm::vec4(normalized_x, normalized_y, 1.f, 1.f);
    near_point /= near_point.w;
    far_point /= far_point.w;

    const glm::vec3 ray_origin{near_point};
    const glm::vec3 ray_direction = glm::normalize(glm::vec3(far_point - near_point));
    if (std::abs(ray_direction.y) < .0001f) return {-1, -1};

    const float distance = -ray_origin.y / ray_direction.y;
    if (distance < 0.f) return {-1, -1};

    const glm::vec3 board_point = ray_origin + ray_direction * distance;
    constexpr float step = .753176f;
    constexpr float board_min_x = -2.63f - step * .5f;
    constexpr float board_max_z = 2.63f + step * .5f;
    const int column = static_cast<int>(std::floor((board_point.x - board_min_x) / step));
    const int row = static_cast<int>(std::floor((board_max_z - board_point.z) / step));

    if (row < 0 || row >= 8 || column < 0 || column >= 8) return {-1, -1};
    return {row, column};
}

void Renderer_3D::render_selection_() {
    glClearColor(0.f, 0.f, 0.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    main_scene_->render_for_selection();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(GL_TEXTURE0);
}

void Renderer_3D::process_object_selection_(double x, double y) {
    if (animation_handler_.is_animating() || game_->is_game_ended() ||
        x < 0 || y < 0 || x >= window_.get_width() || y >= window_.get_height()) {
        return;
    }

    const Piece::piece_coordinates coordinates = get_board_coordinates_at_(x, y);
    if (coordinates.line < 0 || coordinates.column < 0) return;

    Piece *piece = game_->get_board().get_piece_at(coordinates.line, coordinates.column);
    if (piece->get_player_id() != game_->get_current_player()->player_id) {
        Object *selected_object = main_scene_->get_selected_object();
        if (selected_object != nullptr) {
            const Piece::piece_coordinates from_coordinates =
                    get_object_coordinates_(selected_object);
            std::string from = Piece::get_id_from_coordinates(from_coordinates);
            std::string to = Piece::get_id_from_coordinates(coordinates);
            handle_move_(from, to);
        }
        return;
    }

    Object *piece_object = game_pieces_objects_.at(piece);
    const int object_index = main_scene_->get_object_index(piece_object);
    if (object_index < 0) return;

    if (main_scene_->get_selected_index() == object_index) {
        main_scene_->set_selected_index(-1);
        main_scene_->get_camera()->set_selection_focus(false);
        guides_.clear();
        render_guides_();
        set_interaction_cell_("selected_cell", {-1, -1});
    } else {
        main_scene_->set_selected_index(object_index);
        main_scene_->get_camera()->set_selection_focus(true);
        set_interaction_cell_("selected_cell", coordinates);
        guides_ = game_->get_board().get_possible_moves_for(
                *game_->get_current_player(),
                Piece::get_id_from_coordinates(coordinates));
        render_guides_();
    }
}

void Renderer_3D::process_object_hover_(double x, double y) {
    if (animation_handler_.is_animating() ||
        x < 0 || y < 0 || x >= window_.get_width() || y >= window_.get_height()) {
        main_scene_->set_hover_index(-1);
        set_interaction_cell_("hover_cell", {-1, -1});
        return;
    }

    const Piece::piece_coordinates coordinates = get_board_coordinates_at_(x, y);
    if (coordinates.line < 0 || coordinates.column < 0) {
        main_scene_->set_hover_index(-1);
        set_interaction_cell_("hover_cell", {-1, -1});
        return;
    }

    Piece *piece = game_->get_board().get_piece_at(coordinates.line, coordinates.column);

    if (piece->get_player_id() == game_->get_current_player()->player_id) {
        main_scene_->set_hover_index(main_scene_->get_object_index(
                game_pieces_objects_.at(piece)));
        set_interaction_cell_("hover_cell", coordinates);
    } else {
        main_scene_->set_hover_index(-1);
        set_interaction_cell_("hover_cell", {-1, -1});
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

void Renderer_3D::check_for_board_changes() {
    shadow_map_dirty_ = true;
    Board::piecesType pieces = game_->get_board_pieces();

    float step = 0.753176;

    for (int i = 0; i < (int) pieces.size(); ++i) {
        for (int j = 0; j < (int) pieces.at(i).size(); ++j) {
            Piece *piece = pieces[i][j];

            if (piece->get_player_id() == -1) {
                continue;
            }

            Object *piece_obj = game_pieces_objects_.at(piece);
            glm::vec3 end_position{-2.63 + (step * j), 0, 2.63 - (step * i)};

            if (piece_obj->get_position() != end_position) {
                const float travel_in_squares = glm::length(glm::vec2(
                        end_position.x - piece_obj->get_position().x,
                        end_position.z - piece_obj->get_position().z)) / step;
                const float duration = std::clamp(.28f + travel_in_squares * .045f,
                                                  .32f, .55f);
                const float arc_height = std::clamp(.14f + travel_in_squares * .022f,
                                                    .16f, .28f);
                const bool triggers_feedback = pending_move_feedback_ &&
                                               pending_move_destination_.line == i &&
                                               pending_move_destination_.column == j;
                const bool is_capture = pending_move_is_capture_;
                if (triggers_feedback) pending_move_feedback_ = false;

                animation_handler_.add_position_animation(piece_obj->get_position(), end_position,
                                                          duration,
                                                          [this, piece_obj, arc_height,
                                                           triggers_feedback, is_capture, i, j]
                                                          (glm::vec3 position, float alpha) {
                                                              position.y += std::sin(alpha * 3.14159265f) *
                                                                            arc_height;
                                                              piece_obj->move_to(position);

                                                              if (triggers_feedback && alpha >= 1.f) {
                                                                  play_move_sound(is_capture);
                                                                  landing_feedback_.active = true;
                                                                  landing_feedback_.start_time =
                                                                          static_cast<float>(glfwGetTime());
                                                                  landing_feedback_.cell = {i, j};
                                                              }
                                                          });
            }
        }
    }

    for (auto &piece_object: game_pieces_objects_) {
        if (piece_exists_(piece_object.first)) continue;

        auto it = game_pieces_objects_.find(piece_object.first);

        glm::vec3 end_position = piece_object.second->get_position() + glm::vec3{0, -.16f, 0};
        const glm::vec3 start_scale = piece_object.second->scale_;
        const glm::vec3 start_rotation = piece_object.second->rotation_;

        animation_handler_.add_position_animation(piece_object.second->get_position(), end_position,
                                                  .28f, [this, piece_object, it, start_scale,
                                                         start_rotation](
                        glm::vec3 position, float alpha) {
                    piece_object.second->scale_ = glm::mix(start_scale, start_scale * .82f, alpha);
                    piece_object.second->rotation_ = start_rotation + glm::vec3{10.f * alpha, 0.f, 0.f};
                    piece_object.second->move_to(position);

                    if (alpha >= 1) {
                        main_scene_->remove_object(piece_object.second);

                        game_pieces_objects_.erase(it);
                    }
                });
    }
}

void Renderer_3D::show_flash_message_(Piece::piece_coordinates coordinates) {
    flash_message_.start_time = (float) glfwGetTime();
    flash_message_.show = true;
    flash_message_.position = coordinates;
}

void Renderer_3D::handle_move_(std::string &from, std::string &to) {
    Player *current_player = game_->get_current_player();

    Piece::piece_coordinates from_coordinates = Piece::get_piece_coordinates_from_id(from);
    Piece::piece_coordinates to_coordinates = Piece::get_piece_coordinates_from_id(to);
    const bool is_capture = game_->get_board()
                                    .get_piece_at(to_coordinates.line, to_coordinates.column)
                                    ->get_player_id() != -1;

    if (from == to)
        return;

    try {
        game_->make_move(from, to);

        prepare_move_feedback(to, is_capture);
        check_for_board_changes();
        guides_.clear();
        render_guides_();

        main_scene_->set_selected_index(-1);
        main_scene_->get_camera()->set_selection_focus(false);
        main_scene_->set_hover_index(-1);
        set_interaction_cell_("selected_cell", {-1, -1});
        set_interaction_cell_("hover_cell", {-1, -1});

        if (on_move_callback_) {
            on_move_callback_(from, to);
        }
    } catch (std::exception &error) {
        if (error.what() == Errors::ILLEGAL_MOVE) {
            sound_manager_.play_sound("illegal");
            show_flash_message_(from_coordinates);
        }

        if (error.what() == Errors::KING_IS_NOT_SAFE) {
            sound_manager_.play_sound("illegal");
            Piece::piece_coordinates king_coordinates = game_->get_board().find_king(
                    *current_player);

            show_flash_message_(king_coordinates);
        }
    }
}

void Renderer_3D::render_imgui_() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    render_move_guide_overlay_();
    render_board_coordinates_();

    ImGui::SetNextWindowPos(ImVec2(18.f, 18.f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(.58f);
    const ImGuiWindowFlags utility_flags = ImGuiWindowFlags_NoDecoration |
                                           ImGuiWindowFlags_AlwaysAutoResize |
                                           ImGuiWindowFlags_NoSavedSettings |
                                           ImGuiWindowFlags_NoFocusOnAppearing |
                                           ImGuiWindowFlags_NoNav;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.f, 5.f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(7.f, 3.f));
    ImGui::Begin("Help##utility", nullptr, utility_flags);
    if (ImGui::SmallButton("?##help")) {
        onboarding_dismissed_ = false;
        onboarding_start_time_ = static_cast<float>(glfwGetTime());
    }
    ImGui::End();
    ImGui::PopStyleVar(2);

    const float onboarding_elapsed =
            static_cast<float>(glfwGetTime()) - onboarding_start_time_;
    if (!onboarding_dismissed_ && onboarding_elapsed < 4.f) {
        const float fade = std::clamp((4.f - onboarding_elapsed) / .8f, 0.f, 1.f);
        ImGui::SetNextWindowPos(ImVec2(18.f, 61.f), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(.78f * fade);
        const ImGuiWindowFlags onboarding_flags = ImGuiWindowFlags_NoDecoration |
                                                  ImGuiWindowFlags_AlwaysAutoResize |
                                                  ImGuiWindowFlags_NoSavedSettings |
                                                  ImGuiWindowFlags_NoFocusOnAppearing |
                                                  ImGuiWindowFlags_NoNav |
                                                  ImGuiWindowFlags_NoInputs;
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, fade);
        ImGui::Begin("Quick controls##onboarding", nullptr, onboarding_flags);
        ImGui::TextColored(ImVec4(.72f, .88f, .83f, 1.f), "YOUR MOVE");
        ImGui::Text("Click a piece to begin");
        ImGui::TextDisabled("Right-drag: orbit  ·  Wheel: zoom");
        ImGui::TextDisabled("R: reset camera  ·  F1: visual settings");
        ImGui::End();
        ImGui::PopStyleVar();
    }

    if (show_settings_) {
        ImGui::SetNextWindowPos(ImVec2(18.f, 61.f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(340.f, 0.f), ImGuiCond_FirstUseEver);
        ImGui::Begin("Visual settings", &show_settings_, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Checkbox("Enable Skybox", &main_scene_->sky_box_enabled);
        ImGui::Checkbox("Enable Reflections", &main_scene_->reflection_enabled);
        ImGui::Checkbox("Enable Directional Lighting", &main_scene_->enabled_directional_lighting);
        ImGui::Checkbox("Enable Point Light #1", &main_scene_->enabled_points_lights[0]);
        ImGui::Checkbox("Enable Point Light #2", &main_scene_->enabled_points_lights[1]);
        ImGui::Checkbox("Enable Point Light #3", &main_scene_->enabled_points_lights[2]);
        ImGui::Checkbox("Enable Point Light #4", &main_scene_->enabled_points_lights[3]);
        ImGui::Checkbox("Selection debug view", &selection_rendering_);
        ImGui::SliderFloat("Environment rotation", &environment_yaw_degrees_,
                           -180.f, 180.f, "%.0f deg");

        ImGui::Separator();
        if (ImGui::Button("Reset camera")) {
            reset_camera_();
        }
        ImGui::TextDisabled("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::TextDisabled("GPU: %s", window_.get_gpu_name().c_str());
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Renderer_3D::project_world_to_screen_(const glm::vec3 &world_position,
                                           ImVec2 &screen_position) const {
    if (main_scene_ == nullptr) return false;

    const Camera *camera = main_scene_->get_camera();
    const glm::vec4 clip = camera->get_projection_matrix() *
                           camera->get_view_matrix() * glm::vec4(world_position, 1.f);
    if (clip.w <= .001f) return false;

    const glm::vec3 ndc = glm::vec3(clip) / clip.w;
    if (ndc.z < -1.f || ndc.z > 1.f) return false;
    screen_position.x = (ndc.x * .5f + .5f) * static_cast<float>(window_.get_width());
    screen_position.y = (1.f - (ndc.y * .5f + .5f)) *
                        static_cast<float>(window_.get_height());
    return screen_position.x >= -20.f &&
           screen_position.x <= static_cast<float>(window_.get_width()) + 20.f &&
           screen_position.y >= -20.f &&
           screen_position.y <= static_cast<float>(window_.get_height()) + 20.f;
}

void Renderer_3D::render_board_coordinates_() {
    ImDrawList *draw_list = ImGui::GetBackgroundDrawList();
    ImFont *font = ImGui::GetFont();
    constexpr float font_size = 13.f;
    constexpr float step = .753176f;
    constexpr float first = -2.635f;
    constexpr float frame_edge = 3.22f;
    const glm::vec3 camera_position = main_scene_->get_camera()->get_position();
    const float letter_z = camera_position.z >= 0.f ? frame_edge : -frame_edge;
    const float rank_x = camera_position.x >= 0.f ? frame_edge : -frame_edge;
    const ImU32 shadow = IM_COL32(10, 7, 5, 185);
    const ImU32 ivory = IM_COL32(229, 204, 163, 235);

    auto draw_label = [&](const glm::vec3 &world, const char *label) {
        ImVec2 position;
        if (!project_world_to_screen_(world, position)) return;
        const ImVec2 size = font->CalcTextSizeA(font_size, FLT_MAX, 0.f, label);
        position.x -= size.x * .5f;
        position.y -= size.y * .5f;
        draw_list->AddText(font, font_size, ImVec2(position.x + 1.f, position.y + 1.f),
                           shadow, label);
        draw_list->AddText(font, font_size, position, ivory, label);
    };

    for (int column = 0; column < 8; ++column) {
        const char label[2]{static_cast<char>('A' + column), '\0'};
        draw_label({first + step * static_cast<float>(column), .075f, letter_z}, label);
    }
    for (int row = 0; row < 8; ++row) {
        const char label[2]{static_cast<char>('1' + row), '\0'};
        draw_label({rank_x, .075f, -first - step * static_cast<float>(row)}, label);
    }
}

void Renderer_3D::render_move_guide_overlay_() {
    if (main_scene_->get_selected_index() == -1 || guides_.empty()) return;

    ImDrawList *draw_list = ImGui::GetBackgroundDrawList();
    constexpr float step = .753176f;
    constexpr float first = -2.63f;
    const Piece::piece_coordinates selected_coordinates =
            get_object_coordinates_(main_scene_->get_selected_object());
    for (const std::string &guide : guides_) {
        const Piece::piece_coordinates coordinates =
                Piece::get_piece_coordinates_from_id(guide);
        const Piece *target = game_->get_board().get_piece_at(
                coordinates.line, coordinates.column);
        if (target->get_player_id() != -1) continue;

        glm::vec3 world_center{
                first + step * static_cast<float>(coordinates.column),
                .025f,
                -first - step * static_cast<float>(coordinates.line)};
        const int line_delta = coordinates.line - selected_coordinates.line;
        const int column_delta = coordinates.column - selected_coordinates.column;
        if (std::abs(line_delta) + std::abs(column_delta) == 1) {
            // The center of the immediately adjacent square can project onto
            // the selected piece at a cinematic camera pitch. Keep the marker
            // within its target square while biasing it toward the open half.
            world_center.x += step * static_cast<float>(column_delta) * .34f;
            world_center.z -= step * static_cast<float>(line_delta) * .34f;
        }
        ImVec2 center;
        ImVec2 radius_point;
        if (!project_world_to_screen_(world_center, center) ||
            !project_world_to_screen_(world_center + glm::vec3(step * .115f, 0.f, 0.f),
                                      radius_point)) {
            continue;
        }

        const float radius = std::clamp(
                std::hypot(radius_point.x - center.x, radius_point.y - center.y),
                5.f, 9.f);
        draw_list->AddCircleFilled(center, radius + 3.f,
                                   IM_COL32(46, 220, 184, 24), 24);
        draw_list->AddCircleFilled(center, radius,
                                   IM_COL32(119, 228, 207, 205), 24);
        draw_list->AddCircle(center, radius, IM_COL32(190, 250, 237, 175),
                             24, 1.f);
    }
}

void Renderer_3D::render_flash_message_() {
    if (!flash_message_.show) {
        shaders_.at("main_shader")->set_uniform_1_i("flash_message.i", -1);
        shaders_.at("main_shader")->set_uniform_1_i("flash_message.j", -1);

        return;
    }

    float current_time = glfwGetTime();
    float duration = current_time - flash_message_.start_time;
    float flash_duration = flash_message_.duration / flash_message_.flashes_per_second;

    if (duration >= flash_message_.duration) {
        flash_message_.show = false;

        return;
    }

    if ((int) (duration / flash_duration) % 2 != 0) {
        shaders_.at("main_shader")->set_uniform_1_i("flash_message.i", -1);
        shaders_.at("main_shader")->set_uniform_1_i("flash_message.j", -1);

        return;
    }

    shaders_.at("main_shader")->set_uniform_1_i("flash_message.i", flash_message_.position.line);
    shaders_.at("main_shader")->set_uniform_1_i("flash_message.j", flash_message_.position.column);
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

void Renderer_3D::render_guides_() {
    if (main_scene_->get_selected_index() == -1) {
        shaders_.at("main_shader")->set_uniform_1_i("guides_count", 0);

        return;
    }

    shaders_.at("main_shader")->set_uniform_1_i("guides_count", (int) guides_.size());

    for (int i = 0; i < (int) guides_.size(); ++i) {
        Piece::piece_coordinates coordinates = Piece::get_piece_coordinates_from_id(guides_[i]);

        shaders_.at("main_shader")->set_uniform_1_i("guides[" + std::to_string(i) + "].i",
                                                    coordinates.line);

        shaders_.at("main_shader")->set_uniform_1_i("guides[" + std::to_string(i) + "].j",
                                                    coordinates.column);
        const Piece *target = game_->get_board().get_piece_at(
                coordinates.line, coordinates.column);
        shaders_.at("main_shader")->set_uniform_1_i(
                "guide_capture[" + std::to_string(i) + "]",
                target->get_player_id() == -1 ? 0 : 1);
    }
}

void Renderer_3D::render_landing_feedback_() {
    Shader *shader = shaders_.at("main_shader");
    if (!landing_feedback_.active) {
        set_interaction_cell_("landing_cell", {-1, -1});
        shader->set_uniform_1_f("landing_progress", -1.f);
        return;
    }

    const float elapsed = static_cast<float>(glfwGetTime()) - landing_feedback_.start_time;
    const float progress = std::clamp(elapsed / landing_feedback_.duration, 0.f, 1.f);
    if (progress >= 1.f) {
        landing_feedback_.active = false;
        set_interaction_cell_("landing_cell", {-1, -1});
        shader->set_uniform_1_f("landing_progress", -1.f);
        return;
    }

    set_interaction_cell_("landing_cell", landing_feedback_.cell);
    shader->set_uniform_1_f("landing_progress", progress);
}

void Renderer_3D::set_interaction_cell_(const std::string &uniform_name,
                                        Piece::piece_coordinates coordinates) {
    Shader *shader = shaders_.at("main_shader");
    shader->set_uniform_1_i(uniform_name + ".i", coordinates.line);
    shader->set_uniform_1_i(uniform_name + ".j", coordinates.column);
}

void Renderer_3D::reset_camera_() {
    main_scene_->get_camera()->set_orbit(default_camera_positions_[0], {0.f, .30f, 0.f});
}

void Renderer_3D::scroll_callback_(GLFWwindow *window, double offset_x, double offset_y) {
    ImGui_ImplGlfw_ScrollCallback(window, offset_x, offset_y);

    auto *renderer = static_cast<Renderer_3D *>(glfwGetWindowUserPointer(window));
    if (renderer == nullptr || renderer->main_scene_ == nullptr ||
        ImGui::GetIO().WantCaptureMouse) {
        return;
    }

    renderer->onboarding_dismissed_ = true;
    renderer->main_scene_->get_camera()->zoom(offset_y);
}

void Renderer_3D::play_move_sound(bool is_capture) {
    sound_manager_.play_sound(is_capture ? "capture" : "move");
}

void Renderer_3D::prepare_move_feedback(const std::string &destination, bool is_capture) {
    pending_move_destination_ = Piece::get_piece_coordinates_from_id(destination);
    pending_move_is_capture_ = is_capture;
    pending_move_feedback_ = true;
}

void Renderer_3D::render() {
    glfwPollEvents();
    is_running_ = !glfwWindowShouldClose(window_.get_window());
    if (!is_running_) {
        return;
    }

    update_dt_fps_();
    const bool shadows_were_animating = animation_handler_.is_animating();
    animation_handler_.update(dt_);
    handle_inputs_();
    main_scene_->get_camera()->update(dt_);

    // The directional light and salon are static. Rebuild this 3072px map only
    // when a piece moves; camera orbit does not change light-space shadows.
    if (shadow_map_dirty_ || shadows_were_animating || animation_handler_.is_animating()) {
        render_shadow_map_();
        shadow_map_dirty_ = false;
    }
    bind_shadow_map_();
    shaders_.at("main_shader")->set_uniform_1_f("environment_yaw_degrees",
                                                 environment_yaw_degrees_);

    glBindFramebuffer(GL_FRAMEBUFFER, hdr_framebuffer_);
    const GLenum color_attachments[]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, color_attachments);
    glViewport(0, 0, window_.get_width(), window_.get_height());
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    render_flash_message_();
    render_last_move_();
    render_landing_feedback_();

    if (selection_rendering_)
        main_scene_->render_for_selection();
    else {
        main_scene_->render();
        render_selected_outline_();
        render_late_board_feedback_();
    }

    resolve_hdr_framebuffer_();
    render_bloom_();
    composite_hdr_();

    render_imgui_();

    glfwSwapBuffers(window_.get_window());

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(GL_TEXTURE0);

    window_.set_title("Chess 3D - Player " +
                      std::to_string(game_->get_current_player()->player_id + 1) + " to move");
}

Renderer_3D::~Renderer_3D() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (shadow_depth_texture_ != 0) {
        glDeleteTextures(1, &shadow_depth_texture_);
    }
    if (shadow_framebuffer_ != 0) {
        glDeleteFramebuffers(1, &shadow_framebuffer_);
    }
    destroy_post_processing_();

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

void Renderer_3D::on_window_move(window_move_callback callback) {
    on_window_move_callback_ = callback;
}

void Renderer_3D::on_move(move_callback callback) {
    on_move_callback_ = callback;
}
