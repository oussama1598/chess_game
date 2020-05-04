#pragma once

#include "object.h"
#include "light.h"
#include "camera.h"

class Scene {
private:
    std::vector<Camera *> cameras_;
    std::vector<Light *> lights_;
    std::vector<Object *> objects_;

    Camera *selected_camera_;

public:

    inline Camera *get_camera() const { return selected_camera_; }

    inline Light *get_light(int index) { return lights_.at(index); }

    void add_camera(Camera *camera);

    void add_light(Light *light);

    void add_object(Object *object);

//    void use_camera(int camera_index);

    void render();
};
