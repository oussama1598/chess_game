#include "scene.h"

void Scene::add_camera(Camera *camera) {
    cameras_.push_back(camera);

    selected_camera_index_ = (int) cameras_.size() - 1;
}

void Scene::add_light(Light *light) {
    lights_.push_back(light);

    bool is_point_light = dynamic_cast<const Point_Light *>(light) != nullptr;

    if (is_point_light) {
        light->light_index = points_lights_count;

        points_lights_count += 1;
    }
}

void Scene::add_object(Object *object) {
    objects_.push_back(object);
}

void Scene::remove_object(Object *object) {
    auto element = std::find(objects_.begin(), objects_.end(), object);

    if (element == objects_.cend()) return;

    int index = std::distance(objects_.begin(), element);

    objects_.erase(objects_.cbegin() + index);
}

void Scene::render() {
    for (size_t i = 0; i < objects_.size(); ++i) {
        Object *object = objects_[i];
        Shader *shader = object->get_shader();

        for (auto &light: lights_) {
            light->attach_to_shader(shader);
        }

        get_camera()->attach_to_shader(shader);

        if (sky_box_enabled) {
            sky_box_->attach_to_shader(shader);
        }

        shader->set_uniform_1_i("reflection_enabled", (int) reflection_enabled);
        shader->set_uniform_1_i("enabled_directional_lighting",
                                (int) enabled_directional_lighting);

        for (int j = 0; j < 4; ++j) {
            shader->set_uniform_1_i(
                    "enabled_points_lights[" + std::to_string(i) + "]", enabled_points_lights[i]);
        }

        Material *objects_material = object->get_material();

        if (selected_index_ == (int) i)
            object->set_material(selection_material_);

        if (hovered_index_ == (int) i)
            object->set_material(hover_material_);

        object->draw();

        // we may have changed it cause of the selection
        object->set_material(objects_material);
    }

    if (sky_box_enabled) {
        get_camera()->attach_to_shader(sky_box_->get_shader());

        sky_box_->draw();
    }
}

void Scene::render_for_selection() {
    for (size_t i = 0; i < objects_.size(); ++i) {
        Object *object = objects_[i];

        selection_shader_->set_uniform_1_f("object_index", i);

        get_camera()->attach_to_shader(selection_shader_);

        Shader *objects_shader = object->get_shader();

        object->set_shader(selection_shader_);
        object->draw();
        object->set_shader(objects_shader);
    }
}

Object *Scene::get_selected_object() {
    if (selected_index_ == -1)
        return nullptr;

    return objects_.at(selected_index_);
}

Object *Scene::get_hovered_object() {
    if (hovered_index_ == -1)
        return nullptr;

    return objects_.at(hovered_index_);
}

Object *Scene::get_object(int index) {
    if (index == -1)
        return nullptr;

    return objects_.at(index);
}

void Scene::use_camera(int camera_index) {
    if (camera_index > (int) cameras_.size())
        return;

    selected_camera_index_ = camera_index;
}

Scene::~Scene() {
    for (Camera *camera: cameras_) {
        delete camera;
    }

    for (Light *light: lights_) {
        delete light;
    }

    for (Object *object: objects_) {
        delete object;
    }

    delete sky_box_;
}
