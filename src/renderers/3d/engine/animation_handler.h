#pragma once

#include <functional>

class Animation_Handler {
private:
    struct Animation {
        float threshold;
        float step;
        float current_value;
        std::function<void(float)> animation_callback;
    };

    std::vector<Animation> animations_;

public:
    void add_animation(float threshold, float step, std::function<void(float)> animation_callback);

    void update();
};
