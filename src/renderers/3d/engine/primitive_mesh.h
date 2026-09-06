#pragma once

#include <array>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include "vertex.h"

namespace Procedural_Mesh {

inline Vertex vertex(glm::vec3 position, glm::vec2 texture, glm::vec3 normal) {
    return {position, glm::vec3{1.0f}, texture, normal};
}

inline void add_quad(std::vector<Vertex> &vertices,
                     const glm::vec3 &a, const glm::vec3 &b,
                     const glm::vec3 &c, const glm::vec3 &d,
                     const glm::vec3 &normal,
                     const glm::vec2 &uv_a = {0.0f, 0.0f},
                     const glm::vec2 &uv_b = {1.0f, 0.0f},
                     const glm::vec2 &uv_c = {1.0f, 1.0f},
                     const glm::vec2 &uv_d = {0.0f, 1.0f});

inline void add_square_ring(std::vector<Vertex> &vertices,
                            float outer_radius, float outer_y,
                            float inner_radius, float inner_y,
                            bool bevel_normal);

inline std::vector<Vertex> make_table(float half_extent = 3.02f) {
    constexpr float u0 = 0.7034f;
    constexpr float u1 = u0 + 8.0f * 0.036f;
    constexpr float v0 = 0.70915f;
    constexpr float v1 = v0 + 8.0f * 0.0353f;
    const glm::vec3 normal{0.0f, 1.0f, 0.0f};

    const Vertex top_left = vertex({-half_extent, 0.0f, half_extent}, {u0, v0}, normal);
    const Vertex top_right = vertex({half_extent, 0.0f, half_extent}, {u1, v0}, normal);
    const Vertex bottom_right = vertex({half_extent, 0.0f, -half_extent}, {u1, v1}, normal);
    const Vertex bottom_left = vertex({-half_extent, 0.0f, -half_extent}, {u0, v1}, normal);

    return {top_left, bottom_right, top_right,
            top_left, bottom_left, bottom_right};
}

inline void add_box_face(std::vector<Vertex> &vertices,
                         glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d,
                         glm::vec3 normal) {
    vertices.push_back(vertex(a, {0.0f, 0.0f}, normal));
    vertices.push_back(vertex(c, {1.0f, 1.0f}, normal));
    vertices.push_back(vertex(b, {1.0f, 0.0f}, normal));
    vertices.push_back(vertex(a, {0.0f, 0.0f}, normal));
    vertices.push_back(vertex(d, {0.0f, 1.0f}, normal));
    vertices.push_back(vertex(c, {1.0f, 1.0f}, normal));
}

inline std::vector<Vertex> make_box(float half_extent = 1.0f) {
    std::vector<Vertex> vertices;
    vertices.reserve(36);
    const float h = half_extent;

    add_box_face(vertices, {-h, -h, h}, {h, -h, h}, {h, h, h}, {-h, h, h}, {0, 0, 1});
    add_box_face(vertices, {h, -h, -h}, {-h, -h, -h}, {-h, h, -h}, {h, h, -h}, {0, 0, -1});
    add_box_face(vertices, {h, -h, h}, {h, -h, -h}, {h, h, -h}, {h, h, h}, {1, 0, 0});
    add_box_face(vertices, {-h, -h, -h}, {-h, -h, h}, {-h, h, h}, {-h, h, -h}, {-1, 0, 0});
    add_box_face(vertices, {-h, h, h}, {h, h, h}, {h, h, -h}, {-h, h, -h}, {0, 1, 0});
    add_box_face(vertices, {-h, -h, -h}, {h, -h, -h}, {h, -h, h}, {-h, -h, h}, {0, -1, 0});
    return vertices;
}

inline std::vector<Vertex> make_beveled_box(float bevel = 0.055f) {
    std::vector<Vertex> vertices;
    vertices.reserve(60);
    const float inner = 1.f - bevel;
    const float shoulder_y = 1.f - bevel;

    add_box_face(vertices, {-inner, 1.f, inner}, {inner, 1.f, inner},
                 {inner, 1.f, -inner}, {-inner, 1.f, -inner}, {0.f, 1.f, 0.f});
    add_square_ring(vertices, 1.f, shoulder_y, inner, 1.f, true);
    add_box_face(vertices, {-1.f, -1.f, 1.f}, {1.f, -1.f, 1.f},
                 {1.f, shoulder_y, 1.f}, {-1.f, shoulder_y, 1.f}, {0.f, 0.f, 1.f});
    add_box_face(vertices, {1.f, -1.f, -1.f}, {-1.f, -1.f, -1.f},
                 {-1.f, shoulder_y, -1.f}, {1.f, shoulder_y, -1.f}, {0.f, 0.f, -1.f});
    add_box_face(vertices, {1.f, -1.f, 1.f}, {1.f, -1.f, -1.f},
                 {1.f, shoulder_y, -1.f}, {1.f, shoulder_y, 1.f}, {1.f, 0.f, 0.f});
    add_box_face(vertices, {-1.f, -1.f, -1.f}, {-1.f, -1.f, 1.f},
                 {-1.f, shoulder_y, 1.f}, {-1.f, shoulder_y, -1.f}, {-1.f, 0.f, 0.f});
    add_box_face(vertices, {-1.f, -1.f, -1.f}, {1.f, -1.f, -1.f},
                 {1.f, -1.f, 1.f}, {-1.f, -1.f, 1.f}, {0.f, -1.f, 0.f});
    return vertices;
}

inline std::vector<Vertex> make_tapered_prism(float bottom_scale = .72f) {
    std::vector<Vertex> vertices;
    vertices.reserve(36);
    const std::array<glm::vec3, 4> top{{
            {-1.f, 1.f, 1.f}, {1.f, 1.f, 1.f},
            {1.f, 1.f, -1.f}, {-1.f, 1.f, -1.f}}};
    const std::array<glm::vec3, 4> bottom{{
            {-bottom_scale, -1.f, bottom_scale},
            {bottom_scale, -1.f, bottom_scale},
            {bottom_scale, -1.f, -bottom_scale},
            {-bottom_scale, -1.f, -bottom_scale}}};
    for (int side = 0; side < 4; ++side) {
        const int next = (side + 1) % 4;
        const glm::vec3 normal = glm::normalize(glm::cross(
                top[next] - top[side], bottom[side] - top[side]));
        add_quad(vertices, top[side], top[next], bottom[next], bottom[side], normal);
    }
    add_quad(vertices, top[3], top[2], top[1], top[0], {0.f, 1.f, 0.f});
    add_quad(vertices, bottom[0], bottom[1], bottom[2], bottom[3], {0.f, -1.f, 0.f});
    return vertices;
}

inline std::vector<Vertex> make_cylinder_band(float radius, float bottom, float top,
                                               int segments = 64,
                                               float texture_repeats = 8.f) {
    constexpr float pi = 3.14159265358979323846f;
    std::vector<Vertex> vertices;
    vertices.reserve(static_cast<size_t>(segments) * 6);
    for (int segment = 0; segment < segments; ++segment) {
        const float a0 = 2.f * pi * static_cast<float>(segment) /
                         static_cast<float>(segments);
        const float a1 = 2.f * pi * static_cast<float>(segment + 1) /
                         static_cast<float>(segments);
        const glm::vec3 n0{-std::sin(a0), 0.f, -std::cos(a0)};
        const glm::vec3 n1{-std::sin(a1), 0.f, -std::cos(a1)};
        const glm::vec3 p0b{radius * std::sin(a0), bottom, radius * std::cos(a0)};
        const glm::vec3 p1b{radius * std::sin(a1), bottom, radius * std::cos(a1)};
        const glm::vec3 p0t{p0b.x, top, p0b.z};
        const glm::vec3 p1t{p1b.x, top, p1b.z};
        const float u0 = texture_repeats * static_cast<float>(segment) /
                         static_cast<float>(segments);
        const float u1 = texture_repeats * static_cast<float>(segment + 1) /
                         static_cast<float>(segments);
        vertices.push_back(vertex(p0b, {u0, 0.f}, n0));
        vertices.push_back(vertex(p1t, {u1, 1.f}, n1));
        vertices.push_back(vertex(p1b, {u1, 0.f}, n1));
        vertices.push_back(vertex(p0b, {u0, 0.f}, n0));
        vertices.push_back(vertex(p0t, {u0, 1.f}, n0));
        vertices.push_back(vertex(p1t, {u1, 1.f}, n1));
    }
    return vertices;
}

inline std::vector<Vertex> make_cylinder_arc_band(float radius, float bottom, float top,
                                                   float start_degrees, float end_degrees,
                                                   int segments = 24,
                                                   float texture_repeats = 2.f) {
    constexpr float pi = 3.14159265358979323846f;
    const float start = start_degrees * pi / 180.f;
    const float end = end_degrees * pi / 180.f;
    std::vector<Vertex> vertices;
    vertices.reserve(static_cast<size_t>(segments) * 6);
    for (int segment = 0; segment < segments; ++segment) {
        const float t0 = static_cast<float>(segment) / static_cast<float>(segments);
        const float t1 = static_cast<float>(segment + 1) / static_cast<float>(segments);
        const float a0 = start + (end - start) * t0;
        const float a1 = start + (end - start) * t1;
        const glm::vec3 n0{-std::sin(a0), 0.f, std::cos(a0)};
        const glm::vec3 n1{-std::sin(a1), 0.f, std::cos(a1)};
        const glm::vec3 p0b{radius * std::sin(a0), bottom, -radius * std::cos(a0)};
        const glm::vec3 p1b{radius * std::sin(a1), bottom, -radius * std::cos(a1)};
        const glm::vec3 p0t{p0b.x, top, p0b.z};
        const glm::vec3 p1t{p1b.x, top, p1b.z};
        const float u0 = texture_repeats * t0;
        const float u1 = texture_repeats * t1;
        vertices.push_back(vertex(p0b, {u0, 0.f}, n0));
        vertices.push_back(vertex(p1t, {u1, 1.f}, n1));
        vertices.push_back(vertex(p1b, {u1, 0.f}, n1));
        vertices.push_back(vertex(p0b, {u0, 0.f}, n0));
        vertices.push_back(vertex(p0t, {u0, 1.f}, n0));
        vertices.push_back(vertex(p1t, {u1, 1.f}, n1));
    }
    return vertices;
}

inline void add_quad(std::vector<Vertex> &vertices,
                     const glm::vec3 &a, const glm::vec3 &b,
                     const glm::vec3 &c, const glm::vec3 &d,
                     const glm::vec3 &normal,
                     const glm::vec2 &uv_a,
                     const glm::vec2 &uv_b,
                     const glm::vec2 &uv_c,
                     const glm::vec2 &uv_d) {
    vertices.push_back(vertex(a, uv_a, normal));
    vertices.push_back(vertex(c, uv_c, normal));
    vertices.push_back(vertex(b, uv_b, normal));
    vertices.push_back(vertex(a, uv_a, normal));
    vertices.push_back(vertex(d, uv_d, normal));
    vertices.push_back(vertex(c, uv_c, normal));
}

inline std::vector<Vertex> make_board_tile_tops() {
    constexpr float step = 0.753176f;
    constexpr float min_x = -2.63f - step * 0.5f;
    constexpr float max_z = 2.63f + step * 0.5f;
    constexpr float gap = 0.012f;
    constexpr float bevel = 0.017f;
    constexpr float atlas_u = 0.7034f;
    constexpr float atlas_v = 0.70915f;
    constexpr float atlas_u_step = 0.036f;
    constexpr float atlas_v_step = 0.0353f;
    constexpr float uv_padding = 0.00055f;

    std::vector<Vertex> vertices;
    vertices.reserve(64 * 6);
    const glm::vec3 normal{0.0f, 1.0f, 0.0f};

    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            const float outer_x0 = min_x + column * step + gap * 0.5f;
            const float outer_x1 = min_x + (column + 1) * step - gap * 0.5f;
            const float outer_z1 = max_z - row * step - gap * 0.5f;
            const float outer_z0 = max_z - (row + 1) * step + gap * 0.5f;
            const float x0 = outer_x0 + bevel;
            const float x1 = outer_x1 - bevel;
            const float z0 = outer_z0 + bevel;
            const float z1 = outer_z1 - bevel;

            const float u0 = atlas_u + column * atlas_u_step + uv_padding;
            const float u1 = atlas_u + (column + 1) * atlas_u_step - uv_padding;
            const float v0 = atlas_v + row * atlas_v_step + uv_padding;
            const float v1 = atlas_v + (row + 1) * atlas_v_step - uv_padding;

            add_quad(vertices,
                     {x0, 0.0f, z1}, {x1, 0.0f, z1},
                     {x1, 0.0f, z0}, {x0, 0.0f, z0}, normal,
                     {u0, v0}, {u1, v0}, {u1, v1}, {u0, v1});
        }
    }

    return vertices;
}

