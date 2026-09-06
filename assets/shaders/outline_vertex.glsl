#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 3) in vec3 vertex_normal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform float outline_width;

void main()
{
    vec3 expanded_position = vertex_position + normalize(vertex_normal) * outline_width;
    gl_Position = projection_matrix * view_matrix * model_matrix *
                  vec4(expanded_position, 1.0);
}
