#include "animation_handler.h"

#include <algorithm>

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

        const float progress = std::clamp(animation.current_time / animation.duration, 0.f, 1.f);
        const float eased_progress = progress * progress * (3.f - 2.f * progress);
        const glm::vec3 position = glm::mix(animation.start_position, animation.end_position,
                                            eased_progress);

        animation.animation_callback(position, eased_progress);

        if (progress >= 1.f) {

            position_animations_.erase(position_animations_.begin() + i);

            continue;
        }
    }

    for (int i = (int) animations_.size() - 1; i >= 0; --i) {
        Animation &animation = animations_[i];

        animation.current_value += animation.step;

        if (animation.current_value >= animation.threshold) {
            animation.animation_callback(1);

            animations_.erase(animations_.begin() + i);

            continue;
        }

        animation.animation_callback(animation.step);
    }
}