inline std::vector<Vertex> make_board_tile_edges(bool light_tiles) {
    constexpr float step = 0.753176f;
    constexpr float min_x = -2.63f - step * 0.5f;
    constexpr float max_z = 2.63f + step * 0.5f;
    constexpr float gap = 0.012f;
    constexpr float bevel = 0.017f;
    constexpr float bevel_drop = 0.015f;
    constexpr float side_depth = 0.030f;

    std::vector<Vertex> vertices;
    vertices.reserve(32 * 48);

    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            if (((row + column) % 2 == 0) != light_tiles) continue;

            const float ox0 = min_x + column * step + gap * 0.5f;
            const float ox1 = min_x + (column + 1) * step - gap * 0.5f;
            const float oz1 = max_z - row * step - gap * 0.5f;
            const float oz0 = max_z - (row + 1) * step + gap * 0.5f;
            const float tx0 = ox0 + bevel;
            const float tx1 = ox1 - bevel;
            const float tz0 = oz0 + bevel;
            const float tz1 = oz1 - bevel;

            const glm::vec3 north_bevel = glm::normalize(glm::vec3{0.f, bevel, bevel_drop});
            const glm::vec3 south_bevel = glm::normalize(glm::vec3{0.f, bevel, -bevel_drop});
            const glm::vec3 east_bevel = glm::normalize(glm::vec3{bevel_drop, bevel, 0.f});
            const glm::vec3 west_bevel = glm::normalize(glm::vec3{-bevel_drop, bevel, 0.f});

            add_quad(vertices, {tx0, 0.f, tz1}, {tx1, 0.f, tz1},
                     {ox1, -bevel_drop, oz1}, {ox0, -bevel_drop, oz1}, north_bevel);
            add_quad(vertices, {tx1, 0.f, tz0}, {tx0, 0.f, tz0},
                     {ox0, -bevel_drop, oz0}, {ox1, -bevel_drop, oz0}, south_bevel);
            add_quad(vertices, {tx1, 0.f, tz1}, {tx1, 0.f, tz0},
                     {ox1, -bevel_drop, oz0}, {ox1, -bevel_drop, oz1}, east_bevel);
            add_quad(vertices, {tx0, 0.f, tz0}, {tx0, 0.f, tz1},
                     {ox0, -bevel_drop, oz1}, {ox0, -bevel_drop, oz0}, west_bevel);

            add_quad(vertices, {ox0, -bevel_drop, oz1}, {ox1, -bevel_drop, oz1},
                     {ox1, -side_depth, oz1}, {ox0, -side_depth, oz1}, {0.f, 0.f, 1.f});
            add_quad(vertices, {ox1, -bevel_drop, oz0}, {ox0, -bevel_drop, oz0},
                     {ox0, -side_depth, oz0}, {ox1, -side_depth, oz0}, {0.f, 0.f, -1.f});
            add_quad(vertices, {ox1, -bevel_drop, oz1}, {ox1, -bevel_drop, oz0},
                     {ox1, -side_depth, oz0}, {ox1, -side_depth, oz1}, {1.f, 0.f, 0.f});
            add_quad(vertices, {ox0, -bevel_drop, oz0}, {ox0, -bevel_drop, oz1},
                     {ox0, -side_depth, oz1}, {ox0, -side_depth, oz0}, {-1.f, 0.f, 0.f});
        }
    }

    return vertices;
}

