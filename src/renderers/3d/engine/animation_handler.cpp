#include "animation_handler.h"

void
Animation_Handler::add_animation(float threshold, float step,
                                 std::function<void(float)> animation_callback) {
    animations_.push_back(
            {
                    threshold,
                    step,
                    0,
                    animation_callback
            }
    );
}


void Animation_Handler::add_position_animation(glm::vec3 start_position, glm::vec3 end_position,
                                               float duration,
                                               std::function<void(glm::vec3, float)> animation_callback) {
    position_animations_.push_back(
            {
                    start_position,
                    end_position,
                    0,
                    duration,
                    animation_callback
            }
    );
}

void Animation_Handler::update(float dt) {
    for (int i = (int) position_animations_.size() - 1; i >= 0; --i) {
        Position_Animation &animation = position_animations_[i];

        animation.current_time += dt;

        float progress = animation.current_time / animation.duration;
        glm::vec3 position = glm::mix(animation.start_position, animation.end_position, progress);

        if (animation.current_time >= animation.duration) {
            animation.animation_callback(animation.end_position, progress);

            position_animations_.erase(position_animations_.begin() + i);

            continue;
        }

        animation.animation_callback(position, progress);
    }

    for (int i = (int) animations_.size() - 1; i >= 0; --i) {
        Animation &animation = animations_[i];

        animation.current_value += animation.step;

        if (animation.current_value >= animation.threshold) {
            animation.animation_callback(1);

            animations_.erase(animations_.begin());

            continue;
        }

        animation.animation_callback(animation.step);
        return;
    }
}
