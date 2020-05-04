#include "scene.h"

void Scene::add_camera(Camera *camera) {
    cameras_.push_back(camera);

    selected_camera_ = camera;
}

void Scene::add_light(Light *light) {
    lights_.push_back(light);
}

void Scene::add_object(Object *object) {
    objects_.push_back(object);
}

void Scene::render() {
    for (Object *object: objects_) {
        for (auto &light: lights_) {
            light->attach_to_shader(object->get_shader());
        }

        cameras_[0]->attach_to_shader(object->get_shader());

        object->draw();
    }
}