inline void add_square_ring(std::vector<Vertex> &vertices,
                            float outer_radius, float outer_y,
                            float inner_radius, float inner_y,
                            bool bevel_normal) {
    const std::array<glm::vec3, 4> outer{{
            {-outer_radius, outer_y, outer_radius},
            {outer_radius, outer_y, outer_radius},
            {outer_radius, outer_y, -outer_radius},
            {-outer_radius, outer_y, -outer_radius}}};
    const std::array<glm::vec3, 4> inner{{
            {-inner_radius, inner_y, inner_radius},
            {inner_radius, inner_y, inner_radius},
            {inner_radius, inner_y, -inner_radius},
            {-inner_radius, inner_y, -inner_radius}}};
    const std::array<glm::vec3, 4> outward{{
            {0.f, 0.f, 1.f}, {1.f, 0.f, 0.f},
            {0.f, 0.f, -1.f}, {-1.f, 0.f, 0.f}}};

    for (int side = 0; side < 4; ++side) {
        const int next = (side + 1) % 4;
        glm::vec3 normal{0.f, 1.f, 0.f};
        if (bevel_normal) {
            const float radial = outer_radius - inner_radius;
            const float rise = inner_y - outer_y;
            normal = glm::normalize(outward[side] * rise + glm::vec3{0.f, radial, 0.f});
        }
        add_quad(vertices, outer[side], outer[next], inner[next], inner[side], normal);
    }
}

