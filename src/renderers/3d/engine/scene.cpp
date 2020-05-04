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

void Scene::set_sky_box(SkyBox *sky_box) {
    sky_box_ = sky_box;
}

void Scene::render() {
    for (Object *object: objects_) {
        for (auto &light: lights_) {
            light->attach_to_shader(object->get_shader());
        }

        cameras_[0]->attach_to_shader(object->get_shader());

        sky_box_->attach_to_shader(object->get_shader());

        object->draw();
    }

    cameras_[0]->attach_to_shader(sky_box_->get_shader());
    sky_box_->draw();
}
