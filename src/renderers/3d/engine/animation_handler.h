#pragma once

#include <functional>
#include <glm/glm.hpp>

class Animation_Handler {
private:
    struct Animation {
        float threshold;
        float step;
        float current_value;
        std::function<void(float)> animation_callback;
    };

    struct Position_Animation {
        glm::vec3 start_position;
        glm::vec3 end_position;
        float current_time;
        float duration;
        std::function<void(glm::vec3, float)> animation_callback;
    };

    std::vector<Animation> animations_;
    std::vector<Position_Animation> position_animations_;

public:
    inline bool is_animating() { return position_animations_.size() > 0; }

    void add_animation(float threshold, float step, std::function<void(float)> animation_callback);

    void add_position_animation(glm::vec3 start_position, glm::vec3 end_position, float duration,
                                std::function<void(glm::vec3, float)> animation_callback);

    void update(float dt);
};