inline std::vector<Vertex> make_board_frame_ring() {
    std::vector<Vertex> vertices;
    vertices.reserve(96);

    constexpr float outer_radius = 3.30f;
    constexpr float rail_outer = 3.24f;
    constexpr float rail_inner = 3.10f;
    constexpr float inner_radius = 3.025f;
    constexpr float outer_bottom = -0.12f;
    constexpr float bevel_base = -0.055f;
    constexpr float rail_top = 0.018f;
    constexpr float inner_bottom = -0.026f;

    const std::array<glm::vec3, 4> top{{
            {-outer_radius, bevel_base, outer_radius},
            {outer_radius, bevel_base, outer_radius},
            {outer_radius, bevel_base, -outer_radius},
            {-outer_radius, bevel_base, -outer_radius}}};
    const std::array<glm::vec3, 4> bottom{{
            {-outer_radius, outer_bottom, outer_radius},
            {outer_radius, outer_bottom, outer_radius},
            {outer_radius, outer_bottom, -outer_radius},
            {-outer_radius, outer_bottom, -outer_radius}}};
    const std::array<glm::vec3, 4> normals{{
            {0.f, 0.f, 1.f}, {1.f, 0.f, 0.f},
            {0.f, 0.f, -1.f}, {-1.f, 0.f, 0.f}}};
    for (int side = 0; side < 4; ++side) {
        const int next = (side + 1) % 4;
        add_quad(vertices, bottom[side], bottom[next], top[next], top[side], normals[side]);
    }

    add_square_ring(vertices, outer_radius, bevel_base, rail_outer, rail_top, true);
    add_square_ring(vertices, rail_outer, rail_top, rail_inner, rail_top, false);
    add_square_ring(vertices, rail_inner, rail_top, inner_radius, inner_bottom, true);
    return vertices;
}

