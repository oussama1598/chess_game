#include "game/Game.h"
#include "renderers/2d/renderer_2D.h"
#include "renderers/3d/renderer_3D.h"

#include <cmath>

int main() {
    Camera camera({0.f, 6.2f, 7.3f}, {1000.f, 800.f});
    const glm::vec3 initial_position = camera.get_position();
    const float initial_distance = glm::length(initial_position - glm::vec3(0.f, .30f, 0.f));
    camera.orbit(120.0, -25.0);
    const glm::vec3 orbited_position = camera.get_position();
    const float orbited_distance = glm::length(orbited_position - glm::vec3(0.f, .30f, 0.f));
    if (glm::distance(initial_position, orbited_position) < .1f ||
        std::abs(initial_distance - orbited_distance) > .001f) {
        return 1;
    }

    camera.zoom(2.0);
    const float zoomed_distance = glm::length(
            camera.get_position() - glm::vec3(0.f, .30f, 0.f));
    if (zoomed_distance >= orbited_distance) {
        return 1;
    }

    Animation_Handler animation_handler;
    glm::vec3 animated_position{0.f};
    float animated_progress = 0.f;
    animation_handler.add_position_animation(
            {0.f, 0.f, 0.f}, {2.f, 0.f, 0.f}, 1.f,
            [&animated_position, &animated_progress](glm::vec3 position, float progress) {
                animated_position = position;
                animated_progress = progress;
            });
    animation_handler.update(.5f);
    if (std::abs(animated_position.x - 1.f) > .001f ||
        std::abs(animated_progress - .5f) > .001f || !animation_handler.is_animating()) {
        return 1;
    }
    animation_handler.update(.5f);
    if (std::abs(animated_position.x - 2.f) > .001f || animation_handler.is_animating()) {
        return 1;
    }

    Game game;

    {
        Renderer_2D renderer(&game);
        renderer.on_window_move([](int, int) {});
        renderer.on_move([](const std::string &, const std::string &) {});
        renderer.play_move_sound(false);
        renderer.play_move_sound(true);
        renderer.render();
        // Exercise the steady-state path, where the static light-space shadow map
        // is reused instead of rebuilt.
        renderer.render();
    }

    {
        Renderer_3D renderer(&game, false);
        renderer.on_window_move([](int, int) {});
        renderer.on_move([](const std::string &, const std::string &) {});
        renderer.play_move_sound(false);
        renderer.play_move_sound(true);
        renderer.render();
    }

    return 0;
}
