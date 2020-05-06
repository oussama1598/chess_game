#include "animation_handler.h"
#include <iostream>

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

void Animation_Handler::update() {
    for (int i = (int) animations_.size() - 1; i >= 0; --i) {
        Animation &animation = animations_[i];

        animation.current_value += animation.step;

        if (animation.current_value >= animation.threshold) {
            animation.animation_callback(1);

            animations_.erase(animations_.begin());

            return;
        }

        animation.animation_callback(animation.step);
    }
}