inline std::vector<std::pair<float, float>> piece_profile(const std::string &name) {
    if (name == "pawn") {
        return {{0.00f, 0.25f}, {0.08f, 0.29f}, {0.15f, 0.20f}, {0.48f, 0.12f},
                {0.55f, 0.13f}, {0.59f, 0.19f}, {0.68f, 0.22f}, {0.77f, 0.17f}, {0.83f, 0.00f}};
    }
    if (name == "rook") {
        return {{0.00f, 0.28f}, {0.09f, 0.31f}, {0.16f, 0.23f}, {0.62f, 0.18f},
                {0.68f, 0.27f}, {0.82f, 0.27f}, {0.86f, 0.21f}};
    }
    if (name == "bishop") {
        return {{0.00f, 0.27f}, {0.09f, 0.30f}, {0.17f, 0.20f}, {0.55f, 0.12f},
                {0.62f, 0.18f}, {0.72f, 0.20f}, {0.85f, 0.09f}, {0.92f, 0.00f}};
    }
    if (name == "knight") {
        return {{0.00f, 0.27f}, {0.09f, 0.30f}, {0.17f, 0.21f}, {0.55f, 0.14f},
                {0.68f, 0.21f}, {0.82f, 0.16f}, {0.91f, 0.05f}};
    }
    if (name == "queen") {
        return {{0.00f, 0.29f}, {0.09f, 0.32f}, {0.18f, 0.21f}, {0.62f, 0.13f},
                {0.70f, 0.23f}, {0.78f, 0.18f}, {0.86f, 0.27f}, {0.94f, 0.08f}, {0.99f, 0.00f}};
    }
    return {{0.00f, 0.30f}, {0.09f, 0.33f}, {0.18f, 0.22f}, {0.65f, 0.14f},
            {0.73f, 0.24f}, {0.82f, 0.18f}, {0.91f, 0.10f}, {1.02f, 0.10f}};
}

