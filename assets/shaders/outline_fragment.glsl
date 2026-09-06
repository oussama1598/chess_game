#version 440

layout (location = 0) out vec4 SceneColor;
layout (location = 1) out vec4 EmissiveColor;

uniform vec3 outline_color;

void main()
{
    // A solid scene-color core keeps the silhouette continuous; the restrained
    // emissive component supplies only the soft outer finish in post.
    SceneColor = vec4(outline_color * 0.36, 0.96);
    EmissiveColor = vec4(outline_color * 0.82, 1.0);
}
