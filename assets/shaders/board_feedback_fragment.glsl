#version 440

layout (location = 0) out vec4 SceneColor;
layout (location = 1) out vec4 EmissiveColor;

in vec2 vs_text_coord;

struct Cell {
    int i;
    int j;
};

uniform Cell guides[32];
uniform int guide_capture[32];
uniform int guides_count;
uniform vec3 guides_color;

void main()
{
    int column = clamp(int((vs_text_coord.x - 0.7034) / 0.036), 0, 7);
    int row = clamp(int((-vs_text_coord.y - 0.70915) / 0.0353), 0, 7);

    bool empty_guide = false;
    for (int index = 0; index < guides_count; ++index) {
        if (guide_capture[index] == 0 && guides[index].i == row &&
            guides[index].j == column) {
            empty_guide = true;
        }
    }
    if (!empty_guide) discard;

    vec2 cell_position = vec2(
            fract((vs_text_coord.x - 0.7034) / 0.036),
            fract((-vs_text_coord.y - 0.70915) / 0.0353));
    float radius = length(cell_position - vec2(0.5));
    float radial_aa = max(fwidth(radius) * 1.35, 0.002);
    float disc = 1.0 - smoothstep(0.115, 0.115 + radial_aa, radius);
    if (disc <= 0.0) discard;

    SceneColor = vec4(guides_color * 0.72, disc * 0.72);
    EmissiveColor = vec4(guides_color * 0.80, disc * 0.62);
}
