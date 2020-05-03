#include "plane.h"

Plane::Plane() : Object({
                                {
                                        glm::vec3(-0.5f, 0.5f, 0.0f),
                                        glm::vec3(1.f, 0.f, 0.f),
                                        glm::vec2(0.f, 1.f),
                                        glm::vec3(0.f, 0.f, 1.f)
                                },
                                {
                                        glm::vec3(-0.5f, -0.5f, 0.0f),
                                        glm::vec3(0.f, 1.f, 0.f),
                                        glm::vec2(0.f, 0.f),
                                        glm::vec3(0.f, 0.f, 1.f)
                                },

                                {
                                        glm::vec3(0.5f, -0.5f, 0.0f),
                                        glm::vec3(0.f, 0.f, 1.f),
                                        glm::vec2(1.f, 0.f),
                                        glm::vec3(0.f, 0.f, 1.f)
                                },
                                {
                                        glm::vec3(0.5f, 0.5f, 0.0f),
                                        glm::vec3(1.f, 1.f, 0.f),
                                        glm::vec2(1.f, 1.f),
                                        glm::vec3(0.f, 0.f, 1.f)
                                }
                        }, {
                                0, 1, 2,
                                0, 2, 3
                        }) {
}
