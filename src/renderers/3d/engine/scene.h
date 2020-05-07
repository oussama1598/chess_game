#pragma once

#include "object.h"
#include "light.h"
#include "point_light.h"
#include "camera.h"
#include "sky_box.h"

class Scene {
private:
    std::vector<Camera *> cameras_;
    std::vector<Light *> lights_;
    std::vector<Object *> objects_;

    int selected_camera_index_;

    SkyBox *sky_box_;

    Shader *selection_shader_;
    Material *selection_material_;
    int selected_index_{-1};

    Material *hover_material_;
    int hovered_index_{-1};

    int points_lights_count{0};

public:
    // options
    bool sky_box_enabled{true};
    bool reflection_enabled{true};
    bool enabled_directional_lighting{true};
    bool enabled_points_lights[4]{true};

public:

    ~Scene();

    inline Camera *get_camera() const { return cameras_[selected_camera_index_]; }

    inline Light *get_light(int index) { return lights_.at(index); }

    void add_camera(Camera *camera);

    void use_camera(int camera_index);

    int get_selected_camera_index() { return selected_camera_index_; }

    void add_light(Light *light);

    void add_object(Object *object);

    void remove_object(Object *object);

    inline void set_sky_box(SkyBox *sky_box) { sky_box_ = sky_box; };

    inline void set_selection_shader(Shader *shader) { selection_shader_ = shader; }

    inline void set_selection_material(Material *material) { selection_material_ = material; }

    inline void set_hover_material(Material *material) { hover_material_ = material; }

    inline void set_selected_index(int index) { selected_index_ = index; }

    inline int get_selected_index() { return selected_index_; }

    Object *get_selected_object();

    inline void set_hover_index(int index) { hovered_index_ = index; }

    inline int get_hover_index() const { return hovered_index_; }

    Object *get_hovered_object();

    Object *get_object(int index);

    void render();

    void render_for_selection();
};