inline std::vector<Vertex> make_chess_piece(const std::string &name, int segments = 20) {
    constexpr float pi = 3.14159265358979323846f;
    const auto profile = piece_profile(name);
    std::vector<Vertex> vertices;
    vertices.reserve((profile.size() - 1) * static_cast<size_t>(segments) * 6 +
                     static_cast<size_t>(segments) * 3);

    for (size_t ring = 0; ring + 1 < profile.size(); ++ring) {
        const float h0 = profile[ring].first;
        const float r0 = profile[ring].second;
        const float h1 = profile[ring + 1].first;
        const float r1 = profile[ring + 1].second;
        const float dh = h1 - h0;
        const float dr = r1 - r0;

        for (int segment = 0; segment < segments; ++segment) {
            const float a0 = 2.0f * pi * static_cast<float>(segment) / static_cast<float>(segments);
            const float a1 = 2.0f * pi * static_cast<float>(segment + 1) / static_cast<float>(segments);
            const glm::vec3 p00{r0 * std::cos(a0), h0, r0 * std::sin(a0)};
            const glm::vec3 p01{r0 * std::cos(a1), h0, r0 * std::sin(a1)};
            const glm::vec3 p10{r1 * std::cos(a0), h1, r1 * std::sin(a0)};
            const glm::vec3 p11{r1 * std::cos(a1), h1, r1 * std::sin(a1)};
            const glm::vec3 n0 = glm::normalize(glm::vec3{dh * std::cos(a0), -dr, dh * std::sin(a0)});
            const glm::vec3 n1 = glm::normalize(glm::vec3{dh * std::cos(a1), -dr, dh * std::sin(a1)});
            const float u0 = static_cast<float>(segment) / static_cast<float>(segments);
            const float u1 = static_cast<float>(segment + 1) / static_cast<float>(segments);

            vertices.push_back(vertex(p00, {u0, h0}, n0));
            vertices.push_back(vertex(p11, {u1, h1}, n1));
            vertices.push_back(vertex(p10, {u0, h1}, n0));
            if (r1 > 0.0f) {
                vertices.push_back(vertex(p00, {u0, h0}, n0));
                vertices.push_back(vertex(p01, {u1, h0}, n1));
                vertices.push_back(vertex(p11, {u1, h1}, n1));
            }
        }
    }

    const float base_radius = profile.front().second;
    for (int segment = 0; segment < segments; ++segment) {
        const float a0 = 2.0f * pi * static_cast<float>(segment) / static_cast<float>(segments);
        const float a1 = 2.0f * pi * static_cast<float>(segment + 1) / static_cast<float>(segments);
        const glm::vec3 normal{0.0f, -1.0f, 0.0f};
        vertices.push_back(vertex({0.0f, 0.0f, 0.0f}, {0.5f, 0.5f}, normal));
        vertices.push_back(vertex({base_radius * std::cos(a1), 0.0f, base_radius * std::sin(a1)}, {0, 0}, normal));
        vertices.push_back(vertex({base_radius * std::cos(a0), 0.0f, base_radius * std::sin(a0)}, {1, 0}, normal));
    }

    return vertices;
}

}  // namespace Procedural_Mesh
