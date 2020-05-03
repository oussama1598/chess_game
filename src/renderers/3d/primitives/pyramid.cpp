#include "pyramid.h"

Pyramid::Pyramid() : Object(
        {
                //Position								//Color							//Texcoords					//Normals
                //Triangle front
                {
                        glm::vec3(0.f, 0.5f, 0.f),
                        glm::vec3(1.f, 0.f, 0.f),
                        glm::vec2(0.5f, 1.f),
                        glm::vec3(0.f, 0.f, 1.f)
                },
                {
                        glm::vec3(-0.5f, -0.5f, 0.5f),
                        glm::vec3(0.f, 1.f, 0.f),
                        glm::vec2(0.f, 0.f),
                        glm::vec3(0.f, 0.f, 1.f)
                },
                {
                        glm::vec3(0.5f, -0.5f, 0.5f),
                        glm::vec3(0.f, 0.f, 1.f),
                        glm::vec2(1.f, 0.f),
                        glm::vec3(0.f, 0.f, 1.f)
                },

                //Triangle left
                {
                        glm::vec3(0.f, 0.5f, 0.f),
                        glm::vec3(1.f, 1.f, 0.f),
                        glm::vec2(0.5f, 1.f),
                        glm::vec3(-1.f, 0.f, 0.f)
                },
                {
                        glm::vec3(-0.5f, -0.5f, -0.5f),
                        glm::vec3(0.f, 0.f, 1.f),
                        glm::vec2(0.f, 0.f),
                        glm::vec3(-1.f, 0.f, 0.f)
                },
                {
                        glm::vec3(-0.5f, -0.5f, 0.5f),
                        glm::vec3(0.f, 0.f, 1.f),
                        glm::vec2(1.f, 0.f),
                        glm::vec3(-1.f, 0.f, 0.f)
                },

                //Triangle back
                {
                        glm::vec3(0.f, 0.5f, 0.f),
                        glm::vec3(1.f, 1.f, 0.f),
                        glm::vec2(0.5f, 1.f),
                        glm::vec3(0.f, 0.f, -1.f)
                },
                {
                        glm::vec3(0.5f, -0.5f, -0.5f),
                        glm::vec3(0.f, 0.f, 1.f),
                        glm::vec2(0.f, 0.f),
                        glm::vec3(0.f, 0.f, -1.f)
                },
                {
                        glm::vec3(-0.5f, -0.5f, -0.5f),
                        glm::vec3(0.f, 0.f, 1.f),
                        glm::vec2(1.f, 0.f),
                        glm::vec3(0.f, 0.f, -1.f)
                },

                //Triangles right
                {
                        glm::vec3(0.f, 0.5f, 0.f),
                        glm::vec3(1.f, 1.f, 0.f),
                        glm::vec2(0.5f, 1.f),
                        glm::vec3(1.f, 0.f, 0.f)
                },
                {
                        glm::vec3(0.5f, -0.5f, 0.5f),
                        glm::vec3(0.f, 0.f, 1.f),
                        glm::vec2(0.f, 0.f),
                        glm::vec3(1.f, 0.f, 0.f)
                },
                {
                        glm::vec3(0.5f, -0.5f, -0.5f),
                        glm::vec3(0.f, 0.f, 1.f),
                        glm::vec2(1.f, 0.f),
                        glm::vec3(1.f, 0.f, 0.f)
                },
        },
        {}
) {}
